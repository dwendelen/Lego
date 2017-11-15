#include "InputManager.hpp"

#include "Math.hpp"

#include <iostream>
#include <SDL_timer.h>
#include <SDL_events.h>

using namespace std;
using namespace OVR;

namespace engine {

    void InputManager::maybeAddController(int i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
        }
    }

    void InputManager::init() {
        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
            maybeAddController(i);
            if (controller) {
                break;
            }
        }
        cout << SDL_GetPerformanceFrequency() << endl;
        cout << SDL_GetPerformanceCounter() << endl;
    }

    float convertinput(float raw) {
        if (abs(raw) < 3000.0f) {
            return 0;
        } else {
            return raw / 32767.0f;
        }
    }

    void InputManager::processInput() {
        inputMapping->preInput();

        SDL_Event windowEvent;
        while (SDL_PollEvent(&windowEvent)) {
            switch (windowEvent.type) {
                case SDL_QUIT:
                    quit();
                    return;
                case SDL_KEYDOWN:
                    handleKey(windowEvent.key.keysym.scancode);
                    break;
                case SDL_CONTROLLERDEVICEADDED:
                    if (!controller) {
                        int index = windowEvent.cdevice.which;
                        maybeAddController(index);
                    }
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    handleButton(windowEvent.cbutton.button);
                    break;
                case SDL_CONTROLLERAXISMOTION: {
                    Uint8 axis = windowEvent.caxis.axis;
                    handleAxis(windowEvent, axis);
                }
                    break;
            }
        }

        if(x != 0.0f) {
            inputMapping->leftJoystickX(x);
        }
        if(y != 0.0f) {
            inputMapping->leftJoystickY(y);
        }
        if(l != 0.0f) {
            inputMapping->leftTrigger(l);
        }
        if(r != 0.0f) {
            inputMapping->rightTrigger(r);
        }

        inputMapping->postInput();
    }

    void InputManager::handleAxis(const SDL_Event &windowEvent, Uint8 axis) {
        switch (axis) {
            case SDL_CONTROLLER_AXIS_LEFTX:
                x = convertinput(windowEvent.caxis.value);
                break;
            case SDL_CONTROLLER_AXIS_LEFTY:
                y = convertinput(windowEvent.caxis.value);
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                l = convertinput(windowEvent.caxis.value);
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                r = convertinput(windowEvent.caxis.value);
                break;
            default:
                break;
        }
    }

    void InputManager::handleButton(Uint8 button) const {
        switch (button) {
            case SDL_CONTROLLER_BUTTON_A:
                inputMapping->onA();
                break;
            case SDL_CONTROLLER_BUTTON_B:
                inputMapping->onB();
                break;
            case SDL_CONTROLLER_BUTTON_X:
                inputMapping->onX();
                break;
            case SDL_CONTROLLER_BUTTON_Y:
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                inputMapping->onUp();
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                inputMapping->onDown();
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                inputMapping->onLeft();
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                inputMapping->onRight();
                break;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                inputMapping->onRightShoulder();
                break;
            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                inputMapping->onLeftShoulder();
                break;
            case SDL_CONTROLLER_BUTTON_START:
                inputMapping->onStartButton();
                break;
            default:
                break;
        }
    }

    void InputManager::handleKey(const SDL_Scancode &key) const {
        switch (key) {
            case SDL_SCANCODE_ESCAPE:
                //quit();
                //return;
            case SDL_SCANCODE_SPACE:
                //game.placeBlock();
                break;
            case SDL_SCANCODE_A:
                //game.moveUnit({-1, 0, 0});
                break;
            case SDL_SCANCODE_D:
                //game.moveUnit({ 1, 0, 0});
                break;
            case SDL_SCANCODE_W:
                //game.moveUnit({0, 0, -1});
                break;
            case SDL_SCANCODE_S:
                //game.moveUnit({0, 0,  1});
                break;
            case SDL_SCANCODE_Q:
                //game.rotate(Quatf(Vector3f(0, 1, 0), M_PI_2));
                break;
            case SDL_SCANCODE_E:
                //game.rotate(Quatf(Vector3f(0, 1, 0), -M_PI_2));
                break;
            case SDL_SCANCODE_TAB:
                break;
            default:
                break;
        }
    }

    void InputManager::quit() {
        shouldQuit = true;
    }

    InputManager::~InputManager() {
        SDL_GameControllerClose(controller);
    }
}
