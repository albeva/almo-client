//
//  main.cpp
//  sdl_app
//
//  Created by Albert Varaksin on 11/08/2017.
//  Copyright © 2017 Albert Varaksin. All rights reserved.
//
#include "Display.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "Camera.hpp"
#include "Engine.hpp"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
#include <chrono>
#include <iostream>

using namespace almo;


class MainGame: public Engine
{
    // shader program
    Program m_program;
    // vertex attibute
    GLint m_attribute_coord3d;
    // color attribute
    GLint m_attribute_v_color;
    // element size
    GLsizei m_elementSize;
    // attribute locations
    GLint m_projectionLoc;
    GLint m_viewLoc;
    GLint m_modelLoc;
    // model
    glm::mat4 m_model;
    glm::vec3 m_axis_y;
    // keyboard moevements
    bool m_move[4];


    // Set up
    virtual void init() override
    {
        // shader
        m_program.attach(Shader::fromFile("simple.frag", GL_FRAGMENT_SHADER));
        m_program.attach(Shader::fromFile("simple.vert", GL_VERTEX_SHADER));
        m_program.link();
        m_program.use();

        // attribs and uniforms
        m_attribute_coord3d = m_program.getAttribLocation("pos");
        m_attribute_v_color = m_program.getAttribLocation("color");

        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        GLfloat cube_vertices[] = {
            // front
            -1.0, -1.0,  1.0,
            1.0, -1.0,  1.0,
            1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0,
            // back
            -1.0, -1.0, -1.0,
            1.0, -1.0, -1.0,
            1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0,
        };

        GLuint vbo_cube_vertices;
        glGenBuffers(1, &vbo_cube_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(m_attribute_coord3d, // attribute
                              3,                   // number of elements per vertex, here (x,y,z)
                              GL_FLOAT,            // the type of each element
                              GL_FALSE,            // take our values as-is
                              0,                   // no extra data between each position
                              0                    // offset of first element
                              );

        GLfloat cube_colors[] = {
            // front colors
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            // back colors
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            //
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
        };

        GLuint vbo_cube_colors;
        glGenBuffers(1, &vbo_cube_colors);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
        glVertexAttribPointer(m_attribute_v_color, // attribute
                              3,                   // number of elements per vertex, here (x,y,z)
                              GL_FLOAT,            // the type of each element
                              GL_FALSE,            // take our values as-is
                              0,                   // no extra data between each position
                              0                    // offset of first element
                              );

        GLushort cube_elements[] = {
            // front
            0, 1, 2,
            2, 3, 0,
            // top
            1, 5, 6,
            6, 2, 1,
            // back
            7, 6, 5,
            5, 4, 7,
            // bottom
            4, 0, 3,
            3, 7, 4,
            // left
            4, 5, 1,
            1, 0, 4,
            // right
            3, 2, 6,
            6, 7, 3,
        };
        GLuint ibo_cube_elements;
        glGenBuffers(1, &ibo_cube_elements);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
        int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        m_elementSize = size / sizeof(GLushort);

        // vars
        m_projectionLoc = m_program.getUniformLocation("projection");
        m_viewLoc       = m_program.getUniformLocation("view");
        m_modelLoc      = m_program.getUniformLocation("model");

        // cube
        m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
        m_axis_y = glm::vec3(.3f, .6f, .9f);
    }


    // Render
    virtual void render() override
    {
        // logic
        // ----------------------------------------
        glm::mat4 projection = glm::perspective(getCamera().getZoom(), (float)getDisplay().getWidth() / (float)getDisplay().getHeight(), 0.1f, 100.0f);
        m_program.setUniform(m_projectionLoc, projection);

        // Camera controls
        if (m_move[Camera::FORWARD]) {
            getCamera().processKeyboard(Camera::FORWARD, 0.2f);
        }

        if (m_move[Camera::BACKWARD]) {
            getCamera().processKeyboard(Camera::BACKWARD, 0.2f);
        }

        if (m_move[Camera::LEFT]) {
            getCamera().processKeyboard(Camera::LEFT, 0.2f);
        }

        if (m_move[Camera::RIGHT]) {
            getCamera().processKeyboard(Camera::RIGHT, 0.2);
        }

        // animate cube
        float angle = SDL_GetTicks() / 1000.0 * 90;
        glm::mat4 anim = glm::rotate(m_model, glm::radians(angle), m_axis_y);
        m_program.setUniform(m_modelLoc, anim);

        // camera
        auto view = getCamera().getViewMatrix();
        m_program.setUniform(m_viewLoc, view);

        // draw
        glEnableVertexAttribArray(m_attribute_coord3d);
        glEnableVertexAttribArray(m_attribute_v_color);
        glDrawElements(GL_TRIANGLES, m_elementSize, GL_UNSIGNED_SHORT, 0);
        glDisableVertexAttribArray(m_attribute_coord3d);
        glDisableVertexAttribArray(m_attribute_v_color);
    }


    // Handle events
    virtual void handle(const SDL_Event& event) override
    {
        switch (event.type) {
            case SDL_MOUSEMOTION:
                getCamera().processMouseMovement((float)event.motion.xrel, -(float)event.motion.yrel);
                break;
            case SDL_MOUSEWHEEL:
                getCamera().processMouseScroll((float)event.wheel.y / 10);
                break;
            case SDL_KEYDOWN:
                handleKey(event.key.keysym.scancode, true);
                break;
            case SDL_KEYUP:
                handleKey(event.key.keysym.scancode, false);
                break;
        }
    }


    // handle keys
    void handleKey(SDL_Scancode code, bool pressed) {
        switch (code) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                m_move[Camera::FORWARD] = pressed;
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                m_move[Camera::BACKWARD] = pressed;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                m_move[Camera::LEFT] = pressed;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                m_move[Camera::RIGHT] = pressed;
                break;
            default:
                return;
        }
    }
};


int main(int argc, char * argv[])
{
    return MainGame().run();
    return EXIT_SUCCESS;
}
