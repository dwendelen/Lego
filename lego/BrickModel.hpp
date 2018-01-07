//
// Created by xtrit on 22/10/17.
//

#ifndef LEGO_BRICKMODEL_HPP
#define LEGO_BRICKMODEL_HPP

#include <iostream>
#include "../engine/Model.hpp"

namespace lego {
    class BrickModel : public engine::ModelData {
        int nbOfStubsX;
        int nbOfStubsZ;
        int height;

        std::vector <OVR::Vector3f> vertices;
        std::vector <Vector3ui> indices;
    public:
        BrickModel(int nbOfStubsX, int nbofStubsZ, int height)
                : nbOfStubsX(nbOfStubsX)
                , nbOfStubsZ(nbofStubsZ)
                , height(height) {}

        void init();

        virtual std::vector <OVR::Vector3f> getVerticesWithNormal() { return vertices; }

        virtual std::vector <Vector3ui> getIndices() { return indices; }

        virtual uint32_t getNbOfIndices() {
            return indices.size() * 3;
        }

        virtual ~BrickModel() {
            std::cerr << "Model destroyed!" << std::endl;
        };
    };
}

#endif //LEGO_BRICKMODEL_HPP
