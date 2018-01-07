//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_MODEL_HPP
#define LEGO_MODEL_HPP

#include "Math.hpp"
#include <vector>

typedef OVR::Vector3<uint32_t> Vector3ui;

namespace engine {
    class ModelData {

    public:
        virtual std::vector <OVR::Vector3f> getVerticesWithNormal()=0;
        virtual std::vector <Vector3ui> getIndices()=0;
        virtual uint32_t getNbOfIndices() = 0;

        void * renderData;

        virtual ~ModelData() = default;
    };
}

#endif //LEGO_MODEL_HPP
