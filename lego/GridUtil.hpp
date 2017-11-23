//
// Created by xtrit on 22/11/17.
//

#ifndef LEGO_GRIDUTIL_HPP
#define LEGO_GRIDUTIL_HPP

#include "../engine/Math.hpp"

namespace lego {
    class GridUtil {
    public:
        static constexpr float SIZE_GRID = 0.0080f;
        static constexpr float HEIGHT_GRID =  0.0032f;

        static constexpr float HALF_SIZE_GRID = SIZE_GRID / 2;
        static constexpr float HALF_HEIGHT_GRID = HEIGHT_GRID / 2;

        static float snapStub(float value) {
            return SIZE_GRID * roundf(value / SIZE_GRID);
        }

        static float snapHeight(float value) {
            return HEIGHT_GRID * roundf(value / HEIGHT_GRID);
        }

        static int snapSize(float value) {
            int rounded = static_cast<int>(round(value));
            if(rounded < 1) {
                return 1;
            }
            return rounded;
        }
    };
}

#endif //LEGO_GRIDUTIL_HPP
