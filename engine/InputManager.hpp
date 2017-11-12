#ifndef LEGO_INPUTMANAGER_HPP
#define LEGO_INPUTMANAGER_HPP

#include <SDL_gamecontroller.h>

namespace engine {
    class Scene;

    class InputManager {
        private:
            Scene& scene;

            float x;
            float y;
			float z;
        
            uint32_t lastTicks;
            bool shouldQuit;
            SDL_GameController *controller;

            void quit();
            void maybeAddController(int index);
        public:
            InputManager(Scene& scene)
                    : x(0)
                    , y(0)
                    , z(0)
                    , lastTicks(0)
                    , scene(scene)
                    , shouldQuit(false)
                    , controller(NULL){}
            void init();
            void processInput();
            bool isShouldQuit() { return shouldQuit; }
            ~InputManager();
    };
}

#endif //LEGO_INPUTMANAGER_HPP