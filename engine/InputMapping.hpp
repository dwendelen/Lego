//
// Created by xtrit on 14/11/17.
//

#ifndef LEGO_INPUTMAPPING_HPP
#define LEGO_INPUTMAPPING_HPP

namespace engine {
    class InputMapping {
    public:
        virtual void preInput() = 0;
        virtual void postInput() = 0;
        virtual void onQuit() = 0;

        virtual void onUp() = 0;

        virtual void onDown() = 0;

        virtual void onLeft() = 0;

        virtual void onRight() = 0;

        virtual void onStartButton() = 0;

        virtual void onA() = 0;

        virtual void onB() = 0;

        virtual void onX() = 0;

        virtual void onY() = 0;

        virtual void onLeftShoulder() = 0;

        virtual void onRightShoulder() = 0;

        virtual void leftJoystickX(float value) = 0;

        virtual void leftJoystickY(float value) = 0;

        virtual void leftTrigger(float value) = 0;

        virtual void rightTrigger(float value) = 0;

        virtual ~InputMapping() = default;
    };

    class NullInputMapping: public InputMapping {
    public:
        static NullInputMapping& getInstance()
        {
            static NullInputMapping instance;
            return instance;
        }

        void preInput() override {}
        void postInput() override {}
        void onQuit() override {}

        void onUp() override {}
        void onDown() override {}
        void onLeft() override {}
        void onRight() override {}
        void onStartButton() override {}
        void onA() override {}
        void onB() override {}
        void onX() override {}
        void onY() override {}
        void onLeftShoulder() override {}
        void onRightShoulder() override {}

        void leftJoystickX(float value) override {}
        void leftJoystickY(float value) override {}
        void leftTrigger(float value) override {}
        void rightTrigger(float value) override {}

        ~NullInputMapping() override = default;
    };
}

#endif //LEGO_INPUTMAPPING_HPP
