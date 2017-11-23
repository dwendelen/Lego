//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_OBJECT_HPP
#define LEGO_OBJECT_HPP

#include "Math.hpp"

namespace engine {
    class Model;
    class Object {
    public:
        Object(   Model* model
                , OVR::Vector3f position
                , OVR::Quatf orientation
                , OVR::Vector3f color
        )
                : model(model)
                , position(position)
                , orientation(orientation)
                , color(color)
        {}

        OVR::Vector3f position;
        OVR::Quatf orientation;
        OVR::Vector3f color;

        void* renderData = nullptr;
        Model* model;
    };
}

#endif //LEGO_OBJECT_HPP
