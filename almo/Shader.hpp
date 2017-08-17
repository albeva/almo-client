//
//  Shader.hpp
//  sdl_app
//
//  Created by Albert Varaksin on 16/08/2017.
//  Copyright © 2017 Albert Varaksin. All rights reserved.
//
#pragma once
#include <GL/glew.h>
#include <string>

namespace almo {


    class Shader {
    public:

        Shader(const std::string & source, GLenum type);

        ~Shader();

        static std::shared_ptr<Shader> fromFile(const std::string & filename, GLenum type);

        GLuint getId() const { return m_id; }

        GLenum getType() const { return m_type; }

    private:
        GLuint m_id;
        GLenum m_type;
    };

}
