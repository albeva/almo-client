//
//  Program.hpp
//  sdl_app
//
//  Created by Albert Varaksin on 16/08/2017.
//  Copyright © 2017 Albert Varaksin. All rights reserved.
//
#pragma once
#include <GL/glew.h>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace almo {

    class Shader;


    class Program {
    public:

        Program();

        ~Program();

        void attach(std::shared_ptr<Shader> shader);

        void link();

        void use();

        // Get locations

        GLuint getUniformLocation(const std::string& name) const;

        GLuint getAttribLocation(const std::string& name) const;

        // Set matrix

        void setUniform(GLint loc, const glm::mat4& matrix);
        void setUniform(const std::string& name, const glm::mat4& matrix);


    private:
        GLuint m_id;
        std::vector<std::shared_ptr<Shader>> m_shaders;
    };

}
