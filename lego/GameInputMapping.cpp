//
// Created by xtrit on 14/11/17.
//

#include "GameInputMapping.hpp"

#include "Game.hpp"

#include <iostream>

using namespace OVR;

void lego::GameInputMapping::preInput() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

void lego::GameInputMapping::postInput() {
    float meterPerMilisec = 0.0001f;
    game.setVelocity({x * meterPerMilisec, y * meterPerMilisec, z * meterPerMilisec});
}

void lego::GameInputMapping::onUp() {
    game.moveUnit({0, 0, -1});
}

void lego::GameInputMapping::onDown() {
    game.moveUnit({0, 0,  1});
}

void lego::GameInputMapping::onLeft() {
    game.moveUnit({-1, 0, 0});
}

void lego::GameInputMapping::onRight() {
    game.moveUnit({ 1, 0, 0});
}

void lego::GameInputMapping::onStartButton() {
    game.quit();
}

void lego::GameInputMapping::onA() {
    game.placeBlock();
}

void lego::GameInputMapping::onB() {

}

void lego::GameInputMapping::onX() {

}

void lego::GameInputMapping::onY() {

}

void lego::GameInputMapping::onLeftShoulder() {
    game.rotate(Quatf(Vector3f(0, 1, 0), M_PI_2));
}

void lego::GameInputMapping::onRightShoulder() {
    game.rotate(Quatf(Vector3f(0, 1, 0), -M_PI_2));
}

void lego::GameInputMapping::leftJoystickX(float value) {
    x = value;
}

void lego::GameInputMapping::leftJoystickY(float value) {
    z = value;
}

void lego::GameInputMapping::leftTrigger(float value) {
    y = -value;
}

void lego::GameInputMapping::rightTrigger(float value) {
    y = value;
}


lego::GameInputMapping::~GameInputMapping() = default;
