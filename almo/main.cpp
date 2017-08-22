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
    Program m_program;
    bool m_move[4];
    GLuint m_vertexbuffer;
    GLuint m_colorbuffer;
    GLint m_vertxAttribute;
    GLint m_colorAttribute;
    GLint m_projectionLoc;
    GLint m_viewLoc;
    GLint m_modelLoc;
    glm::mat4 m_model;
    glm::vec3 m_axis_y;


    // Set up
    virtual void init() override
    {
        // shader
        m_program.attach(Shader::fromFile("simple.frag", GL_FRAGMENT_SHADER));
        m_program.attach(Shader::fromFile("simple.vert", GL_VERTEX_SHADER));
        m_program.link();
        m_program.use();

        // locations
        m_vertxAttribute = m_program.getAttribLocation("pos");
        m_colorAttribute = m_program.getAttribLocation("color");
        m_projectionLoc  = m_program.getUniformLocation("projection");
        m_viewLoc        = m_program.getUniformLocation("view");
        m_modelLoc       = m_program.getUniformLocation("model");

        // VAO
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
        // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
        static const GLfloat g_vertex_buffer_data[] = {
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
        };

        // One color for each vertex. They were generated randomly.
        static const GLfloat g_color_buffer_data[] = {
            1.f,  1.f,  1.f, // A
            1.f,  1.f,  1.f,
            1.f,  1.f,  1.f,
            0.f,  0.f,  0.f, // B
            0.f,  0.f,  0.f,
            0.f,  0.f,  0.f,
            0.f,  1.f,  1.f, // C
            0.f,  1.f,  1.f,
            0.f,  1.f,  1.f,
            0.f,  0.f,  0.f, // B
            0.f,  0.f,  0.f,
            0.f,  0.f,  0.f,
            1.f,  1.f,  1.f, // A
            1.f,  1.f,  1.f,
            1.f,  1.f,  1.f,
            0.f,  1.f,  1.f, // C
            0.f,  1.f,  1.f,
            0.f,  1.f,  1.f,
            1.f,  0.f,  1.f, // D
            1.f,  0.f,  1.f,
            1.f,  0.f,  1.f,
            1.f,  1.f,  0.f, // E
            1.f,  1.f,  0.f,
            1.f,  1.f,  0.f,
            1.f,  1.f,  0.f, // E
            1.f,  1.f,  0.f,
            1.f,  1.f,  0.f,
            .3f,  .6f,  .9f, // F
            .3f,  .6f,  .9f,
            .3f,  .6f,  .9f,
            .3f,  .6f,  .9f, // F
            .3f,  .6f,  .9f,
            .3f,  .6f,  .9f,
            1.f,  0.f,  1.f, // D
            1.f,  0.f,  1.f,
            1.f,  0.f,  1.f,
        };

        glGenBuffers(1, &m_vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &m_colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

        // cube
        m_model  = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -4.f));
        m_axis_y = glm::vec3(.3f, .6f, .9f);
    }


    // Render
    virtual void render() override
    {
        m_program.use();

        move();

        // logic
        glm::mat4 projection = glm::perspective(getCamera().getZoom(), (float)getDisplay().getWidth() / (float)getDisplay().getHeight(), 0.1f, 100.0f);
        m_program.setUniform(m_projectionLoc, projection);

        // animate cube
        float angle = SDL_GetTicks() / 1000.0 * 90;
        glm::mat4 anim = glm::rotate(m_model, glm::radians(angle), m_axis_y);
        m_program.setUniform(m_modelLoc, anim);

        // camera
        auto view = getCamera().getViewMatrix();
        m_program.setUniform(m_viewLoc, view);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(m_vertxAttribute);
        std::cout << m_colorAttribute << '\n';
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
        glVertexAttribPointer(m_vertxAttribute,   // attribute. No particular reason for 0, but must match the layout in the shader.
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(m_colorAttribute);
        glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
        glVertexAttribPointer(m_colorAttribute,   // attribute. No particular reason for 1, but must match the layout in the shader.
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

        glDisableVertexAttribArray(m_vertxAttribute);
        glDisableVertexAttribArray(m_colorAttribute);
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


    // moev the camera
    void move() {
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
    }
};


int main(int argc, char * argv[])
{
    return MainGame().run();
    return EXIT_SUCCESS;
}
