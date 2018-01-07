//
// Created by xtrit on 06.01.18.
//

#ifndef LEGO_INPUTGENERATINGINPUTMAPPING_HPP
#define LEGO_INPUTGENERATINGINPUTMAPPING_HPP


#include "InputMapping.hpp"
#include "Input.hpp"

namespace engine {
    class InputGeneratingInputMapping : public InputMapping {
    public:
        Input input;
        bool quit = false;

        void preInput() override;

        void postInput() override;

        void onQuit() override;

        void onUp() override;

        void onDown() override;

        void onLeft() override;

        void onRight() override;

        void onStartButton() override;

        void onA() override;

        void onB() override;

        void onX() override;

        void onY() override;

        void onLeftShoulder() override;

        void onRightShoulder() override;

        void leftJoystickX(float value) override;

        void leftJoystickY(float value) override;

        void leftTrigger(float value) override;

        void rightTrigger(float value) override;
    };
}

#endif //LEGO_INPUTGENERATINGINPUTMAPPING_HPP
