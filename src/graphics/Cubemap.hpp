#pragma once

#include <array>
#include <string>

#include <GL/glew.h>

class Cubemap  {
public:
    Cubemap();

    void loadFromFile(const std::array<std::string, 6>& locations);
    inline void use() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }
    inline GLuint operator()() { return id; }

    void destroy();
private:
    GLuint id;

    void create();
};
