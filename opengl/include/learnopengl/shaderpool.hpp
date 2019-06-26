#ifndef LEARNOPENGL_SHADERPOOL_HPP
#define LEARNOPENGL_SHADERPOOL_HPP

#include <map>
#include <vector>

#include <learnopengl/shader.hpp>

namespace learnopengl {
class ShaderPool
{
public:
    ShaderPool() {}
    ~ShaderPool();

    ShaderPool& add(const std::string& name, Shader* shader);
    ShaderPool& add(const std::vector<std::string>& list); // list[0] = name
    ShaderPool& remove(const std::string& name);

    Shader& get(const std::string& name);

private:
    std::map<std::string, Shader*> pool_;
};

ShaderPool::~ShaderPool()
{
    for (auto it : pool_) {
        if (it.second) {
            delete it.second;
        }
    }
    pool_.clear();
}

ShaderPool& ShaderPool::add(const std::string& name, Shader* shader)
{
    assert(pool_.find(name) == pool_.end());
    pool_[name] = shader;
    return *this;
}

ShaderPool& ShaderPool::add(const std::vector<std::string>& list)
{
    assert(list.size() > 1);
    Shader* shader = new Shader;
    for (auto s : list) {
        shader->attach(s);
    }
    shader->link();
    auto index = list[0].rfind(".");
    auto name = list[0].substr(0, index);
    pool_[name] = shader;

    return *this;
}

ShaderPool& ShaderPool::remove(const std::string& name)
{
    auto it = pool_.find(name);
    if (it != pool_.end()) {
        delete it->second;
        pool_.erase(it);
    }
    return *this;
}

Shader& ShaderPool::get(const std::string& name)
{
    assert(pool_.find(name) != pool_.end());
    return *pool_[name];
}

} // namespace learnopengl

#endif // !LEARNOPENGL_SHADERPOOL_HPP
