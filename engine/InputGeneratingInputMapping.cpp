//
// Created by xtrit on 06.01.18.
//

#include "InputGeneratingInputMapping.hpp"

void engine::InputGeneratingInputMapping::preInput() {
    input = Input{};
}

void engine::InputGeneratingInputMapping::postInput() {

}

void engine::InputGeneratingInputMapping::onQuit() {
    quit = true;
}

void engine::InputGeneratingInputMapping::onUp() {
    input.pressedButtons.push_back(SDL_CONTROLLER_BUTTON_DPAD_UP);
}

void engine::InputGeneratingInputMapping::onDown() {
    input.pressedButtons.push_back(SDL_CONTROLLER_BUTTON_DPAD_DOWN);
}

void engine::InputGeneratingInputMapping::onLeft() {
    input.pressedButtons.push_back(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
}

void engine::InputGeneratingInputMapping::onRight() {
    input.pressedButtons.push_back(SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
}

void engine::InputGeneratingInputMapping::onStartButton() {
    input.pressedButtons.push_back(SDL_CONTROLLER_BUTTON_START);
}

void engine::InputGeneratingInputMapping::onA() {
    input.pressedButtons.push_back(SDL_CONTROLLER_BUTTON_A);
}

void engine::InputGeneratingInputMapping::onB() {
    input.pressedButtons.push_back(SDL_CONTROLLER_BUTTON_B);
}

void engine::InputGeneratingInputMapping::onX() {
    input.pressedButtons.push_back(SDL_CONTROLLER_BUTTON_X);
}

void engine::InputGeneratingInputMapping::onY() {
    input.pressedButtons.push_back(SDL_CONTROLLER_BUTTON_Y);
}

void engine::InputGeneratingInputMapping::onLeftShoulder() {
    input.pressedButtons.push_back(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
}

void engine::InputGeneratingInputMapping::onRightShoulder() {
    input.pressedButtons.push_back(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
}

void engine::InputGeneratingInputMapping::leftJoystickX(float value) {
    input.axes.emplace_back(SDL_CONTROLLER_AXIS_LEFTX, value);
}

void engine::InputGeneratingInputMapping::leftJoystickY(float value) {
    input.axes.emplace_back(SDL_CONTROLLER_AXIS_LEFTY, value);
}

void engine::InputGeneratingInputMapping::leftTrigger(float value) {
    input.axes.emplace_back(SDL_CONTROLLER_AXIS_TRIGGERLEFT, value);
}

void engine::InputGeneratingInputMapping::rightTrigger(float value) {
    input.axes.emplace_back(SDL_CONTROLLER_AXIS_TRIGGERRIGHT, value);
}
