//
// Created by Wendelen Daan on 15/11/2017.
//

#include <iostream>
#include <SDL_timer.h>

#include "ConsoleRenderer.hpp"

void console::ConsoleRenderer::init() {
    std::cout << "Init" << std::endl;
}

void console::ConsoleRenderer::render(engine::Scene &scene) {
    std::cout << "Render scene" << std::endl;
    SDL_Delay(10);
}

void console::ConsoleRenderer::loadModel(engine::Model &model) {
    std::cout << "Load model" << std::endl;
}

void console::ConsoleRenderer::loadObject(engine::Object &object) {
    std::cout << "Load object" << std::endl;
}
