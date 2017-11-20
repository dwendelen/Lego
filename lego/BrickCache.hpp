//
// Created by xtrit on 20/11/17.
//

#ifndef LEGO_BRICKCACHE_HPP
#define LEGO_BRICKCACHE_HPP


#include <cstdint>
#include <functional>
#include <unordered_map>
#include <memory>

namespace engine {
    class RenderingEngine;
}

namespace lego {
    class BrickModel;

    class BrickDimension {
    public:
        uint32_t width;
        uint32_t depth;
        uint32_t height;

        static size_t hash(BrickDimension dim);
        static bool equals(BrickDimension dim1, BrickDimension dim2);
    };


    class BrickCache {
    public:
        explicit BrickCache(engine::RenderingEngine& renderingEngine);
        BrickModel &getBrick(uint32_t width, uint32_t depth, uint32_t height);

    private:
        engine::RenderingEngine& renderingEngine;

        std::unordered_map<BrickDimension, std::unique_ptr<BrickModel>, decltype(&BrickDimension::hash), decltype(&BrickDimension::equals)> bricks;
    };
}

#endif //LEGO_BRICKCACHE_HPP
