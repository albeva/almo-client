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

    // MARK: setup the scene
    GLuint vbo_cube_vertices, vbo_cube_colors;
    GLuint ibo_cube_elements;
    GLint attribute_coord3d, attribute_v_color;
    GLint uniform_mvp;

    // program
    attribute_coord3d = 0; //program.getAttribLocation("coord3d");
    attribute_v_color = 1; //program.getAttribLocation("v_color");
    uniform_mvp = program.getUniformLocation("MVP");

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
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        // back colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
    };
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
    glGenBuffers(1, &ibo_cube_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
    int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    // view
    glm::mat4 projection = glm::perspective(45.0f, (float)display.getWidth() / (float)display.getHeight(), 0.1f, 10.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
    glm::vec3 axis_y(0.3, 0.6, 0.9);

    // fps meter
    size_t frameCounter = 0;
    auto startTime = std::chrono::steady_clock::now();

    // MARK: Run the loop
    SDL_Event event;
    while (true) {
        // handle events
        // ----------------------------------------
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return EXIT_SUCCESS;
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
        float angle = SDL_GetTicks() / 1000.0 * 45;  // 45° per second
        glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_y);
        glm::mat4 mvp = projection * view * model * anim;

        program.use();
        glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

        // draw
        // ----------------------------------------
        glEnableVertexAttribArray(attribute_coord3d);
        glEnableVertexAttribArray(attribute_v_color);
        glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
        glDisableVertexAttribArray(attribute_coord3d);
        glDisableVertexAttribArray(attribute_v_color);

        // swap
        // ----------------------------------------
        display.swap();
    }

    return EXIT_SUCCESS;
}
