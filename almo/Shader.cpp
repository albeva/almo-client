//
//  Shader.cpp
//  sdl_app
//
//  Created by Albert Varaksin on 16/08/2017.
//  Copyright © 2017 Albert Varaksin. All rights reserved.
//
#include "Shader.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>

using namespace almo;


Shader::Shader(const std::string & source, GLenum type)
{
    m_id = glCreateShader(type);
    if (m_id == 0) {
        std::cerr << "Failed to create a shader" << std::endl;
        throw EXIT_FAILURE;
    }

    char const * src = source.c_str();
    glShaderSource(m_id, 1, &src, nullptr);
    glCompileShader(m_id);

    int result;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        int len;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            std::vector<char> message(len + 1);
            glGetShaderInfoLog(m_id, len, nullptr, &message[0]);
            std::cerr << "Error: " << &message[0] << std::endl;
        } else {
            std::cerr << "Unknown error occured when compiling a shader" << std::endl;
        }
        throw EXIT_FAILURE;
    }
}


Shader::~Shader()
{
    if (m_id) {
        glDeleteShader(m_id);
    }
}


std::shared_ptr<Shader> Shader::fromFile(const std::string & filename, GLenum type)
{
    std::ifstream t(filename);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return std::make_shared<Shader>(str, type);
}
