//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_OBJECT_HPP
#define LEGO_OBJECT_HPP

#include "Math.hpp"

namespace engine {
    class Model;
    class Object {
    private:
        Model& model;
    public:
        Object(   Model& model
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

        void* renderData;

        Model& getModel() const {
            return model;
        }
    };

    class MovingObject: public Object {
    public:
        OVR::Vector3f velocity;

        MovingObject(
                Model &model,
                OVR::Vector3f position,
                OVR::Quatf orientation,
                OVR::Vector3f color
        ) : Object(model, position, orientation, color)
        {}

        void move(float seconds) {
            position += velocity * seconds;
        }
    };
}

#endif //LEGO_OBJECT_HPP
