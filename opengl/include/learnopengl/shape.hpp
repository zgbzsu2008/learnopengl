#ifndef LEARNOPENGL_SHAPE_HPP
#define LEARNOPENGL_SHAPE_HPP

#include <learnopengl/vertex.hpp>
#include <learnopengl/shader.hpp>
#include <learnopengl/text.hpp>

namespace learnopengl {

class Shape
{
public:
    Shape(int n, bool show = true)
        : n_(n)
        , v_(6)
        , show_(show)
    {
        this->genMesh();
    }

    Shape& render(Shader& shader, int type = GL_TRIANGLES)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f));
        shader.activate().bind("model", model);
        v_.render(type);

        return *this;
    }

    Shape& render(Shader& shader, Text& text) 
    {
        float pos = (n_ + 3) *0.1f;
        text.render(shader, u"x", glm::vec3(pos, 0.0f, 0.0f), 3.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        text.render(shader, u"y", glm::vec3(0.0f, pos, 0.0f), 3.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        text.render(shader, u"z", glm::vec3(0.0f, 0.0f, pos), 3.0f, glm::vec3(0.0f, 0.0f, 1.0f));

        for (int n = 0; n < 10; ++n) {
            auto ustr = learnopengl::utf8rtou16s(std::to_string(n));
            text.render(shader, ustr, glm::vec3(0.1f * n, 0.0f, 0.0f), 0.5f);
            text.render(shader, ustr, glm::vec3(0.0f, 0.1f * n, 0.0f), 0.5f);
            text.render(shader, ustr, glm::vec3(0.0f, 0.0f, 0.1f * n), 0.5f);
        }

        return *this;
    }

private:
    void genMesh();

    Vertex v_;
    int n_;
    bool show_;
};

void Shape::genMesh()
{
    std::vector<glm::vec3> mesh;
    float size = 1.0f * (n_ + 3);

    // x轴 green
    mesh.emplace_back(0.0f, 0.0f, 0.0f);
    mesh.emplace_back(0.0f, 1.0f, 0.0f);
    mesh.emplace_back(size, 0.0f, 0.0f);
    mesh.emplace_back(0.0f, 1.0f, 0.0f);

    // 小箭头
    mesh.emplace_back(size, 0.0f, 0.0f);
    mesh.emplace_back(0.0f, 1.0f, 0.0f);
    mesh.emplace_back(size - 1.0f, 0.3f, 0.0f);
    mesh.emplace_back(0.0f, 1.0f, 0.0f);
    mesh.emplace_back(size, 0.0f, 0.0f);
    mesh.emplace_back(0.0f, 1.0f, 0.0f);
    mesh.emplace_back(size - 1.0f, 0.0f, 0.3f);
    mesh.emplace_back(0.0f, 1.0f, 0.0f);

    // y轴 blue
    mesh.emplace_back(0.0f, 0.0f, 0.0f);
    mesh.emplace_back(0.0f, 0.0f, 1.0f);
    mesh.emplace_back(0.0f, size, 0.0f);
    mesh.emplace_back(0.0f, 0.0f, 1.0f);

    // 小箭头
    mesh.emplace_back(0.0f, size, 0.0f);
    mesh.emplace_back(0.0f, 0.0f, 1.0f);
    mesh.emplace_back(0.5f, size - 1.0f, 0.0f);
    mesh.emplace_back(0.0f, 0.0f, 1.0f);
    mesh.emplace_back(0.0f, size, 0.0f);
    mesh.emplace_back(0.0f, 0.0f, 1.0f);
    mesh.emplace_back(0.0f, size - 1.0f, 0.5f);
    mesh.emplace_back(0.0f, 0.0f, 1.0f);

    // z轴 red
    mesh.emplace_back(0.0f, 0.0f, 0.0f);
    mesh.emplace_back(1.0f, 0.0f, 0.0f);
    mesh.emplace_back(0.0f, 0.0f, size);
    mesh.emplace_back(1.0f, 0.0f, 1.0f);

    // 小箭头
    mesh.emplace_back(0.0f, 0.0f, size);
    mesh.emplace_back(1.0f, 0.0f, 0.0f);
    mesh.emplace_back(0.5f, 0.0f, size - 1.0f);
    mesh.emplace_back(1.0f, 0.0f, 1.0f);
    mesh.emplace_back(0.0f, 0.0f, size);
    mesh.emplace_back(1.0f, 0.0f, 0.0f);
    mesh.emplace_back(0.0f, 0.5f, size - 1.0f);
    mesh.emplace_back(1.0f, 0.0f, 1.0f);

    size = 1.0f * (n_ - 1);
    if (show_) {
        for (int x = 1; x < n_; ++x) {
            mesh.emplace_back(1.0f * x, 0.0f, 0.0f);
            mesh.emplace_back(0.0f, 0.0f, 1.0f);
            mesh.emplace_back(1.0f * x, 0.0f, size);
            mesh.emplace_back(0.0f, 0.0f, 1.0f);
            mesh.emplace_back(1.0f * x, 0.0f, 0.0f);
            mesh.emplace_back(1.0f, 0.0f, 0.0f);
            mesh.emplace_back(1.0f * x, size, 0.0f);
            mesh.emplace_back(1.0f, 0.0f, 0.0f);
        }

        for (int y = 1; y < n_; ++y) {
            mesh.emplace_back(0.0f, 1.0f * y, 0.0f);
            mesh.emplace_back(1.0f, 0.0f, 0.0f);
            mesh.emplace_back(size, 1.0f * y, 0.0f);
            mesh.emplace_back(1.0f, 0.0f, 0.0f);
            mesh.emplace_back(0.0f, 1.0f * y, 0.0f);
            mesh.emplace_back(0.0f, 1.0f, 0.0f);
            mesh.emplace_back(0.0f, 1.0f * y, size);
            mesh.emplace_back(0.0f, 1.0f, 0.0f);
        }

        for (int z = 1; z < n_; ++z) {
            mesh.emplace_back(0.0f, 0.0f, 1.0f * z);
            mesh.emplace_back(0.0f, 0.0f, 1.0f);
            mesh.emplace_back(size, 0.0f, 1.0f * z);
            mesh.emplace_back(0.0f, 0.0f, 1.0f);
            mesh.emplace_back(0.0f, 0.0f, 1.0f * z);
            mesh.emplace_back(0.0f, 1.0f, 0.0f);
            mesh.emplace_back(0.0f, size, 1.0f * z);
            mesh.emplace_back(0.0f, 1.0f, 0.0f);
        }
    }
    v_.bind((float*)mesh.data(), mesh.size() * 3).setVertexAttrib(3, 3, 0);
}

} // namespace learnopengl

#endif // LEARNOPENGL_SHAPE_HPP
