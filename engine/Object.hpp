//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_OBJECT_HPP
#define LEGO_OBJECT_HPP

#include "game_engine.hpp"
#include "Math.hpp"


namespace engine {
    class ObjectData {
    public:
        ObjectData(   ModelData* model
                , OVR::Vector3f position
                , OVR::Quatf orientation
                , OVR::Vector4f color
        )
                : model(model)
                , position(position)
                , orientation(orientation)
                , color(color)
        {}

        OVR::Vector3f position;
        OVR::Quatf orientation;
        OVR::Vector4f color;

        ModelData* model;
    };
}

#endif //LEGO_OBJECT_HPP
