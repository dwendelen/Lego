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
#include "Game.hpp"

using namespace std;

void engine::Engine::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        throw runtime_error("Could not init SDL: " + string(SDL_GetError()));
    }

    renderingEngine.init();
    inputManager.init();
    game.init();

    lastTick = SDL_GetTicks();
}

void engine::Engine::run() {
    auto game_s = std::chrono::high_resolution_clock::now();
    uint64_t nbOfFrames = 0;

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
        renderingEngine.render();
        auto render = std::chrono::high_resolution_clock::now();

        cout << "calc FPS " << std::chrono::duration<double, milli>(ticks - start).count() << endl;
        cout << "input    " << std::chrono::duration<double, milli>(input - ticks).count() << endl;
        cout << "physics  " << std::chrono::duration<double, milli>(fysics - input).count() << endl;
        cout << "renderin " << std::chrono::duration<double, milli>(render - fysics).count() << endl;
        cout << "FPS 2    " << 1000.0/std::chrono::duration<double, milli>(render - start).count() << endl;
        nbOfFrames++;
    }

    auto game_e = std::chrono::high_resolution_clock::now();
    cout << "Average FPS " << 1000 * nbOfFrames/std::chrono::duration<double, milli>(game_e - game_s).count() << endl;

}
