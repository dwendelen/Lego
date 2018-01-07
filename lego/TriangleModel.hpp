//
// Created by xtrit on 3/11/17.
//

#ifndef LEGO_TRIANGLEMODEL_HPP
#define LEGO_TRIANGLEMODEL_HPP

#include "../engine/Model.hpp"

namespace lego {
    class TriangleModel : public engine::ModelData {
    public:
        virtual std::vector <OVR::Vector3f> getVerticesWithNormal() {
            return std::vector<OVR::Vector3f> {
                    { 0.5f,  0.0f, 0.5f},
                    { 1.0f,  1.0f, 1.0f},
                    { 0.0f, -0.5f, 0.5f},
                    { 1.0f,  1.0f, 1.0f},
                    {-0.5f,  0.0f, 0.5f},
                    { 1.0f,  1.0f, 1.0f},
            };
        };
        virtual std::vector <Vector3ui> getIndices() {
            return std::vector<Vector3ui> {{0, 1, 2}};
        };

        virtual ~TriangleModel() = default;
    };
}

#endif //LEGO_TRIANGLEMODEL_HPP
