//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_OBJECT_HPP
#define LEGO_OBJECT_HPP

#include <Extras/OVR_Math.h>

namespace engine {
    template<class T> class Model;

    template<class ModelData, class ObjectData>
    class Object {
    private:
        Model<ModelData> &model;
    public:
        Object(   Model<ModelData> &model
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
        ObjectData objectData;

        Model<ModelData> &getModel() const {
            return model;
        }
    };
}

#endif //LEGO_OBJECT_HPP
