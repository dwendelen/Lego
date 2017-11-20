//
// Created by xtrit on 20/11/17.
//

#include "BrickCache.hpp"
#include "BrickModel.hpp"
#include "../engine/RenderingEngine.hpp"

lego::BrickCache::BrickCache(engine::RenderingEngine& renderingEngine):
        bricks(32, BrickDimension::hash, BrickDimension::equals),
        renderingEngine(renderingEngine)
{}

lego::BrickModel &lego::BrickCache::getBrick(uint32_t width, uint32_t depth, uint32_t height) {
    BrickDimension dim{};
    dim.height = height;
    dim.depth = depth;
    dim.width = width;

    auto find = bricks.find(dim);
    if (find == bricks.end()) {
        std::unique_ptr<BrickModel> newModel = std::unique_ptr<BrickModel>(new BrickModel(width, depth, height));
        BrickModel& modelRef = *newModel;
        bricks.emplace(dim, std::move(newModel));

        modelRef.init();
        renderingEngine.loadModel(modelRef);

        return modelRef;
    }

    return *find->second;
}

size_t lego::BrickDimension::hash(lego::BrickDimension dim) {
    size_t hash = dim.height;
    hash = 31 * hash + dim.depth;
    hash = 31 * hash + dim.width;

    return hash;
}

bool lego::BrickDimension::equals(lego::BrickDimension dim1, lego::BrickDimension dim2) {
    return dim1.width == dim2.width
           && dim1.depth == dim2.depth
           && dim1.height == dim2.height;

}
