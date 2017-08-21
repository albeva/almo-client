//
//  Engine.hpp
//  almo
//
//  Created by Albert Varaksin on 20/08/2017.
//  Copyright © 2017 Albert Varaksin. All rights reserved.
//
#pragma once
#include <memory>
#include <SDL2/SDL.h>

namespace almo {

    class Display;
    class Camera;

    // Game Engine
    class Engine {
    public:

        Engine();

        Display& getDisplay();

        Camera& getCamera();

        // set things up
        virtual void init() = 0;

        // render the scene
        virtual void render() = 0;

        // Add custom handling to SDL events
        virtual void handle(const SDL_Event& event) {}

        // run the engine
        int run();

    private:
        std::unique_ptr<Display> m_display;
        std::unique_ptr<Camera> m_camera;
    };

}
