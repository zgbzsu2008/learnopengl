#ifndef LEARNOPENGL_VERTEX_HPP
#define LEARNOPENGL_VERTEX_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cassert>

namespace learnopengl {

class Vertex
{
public:
    explicit Vertex(int format = 0);
    ~Vertex();

    template<typename T> // type==0则glBufferSubData否则glBufferData
    Vertex& bind(const T* buffer, int len, int type = GL_STATIC_DRAW);

    void render(int type = GL_TRIANGLES);
    void setVertexAttrib(int vertex, int color, int texture);

    unsigned int getVAO() const { return VAO; }
    int size() const { return vblen_ / fromat_; }

    Vertex(Vertex&& v);
    Vertex& operator=(Vertex&& v);
private:
    Vertex(const Vertex&) = delete; // VAO唯一不能拷贝
    Vertex& operator=(const Vertex&) = delete;

    int fromat_; // Vertex3Color3Texture2 = 3+3+2=8
    int vblen_, eblen_;
    unsigned int VBO, VAO, EBO;
};

Vertex::Vertex(int count)
    : fromat_(count)
    , vblen_(0)
    , eblen_(0)
{
    if (fromat_ > 0) {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }
}

Vertex::Vertex(Vertex&& v)
{
    if (this != &v) {
        if (v.fromat_ > 0) {
            fromat_ = v.fromat_;
            vblen_ = v.vblen_;
            eblen_ = v.eblen_;
            VAO = v.VAO;
            VBO = v.VBO;
            EBO = v.EBO;
            v.fromat_ = v.vblen_ = v.eblen_ = 0;
            v.VAO = v.VBO = v.EBO = 0;
        }
    }
};

Vertex& Vertex::operator=(Vertex&& v)
{
    if (this != &v) {
        if (v.fromat_ > 0) {
            fromat_ = v.fromat_;
            vblen_ = v.vblen_;
            eblen_ = v.eblen_;
            VAO = v.VAO;
            VBO = v.VBO;
            EBO = v.EBO;
            v.fromat_ = v.vblen_ = v.eblen_ = 0;
            v.VAO = v.VBO = v.EBO = 0;
        }
    }
    return *this;
}

Vertex::~Vertex()
{
    if (fromat_ > 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

template<>
Vertex& Vertex::bind(const int* buffer, int len, int type)
{
    if (len > 0) {
        if (type) {
            eblen_ = len;
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, len * sizeof(int), buffer, type);
        } else {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, len * sizeof(int), buffer);
        }
    }
    return *this;
}

template<>
Vertex& Vertex::bind(const float* buffer, int len, int type)
{
    if (len > 0) {
        if (type) {
            vblen_ = len;
            glBufferData(GL_ARRAY_BUFFER, len * sizeof(float), buffer, type);
        } else {
            glBufferSubData(GL_ARRAY_BUFFER, 0, len * sizeof(float), buffer);
        }
    }
    return *this;
}

void Vertex::setVertexAttrib(int vertex, int color, int texture)
{
    if (fromat_ <= 0) {
        fromat_ = vertex + color + texture;
    } else {
        assert(fromat_ == (vertex + color + texture));
    }

    int len = sizeof(float);
    int id = 0, ptr = 0;
    if (vertex) {
        glVertexAttribPointer(id, vertex, GL_FLOAT, GL_FALSE, fromat_ * len, (void*)ptr);
        glEnableVertexAttribArray(id++);
        ptr += vertex;
    }
    if (color) {
        glVertexAttribPointer(id, color, GL_FLOAT, GL_FALSE, fromat_ * len, (void*)(ptr * len));
        glEnableVertexAttribArray(id++);
        ptr += color;
    }
    if (texture) {
        glVertexAttribPointer(id, texture, GL_FLOAT, GL_FALSE, fromat_ * len, (void*)(ptr * len));
        glEnableVertexAttribArray(id++);
        ptr += texture;
    }
}

void Vertex::render(int type)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (eblen_ > 0) {
        glDrawElements(type, eblen_, GL_UNSIGNED_INT, 0);
    } else if (vblen_ > 0) {
        glDrawArrays(type, 0, size());
    }
}

} // namespace earnopengl

#endif // !LEARNOPENGL_VERTEX_HPP
