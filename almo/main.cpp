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

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
#include <chrono>
#include <iostream>

using namespace almo;


int main(int argc, char * argv[])
{
    // screen
    Display display("Almo", 800, 600);

    // shader
    Program program;
    program.attach(Shader::fromFile("simple.frag", GL_FRAGMENT_SHADER));
    program.attach(Shader::fromFile("simple.vert", GL_VERTEX_SHADER));
    program.link();
    program.use();

    // attribs and uniforms
    GLint attribute_coord3d = program.getAttribLocation("pos");
    GLint attribute_v_color = program.getAttribLocation("color");

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
    glVertexAttribPointer(attribute_coord3d, // attribute
                          3,                 // number of elements per vertex, here (x,y,z)
                          GL_FLOAT,          // the type of each element
                          GL_FALSE,          // take our values as-is
                          0,                 // no extra data between each position
                          0                  // offset of first element
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
    glVertexAttribPointer(attribute_v_color, // attribute
                          3,                 // number of elements per vertex, here (x,y,z)
                          GL_FLOAT,          // the type of each element
                          GL_FALSE,          // take our values as-is
                          0,                 // no extra data between each position
                          0                  // offset of first element
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
    GLsizei elementSize = size / sizeof(GLushort);


    // set up camera

    GLint projectionLoc = program.getUniformLocation("projection");
    GLint viewLoc       = program.getUniformLocation("view");
    GLint modelLoc      = program.getUniformLocation("model");

    // cube
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
    glm::vec3 axis_y(.3f, .6f, .9f);

    // camera
    Camera camera;

    // fps meter
    size_t frameCounter = 0;
    auto startTime = std::chrono::steady_clock::now();
    bool keys[1024];
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // MARK: Run the loop
    SDL_Event event;
    while (true) {
        // handle events
        // ----------------------------------------
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return EXIT_SUCCESS;
                    break;
                case SDL_MOUSEMOTION:
                    camera.processMouseMovement((float)event.motion.xrel, -(float)event.motion.yrel);
                    break;
                case SDL_MOUSEWHEEL:
                    camera.processMouseScroll((float)event.wheel.y / 10);
                    break;
                case SDL_KEYDOWN:
                {
                    auto code = event.key.keysym.sym;
                    if (code >= 0 && code < sizeof(keys)) {
                        keys[code] = true;
                    }
                    break;
                }
                case SDL_KEYUP:
                {
                    auto code = event.key.keysym.sym;
                    if (code >= 0 && code < sizeof(keys)) {
                        keys[code] = false;
                    }
                    break;
                }
            }
        }

        // FPS
        // ----------------------------------------
        frameCounter += 1;
        auto currentTime = std::chrono::steady_clock::now();
        if (currentTime - startTime >= std::chrono::seconds{1}) {
            display.setTitle(std::string("Almo - ") + std::to_string(frameCounter) + "fps");
            startTime = currentTime;
            frameCounter = 0;
        }

        // clear
        // ----------------------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // logic
        // ----------------------------------------
        glm::mat4 projection = glm::perspective(camera.getZoom(), (float)display.getWidth() / (float)display.getHeight(), 0.1f, 100.0f);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Camera controls
        if( keys[SDLK_w])
        {
            camera.processKeyboard(Camera::FORWARD, 0.2);
        }

        if( keys[SDLK_s])
        {
            camera.processKeyboard(Camera::BACKWARD, 0.2);
        }

        if( keys[SDLK_a])
        {
            camera.processKeyboard(Camera::LEFT, 0.2);
        }

        if( keys[SDLK_d])
        {
            camera.processKeyboard(Camera::RIGHT, 0.2);
        }

        float angle = SDL_GetTicks() / 1000.0 * 90;  // 45° per second
        glm::mat4 anim = glm::rotate(model, glm::radians(angle), axis_y);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(anim));
        auto view = camera.getViewMatrix();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // draw
        // ----------------------------------------
        glEnableVertexAttribArray(attribute_coord3d);
        glEnableVertexAttribArray(attribute_v_color);
        glDrawElements(GL_TRIANGLES, elementSize, GL_UNSIGNED_SHORT, 0);
        glDisableVertexAttribArray(attribute_coord3d);
        glDisableVertexAttribArray(attribute_v_color);

        // swap
        // ----------------------------------------
        display.swap();
    }

    return EXIT_SUCCESS;
}
