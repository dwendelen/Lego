//
// Created by xtrit on 22/10/17.
//

#ifndef LEGO_BRICKMODEL_HPP
#define LEGO_BRICKMODEL_HPP

#include "../engine/Model.hpp"

namespace lego {
    template<class RenderData>
    class BrickModel : public engine::Model<RenderData> {
        int nbOfStubsX;
        int nbOfStubsZ;
        int height;

        std::vector <OVR::Vector3f> vertices;
        std::vector <Vector3ui> indices;

        RenderData renderData;
    public:
        BrickModel(int nbOfStubsX, int nbofStubsZ, int height)
                : nbOfStubsX(nbOfStubsX)
                , nbOfStubsZ(nbofStubsZ)
                , height(height) {}

        void init();

        virtual std::vector <OVR::Vector3f> getVerticesWithNormal() { return vertices; }

        virtual std::vector <Vector3ui> getIndices() { return indices; }

        virtual RenderData getRenderData() { return renderData; }
        virtual void setRenderData(RenderData renderData) { this->renderData = renderData; }

        virtual ~BrickModel() = default;
    };
}

#endif //LEGO_BRICKMODEL_HPP
