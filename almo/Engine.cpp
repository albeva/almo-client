//
//  Engine.cpp
//  almo
//
//  Created by Albert Varaksin on 20/08/2017.
//  Copyright © 2017 Albert Varaksin. All rights reserved.
//
#include "Engine.hpp"
#include "Display.hpp"
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


Engine::Engine()
{
    m_display = std::make_unique<Display>("Almo", 800, 600);
    m_camera = std::make_unique<Camera>();
}


Display& Engine::getDisplay() {
    return *m_display.get();
}


Camera& Engine::getCamera() {
    return *m_camera.get();
}


int Engine::run() {
    init();
    
    SDL_Event event;
    size_t frameCounter = 0;
    auto startTime = std::chrono::steady_clock::now();
    while (true) {
        // handle events
        // ----------------------------------------
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return EXIT_SUCCESS;
            }
            handle(event);
        }

        // FPS
        // ----------------------------------------
        frameCounter += 1;
        auto currentTime = std::chrono::steady_clock::now();
        if (currentTime - startTime >= std::chrono::seconds{1}) {
            getDisplay().setTitle(std::string("Almo - ") + std::to_string(frameCounter) + "fps");
            startTime = currentTime;
            frameCounter = 0;
        }

        // clear
        // ----------------------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw
        // ----------------------------------------
        render();

        // swap
        // ----------------------------------------
        getDisplay().swap();
    }
}
