//
// Created by xtrit on 21/10/17.
//

#include "Engine.hpp"

#include <string>

#include <SDL.h>
#include <iostream>
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
        uint32_t newTick = SDL_GetTicks();
        float secondsPassed = (newTick - lastTick) / 1000.0f;
        cout << "FPS " << 1/secondsPassed << endl;
        lastTick = newTick;

        inputManager.processInput();
        game.tick(secondsPassed);
        renderingEngine.render(scene);
    }
}
