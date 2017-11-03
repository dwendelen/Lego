//
// Created by xtrit on 3/11/17.
//

#ifndef LEGO_TRIANGLEMODEL_HPP
#define LEGO_TRIANGLEMODEL_HPP

#include "../engine/Model.hpp"

namespace lego {
    template<class Data>
    class TriangleModel : public engine::Model<Data> {
    private:
        Data renderData;
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

        Data getRenderData() {
            return renderData;
        }

        void setRenderData(Data renderData) {
            this->renderData = renderData;
        }

        virtual ~TriangleModel() = default;
    };
}

#endif //LEGO_TRIANGLEMODEL_HPP
