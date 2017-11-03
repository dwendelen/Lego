//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_CAMERA_HPP
#define LEGO_CAMERA_HPP


#include "Extras/OVR_Math.h"

namespace engine {
    class Camera {
    public:
        OVR::Vector3f getPosition() {
            return {0, 0, 0};
        }

        OVR::Vector3f getLookAt() {
            return {0, 0, -1};
        }

        OVR::Vector3f getUp() {
            return {0, 1, 0};
        }

        OVR::Vector2f getFovWidthInRad() {
            return {0.8, 0.6};
        }
    };
}

#endif //LEGO_CAMERA_HPP
