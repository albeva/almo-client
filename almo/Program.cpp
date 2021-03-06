//
//  Program.cpp
//  sdl_app
//
//  Created by Albert Varaksin on 16/08/2017.
//  Copyright © 2017 Albert Varaksin. All rights reserved.
//
#include "Program.hpp"
#include "Shader.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>
using namespace almo;


Program::Program() {
    m_id = glCreateProgram();
    if (m_id == 0) {
        std::cerr << "Failed to create shader program" << std::endl;
        throw EXIT_FAILURE;
    }
}


Program::~Program() {
    if (m_id) {
        glDeleteProgram(m_id);
    }
}


void Program::attach(std::shared_ptr<Shader> shader) {
    m_shaders.push_back(shader);
    glAttachShader(m_id, shader->getId());
}


void Program::link() {
    // link
    glLinkProgram(m_id);

    // any errors?
    int result;
    glGetProgramiv(m_id, GL_LINK_STATUS, &result);
    if (result != GL_TRUE) {
        int len;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            std::vector<char> message(len + 1);
            glGetProgramInfoLog(m_id, len, nullptr, &message[0]);
            std::cerr << "Error: " << &message[0] << std::endl;
        } else {
            std::cerr << "Unknown error occured when linking shaders" << std::endl;
        }
        throw EXIT_FAILURE;
    }

    // release the shaders
    for (auto const & shader: m_shaders) {
        glDetachShader(m_id, shader->getId());
    }
    m_shaders.clear();
}


void Program::use() {
    glUseProgram(m_id);
}


GLuint Program::getUniformLocation(const std::string& name) const {
    auto loc =  glGetUniformLocation(m_id, name.c_str());
    if (loc == -1) {
        std::cerr << "Could not find uniform '" << name << "'" << std::endl;
        throw EXIT_FAILURE;
    }
    return loc;
}


GLuint Program::getAttribLocation(const std::string& name) const {
    auto loc =  glGetAttribLocation(m_id, name.c_str());
    if (loc == -1) {
        std::cerr << "Could not find attribute '" << name << "'" << std::endl;
        throw EXIT_FAILURE;
    }
    return loc;
}


void Program::setUniform(GLint loc, const glm::mat4& matrix)
{
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}


void Program::setUniform(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}
