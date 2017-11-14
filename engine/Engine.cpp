//
// Created by xtrit on 21/10/17.
//

#include "Engine.hpp"

#include "SDL.h"
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
}

void engine::Engine::run() {
    while(!inputManager.isShouldQuit()) {//for(int i = 0; i < 100; i++) {
        inputManager.processInput();
        renderingEngine.render(scene);
    }
}
