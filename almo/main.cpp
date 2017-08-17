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
using namespace almo;


int main(int argc, const char * argv[]) {

    Display display(800, 600);

    Program program;
    program.attach(Shader::fromFile("simple.frag", GL_FRAGMENT_SHADER));
    program.attach(Shader::fromFile("simple.vert", GL_VERTEX_SHADER));
    program.link();
    program.use();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                goto exit_loop;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        display.swap();
    }
    exit_loop:


    return EXIT_SUCCESS;
}
