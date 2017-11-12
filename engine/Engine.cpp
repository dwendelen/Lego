//
// Created by xtrit on 21/10/17.
//

#include "Engine.hpp"
#include "SDL.h"
#include <string>
#include <stdexcept>

using namespace std;

void engine::Engine::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        throw runtime_error("Could not init SDL: " + string(SDL_GetError()));
    }
}
