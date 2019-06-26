#ifndef LEARNOPENGL_TEXT
#define LEARNOPENGL_TEXT

#include <map>
#include <uchar.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

#include <learnopengl/shader.hpp>
#include <learnopengl/texture.hpp>
#include <learnopengl/vertex.hpp>
#include <learnopengl/codecvt.hpp>

namespace learnopengl {

class Text
{
public:
    Text(const std::string& font, const std::string& root = "./");
    ~Text();

    void genTextTexture(char16_t c);
    void genTextTexture(char16_t begin, char16_t end);
    void genTextTexture(const std::u16string& wstr);

    Text& setPixelSize(int width, int height);
    Text& setItalic(float italic = 0.5f);
    Text& setBold();

    // glm::vec2 pos 屏幕左下角(0,0), 右上角(800,600)
    void render(Shader& shader, const std::u16string& text, glm::vec2 pos = glm::vec2(0.0f), float scale = 1.0f,
                const glm::vec3& color = glm::vec3(1.0f), glm::vec2 screem = glm::vec2(800.0f, 600.0f));

    // glm::vec3 pos 标准坐标[-1, 1], z深度
    void render(Shader& shader, const std::u16string& text, glm::vec3 pos = glm::vec3(0.0f), float scale = 1.0f,
                const glm::vec3& color = glm::vec3(1.0f), glm::vec2 screem = glm::vec2(800.0f, 600.0f));

private:
    struct Character
    {
        unsigned int texture;
        glm::ivec2 size;
        glm::ivec2 bearing;
        unsigned int advance;
    };

    std::map<char16_t, Character> characters_;

    FT_Library ft_;
    FT_Face face_;
};

Text::Text(const std::string& font, const std::string& path)
    : ft_(0)
    , face_(0)
{
    if (FT_Init_FreeType(&ft_)) {
        std::cout << "Could not init freetype library" << std::endl;
    }
    if (FT_New_Face(ft_, (path + font).c_str(), 0, &face_)) {
        std::cout << "Failed to load font" << std::endl;
    }

    if (FT_Select_Charmap(face_, FT_ENCODING_UNICODE)) {
        std::cout << "Failed to select charmap" << std::endl;
    }
}

Text::~Text()
{
    if (face_) {
        FT_Done_Face(face_);
    }
    if (ft_) {
        FT_Done_FreeType(ft_);
    }
}

Text& Text::setPixelSize(int width, int height)
{
    FT_Set_Pixel_Sizes(face_, width, height);
    return *this;
}

Text& Text::setItalic(float italic)
{
    FT_Matrix mat2;
    mat2.xx = 0x10000L;
    mat2.xy = FT_Fixed(italic * 0x10000L);
    mat2.yx = 0;
    mat2.yy = 0x10000L;
    FT_Set_Transform(face_, &mat2, nullptr);

    return *this;
}

Text& Text::setBold()
{
    int strength = 1 << 6;
    if (face_->glyph->format == FT_GLYPH_FORMAT_OUTLINE) {
        FT_Outline_Embolden(&face_->glyph->outline, strength);
    }

    return *this;
}

void Text::genTextTexture(char16_t c)
{
    if (characters_.insert({ c, Character() }).second) {
        if (FT_Load_Char(face_, c, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP)) { // FT_LOAD_NO_BITMAP
            std::cout << "Failed to load glyph : " << c << std::endl;
        }

        int width = face_->glyph->bitmap.width;
        int height = face_->glyph->bitmap.rows;

        characters_[c] = { learnopengl::textureFromBuffer(width, height, GL_RED, face_->glyph->bitmap.buffer),
                           glm::ivec2(width, height), glm::ivec2(face_->glyph->bitmap_left, face_->glyph->bitmap_top),
                           static_cast<unsigned int>(face_->glyph->advance.x) };
    }
}

void Text::genTextTexture(char16_t begin, char16_t end)
{
    for (char16_t c = begin; c < end; ++c) {
        genTextTexture(c);
    }
}

void Text::genTextTexture(const std::u16string& wstr)
{
    for (auto c : wstr) {
        genTextTexture(c);
    }
}

void Text::render(Shader& shader, const std::u16string& text, glm::vec2 pos, float scale, const glm::vec3& color,
                  glm::vec2 screem)
{
    glm::mat4 projection = glm::ortho(0.0f, screem.x, 0.0f, screem.y);
    shader.activate().bind("projection", projection).bind("view", glm::mat4(1.0f)).bind("model", glm::mat4(1.0f));

    Vertex v(4);
    v.bind<float>(0, 24, GL_DYNAMIC_DRAW).setVertexAttrib(2, 2, 0);
    glActiveTexture(GL_TEXTURE0);
    shader.activate().bind("textColor", color).bind("text", 0);

    Character ch;
    for (auto c : text) {
        ch = characters_[c];
        float x = pos.x + ch.bearing.x * scale;
        float y = pos.y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[] = { x, y + h, 0.0, 0.0, x,     y, 0.0, 1.0, x + w, y,     1.0, 1.0,
                             x, y + h, 0.0, 0.0, x + w, y, 1.0, 1.0, x + w, y + h, 1.0, 0.0 };
        glBindTexture(GL_TEXTURE_2D, ch.texture);
        v.bind(vertices, 24, 0).render();
        pos.x += (ch.advance >> 6) * scale;
    }
}

void Text::render(Shader& shader, const std::u16string& text, glm::vec3 pos, float scale, const glm::vec3& color,
                  glm::vec2 screem)
{
    Vertex v(5);
    v.bind<float>(0, 30, GL_DYNAMIC_DRAW).setVertexAttrib(3, 2, 0);
    shader.activate().bind("textColor", color).bind("model", glm::mat4(1.0f));

    glActiveTexture(GL_TEXTURE0);
    shader.activate().bind("text", 0);

    pos.x *= screem.x;
    pos.y *= screem.y;
    Character ch;
    for (auto c : text) {
        ch = characters_[c];
        float x = pos.x + ch.bearing.x * scale;
        float y = pos.y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        float z = pos.z;

        x /= screem.x;
        y /= screem.y;
        w /= screem.x;
        h /= screem.y;

        float vertices[] = { x, y + h, z, 0.0, 0.0, x,     y, z, 0.0, 1.0, x + w, y,     z, 1.0, 1.0,
                             x, y + h, z, 0.0, 0.0, x + w, y, z, 1.0, 1.0, x + w, y + h, z, 1.0, 0.0 };
        glBindTexture(GL_TEXTURE_2D, ch.texture);
        v.bind(vertices, 30, 0).render();
        pos.x += (ch.advance >> 6) * scale;
    }
}

} // namespace learnopengl

#endif // !LEARNOPENGL_TEXT
