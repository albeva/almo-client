//
//  Display.hpp
//  sdl_app
//
//  Created by Albert Varaksin on 16/08/2017.
//  Copyright © 2017 Albert Varaksin. All rights reserved.
//
#pragma once
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>

namespace almo {

    /**
     * Game Window
     */
    class Display {
    public:

        Display(const std::string& title, int width, int height);

        ~Display();

        void swap();

        void setTitle(const std::string& title);

        int getWidth() const { return m_width; }
        
        int getHeight() const { return m_height; }

        SDL_Window* getWindow() const { return m_window; }

        SDL_GLContext getContext() const { return m_context; }
        
    private:
        int m_width;
        int m_height;
        SDL_Window * m_window;
        SDL_GLContext m_context;
    };


}
