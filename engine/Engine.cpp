//
// Created by xtrit on 21/10/17.
//

#include "Engine.hpp"
#include "SDL.h"
#include "../vulkan/ObjectData.hpp"
#include "../vulkan/ModelData.hpp"
#include <string>
#include <stdexcept>

using namespace std;

template<class ModelData, class ObjectData>
void engine::Engine<ModelData, ObjectData>::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        throw runtime_error("Could not init SDL: " + string(SDL_GetError()));
    }
}

template class engine::Engine<vulkan::ModelData, vulkan::ObjectData>;
