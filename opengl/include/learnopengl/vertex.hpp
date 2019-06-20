#ifndef LEARNOPENGL_VERTEX
#define LEARNOPENGL_VERTEX

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cassert>

namespace learnopengl {

class Vertex
{
public:
    Vertex(const float* vertices, int vlen, const int* indices = 0, int len = 0);
    ~Vertex();

    void render();
    void setVertexAttrib(int vertex, int color, int texture);

    unsigned int getVAO() const { return VAO; }
    int count() const { return count_; }

private:
    unsigned int VBO, VAO, EBO;
    int count_;
    std::vector<float> vertices_;
    std::vector<int> indices_;
};

Vertex::Vertex(const float* vertices, int vlen, const int* indices, int len)
    : vertices_(vertices, vertices + vlen)
    , indices_(indices, indices + len)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    if (!vertices_.empty()) {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), vertices_.data(), GL_STATIC_DRAW);
    }

    if (!indices_.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(float), indices_.data(), GL_STATIC_DRAW);
    }
}

Vertex::~Vertex()
{
    glDeleteVertexArrays(1, &VAO);
    if (!vertices_.empty()) {
        glDeleteBuffers(1, &VBO);
    }

    if (!indices_.empty()) {
        glDeleteBuffers(1, &EBO);
    }
}

void Vertex::setVertexAttrib(int vertex, int color, int texture)
{
    int i = 0, j = 0;
    count_ = vertex + color + texture;
    if (vertex) {
        glVertexAttribPointer(i, vertex, GL_FLOAT, GL_FALSE, count_ * sizeof(float), (void*)j);
        glEnableVertexAttribArray(i++);
        j += vertex;
    }

    if (color) {
        glVertexAttribPointer(i, color, GL_FLOAT, GL_FALSE, count_ * sizeof(float), (void*)(j * sizeof(float)));
        glEnableVertexAttribArray(i++);
        j += color;
    }

    if (texture) {
        glVertexAttribPointer(i, texture, GL_FLOAT, GL_FALSE, count_ * sizeof(float), (void*)(j * sizeof(float)));
        glEnableVertexAttribArray(i++);
        j += texture;
    }
}

void Vertex::render()
{
    glBindVertexArray(VAO);
    if (!indices_.empty()) {
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertices_.size() / count_);
    }
}
} // learnopengl

#endif // !LEARNOPENGL_VERTEX
