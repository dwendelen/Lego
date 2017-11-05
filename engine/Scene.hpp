//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_SCENE_HPP
#define LEGO_SCENE_HPP


#include <vector>
#include "Object.hpp"

namespace engine {
    class Camera;

    template<class ModelData, class ObjectData>
    class Scene {
    private:

    public:
        Camera &camera;
        std::vector<Object<ModelData, ObjectData>> objects;
        Object<ModelData, ObjectData>& controllingObject;

        explicit Scene(Camera &camera, Object<ModelData, ObjectData> & controllingObject) : camera(camera), controllingObject(controllingObject) {}

        Camera &getCamera() const {
            return camera;
        }
    };
}

#endif //LEGO_SCENE_HPP
