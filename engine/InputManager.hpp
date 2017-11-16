#ifndef LEGO_INPUTMANAGER_HPP
#define LEGO_INPUTMANAGER_HPP

#include <SDL_gamecontroller.h>
#include <SDL_scancode.h>
#include <SDL_events.h>
#include "InputMapping.hpp"

namespace engine {

    class InputManager {
        private:
            InputMapping* inputMapping;

            float x;
			float y;
            float r;
            float l;

            SDL_GameController *controller;

            void maybeAddController(int index);
        void handleKey(const SDL_Scancode &key) const;
        void handleButton(Uint8 button) const;
        void handleAxis(const SDL_Event &windowEvent, Uint8 axis);

        public:
            InputManager()
                    : inputMapping(&NullInputMapping::getInstance())
                    , x(0)
                    , y(0)
                    , r(0)
                    , l(0)
                    , controller(NULL){}
            void init();
            void setMapping(InputMapping& inputMapping) {
                this->inputMapping = &inputMapping;
            }
            void processInput();
            ~InputManager();
    };
}

#endif //LEGO_INPUTMANAGER_HPP