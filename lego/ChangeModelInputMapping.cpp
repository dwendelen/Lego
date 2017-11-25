//
// Created by xtrit on 22/11/17.
//

#include "ChangeModelInputMapping.hpp"

#include "Game.hpp"

void lego::ChangeModelInputMapping::preInput() {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

void lego::ChangeModelInputMapping::postInput() {
    float sizesPerSecond = 6.0f;
    game.setModelVelocity({x * sizesPerSecond, y * sizesPerSecond, z * sizesPerSecond});
}

void lego::ChangeModelInputMapping::onQuit() {
    game.quit();
}

void lego::ChangeModelInputMapping::onUp() {

}

void lego::ChangeModelInputMapping::onDown() {

}

void lego::ChangeModelInputMapping::onLeft() {

}

void lego::ChangeModelInputMapping::onRight() {

}

void lego::ChangeModelInputMapping::onStartButton() {
    game.quit();
}

void lego::ChangeModelInputMapping::onA() {
    game.controlBrick();
}

void lego::ChangeModelInputMapping::onB() {

}

void lego::ChangeModelInputMapping::onX() {
    game.controlBrick();
}

void lego::ChangeModelInputMapping::onY() {

}

void lego::ChangeModelInputMapping::onLeftShoulder() {
    game.changeColor(-1);
}

void lego::ChangeModelInputMapping::onRightShoulder() {
    game.changeColor(1);
}

void lego::ChangeModelInputMapping::leftJoystickX(float value) {
    x = value;
}

void lego::ChangeModelInputMapping::leftJoystickY(float value) {
    z = -value;
}

void lego::ChangeModelInputMapping::leftTrigger(float value) {
    y = -value;
}

void lego::ChangeModelInputMapping::rightTrigger(float value) {
    y = value;
}

