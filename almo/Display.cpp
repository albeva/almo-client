//
//  Display.cpp
//  sdl_app
//
//  Created by Albert Varaksin on 16/08/2017.
//  Copyright © 2017 Albert Varaksin. All rights reserved.
//
#include "Display.hpp"
#include <iostream>
using namespace almo;


Display::Display(const std::string& title, int width, int height)
: m_width(width)
, m_height(height)
, m_window(nullptr)
, m_context(0)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        throw EXIT_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (m_window == nullptr) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        throw EXIT_FAILURE;
    }

    m_context = SDL_GL_CreateContext(m_window);
    if (m_context == nullptr) {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        throw EXIT_FAILURE;
    }

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize OpenGL" << std::endl;
        throw EXIT_FAILURE;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(SDL_DISABLE);

    // SDL_GL_SetSwapInterval(0);
    SDL_GL_GetDrawableSize(m_window, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}


Display::~Display()
{
    if (m_context) {
        SDL_GL_DeleteContext(m_context);
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
    }

    SDL_Quit();
}


void Display::setTitle(const std::string &title)
{
    SDL_SetWindowTitle(m_window, title.c_str());
}


void Display::swap()
{
    SDL_GL_SwapWindow(m_window);
}
