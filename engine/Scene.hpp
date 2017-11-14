//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_SCENE_HPP
#define LEGO_SCENE_HPP


#include <vector>
#include <bits/unique_ptr.h>
#include "Math.hpp"
#include "Object.hpp"

namespace engine {
    class Camera;

    class Scene {
    private:
        OVR::Vector3f pseudoPosition;
    public:
        std::vector<std::unique_ptr<Object>> placedObjects;
        std::unique_ptr<Object> controllingObject;

        explicit Scene() = default;

        void init();
        void placeBlock();
        void move(OVR::Vector3f translation);
        void moveUnit(OVR::Vector3i translation);
        void rotate(OVR::Quatf rotation);

        virtual ~Scene();
    };
}

#endif //LEGO_SCENE_HPP
