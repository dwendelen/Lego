//
// Created by xtrit on 21/10/17.
//

#include "Engine.hpp"

#include <string>

#include <SDL.h>
#include <iostream>
#include <chrono>
#include "RenderingEngine.hpp"
#include "InputManager.hpp"
#include "Scene.hpp"
#include "../lego/Game.hpp"

using namespace std;

void engine::Engine::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        throw runtime_error("Could not init SDL: " + string(SDL_GetError()));
    }

    renderingEngine.init();
    inputManager.init();
    scene.init();
    game.init();

    lastTick = SDL_GetTicks();
}

void engine::Engine::run() {
    while(game.isRunning()) {
        auto start = std::chrono::high_resolution_clock::now();

        uint32_t newTick = SDL_GetTicks();
        float secondsPassed = (newTick - lastTick) / 1000.0f;
        cout << "FPS " << 1/secondsPassed << endl;
        lastTick = newTick;

        auto ticks = std::chrono::high_resolution_clock::now();
        inputManager.processInput();
        auto input = std::chrono::high_resolution_clock::now();
        game.tick(secondsPassed);
        auto fysics = std::chrono::high_resolution_clock::now();
        renderingEngine.render(scene);
        auto render = std::chrono::high_resolution_clock::now();

        cout << "calc FPS " << std::chrono::duration<double, milli>(ticks - start).count() << endl;
        cout << "input    " << std::chrono::duration<double, milli>(input - ticks).count() << endl;
        cout << "physics  " << std::chrono::duration<double, milli>(fysics - input).count() << endl;
        cout << "renderin " << std::chrono::duration<double, milli>(render - fysics).count() << endl;
    }
}
