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
    return glGetUniformLocation(m_id, name.c_str());
}

