//
// Created by xtrit on 14/11/17.
//

#include "ControlBrickInputMapping.hpp"

#include "Game.hpp"

#include <iostream>

using namespace OVR;

void lego::ControlBrickInputMapping::preInput() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

void lego::ControlBrickInputMapping::postInput() {
    float meterPerSec = 0.1f;
    game.setVelocity({x * meterPerSec, y * meterPerSec, z * meterPerSec});
}

void lego::ControlBrickInputMapping::onQuit() {
    game.quit();
}

void lego::ControlBrickInputMapping::onUp() {
    game.moveUnit({0, 0, -1});
}

void lego::ControlBrickInputMapping::onDown() {
    game.moveUnit({0, 0,  1});
}

void lego::ControlBrickInputMapping::onLeft() {
    game.moveUnit({-1, 0, 0});
}

void lego::ControlBrickInputMapping::onRight() {
    game.moveUnit({ 1, 0, 0});
}

void lego::ControlBrickInputMapping::onStartButton() {
    game.quit();
}

void lego::ControlBrickInputMapping::onA() {
    game.placeBlock();
}

void lego::ControlBrickInputMapping::onB() {

}

void lego::ControlBrickInputMapping::onX() {
    game.changeBrick();
}

void lego::ControlBrickInputMapping::onY() {

}

void lego::ControlBrickInputMapping::onLeftShoulder() {
    game.rotate(Quatf(Vector3f(0, 1, 0), M_PI_2));
}

void lego::ControlBrickInputMapping::onRightShoulder() {
    game.rotate(Quatf(Vector3f(0, 1, 0), -M_PI_2));
}

void lego::ControlBrickInputMapping::leftJoystickX(float value) {
    x = value;
}

void lego::ControlBrickInputMapping::leftJoystickY(float value) {
    z = value;
}

void lego::ControlBrickInputMapping::leftTrigger(float value) {
    y = -value;
}

void lego::ControlBrickInputMapping::rightTrigger(float value) {
    y = value;
}


lego::ControlBrickInputMapping::~ControlBrickInputMapping() = default;
