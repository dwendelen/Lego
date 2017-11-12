//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_SCENE_HPP
#define LEGO_SCENE_HPP


#include <vector>
#include "Math.hpp"
#include "Object.hpp"

namespace engine {
    class Camera;

    class Scene {
    private:
        OVR::Vector3f pseudoPosition;
    public:
        Camera &camera;
        std::vector<Object> objects;
        Object& controllingObject;

        explicit Scene(Camera &camera, Object& controllingObject)
                : camera(camera)
                , controllingObject(controllingObject) {}

        Camera &getCamera() const {
            return camera;
        }

        void placeBlock();
        void move(OVR::Vector3f translation);
        void moveUnit(OVR::Vector3i translation);
        void rotate(OVR::Quatf rotation);

        virtual ~Scene();
    };
}

#endif //LEGO_SCENE_HPP
