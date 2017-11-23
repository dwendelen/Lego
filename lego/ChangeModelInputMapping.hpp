//
// Created by xtrit on 22/11/17.
//

#ifndef LEGO_CHANGEMODELINPUTMAPPING_HPP
#define LEGO_CHANGEMODELINPUTMAPPING_HPP

#include "../engine/InputMapping.hpp"

namespace lego {
    class Game;
    class ChangeModelInputMapping: public engine::InputMapping {
    private:
        Game& game;
        float x;
        float y;
        float z;
    public:
        explicit ChangeModelInputMapping(Game &game) : game(game) {}

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

#endif //LEGO_CHANGEMODELINPUTMAPPING_HPP
