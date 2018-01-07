//
// Created by Wendelen Daan on 15/11/2017.
//

#include <iostream>
#include <SDL_timer.h>

#include "ConsoleRenderer.hpp"
#include "../engine/Events.hpp"
#include "../engine/EventBus.hpp"

using namespace std;
using namespace engine;

void console::ConsoleRenderer::init() {
    std::cout << "Init" << std::endl;
}

string printVec3(OVR::Vector3f vec) {
    return "(" +
            to_string(vec.x) + ", " +
            to_string(vec.y) + ", " +
            to_string(vec.z) + ")";
}

string printVec4(OVR::Vector4f vec) {
    return "(" +
           to_string(vec.x) + ", " +
           to_string(vec.y) + ", " +
           to_string(vec.z) + ", " +
           to_string(vec.w) + ")";
}

string printQuatf(OVR::Quatf qaut) {
    return "(" +
           to_string(qaut.x) + ", " +
           to_string(qaut.y) + ", " +
           to_string(qaut.z) + ", " +
           to_string(qaut.w) + ")";
}

void newObjectt(NewObjectEvent *e1) {
    cout << "New object "
         << e1->object << " "
         << e1->model << " "
         << printVec3(e1->position) << " "
         << printQuatf(e1->orientation) << " "
         << printVec4(e1->color)
         << endl;
}

void updateObjectt(const UpdateObjectEvent *e2)  {
    cout << "Update object "
            << e2->object << " "
            << printVec3(e2->position) << " "
            << printQuatf(e2->orientation) << " "
            << printVec4(e2->color)
            << endl;
}

void changeModel(const ChangeModelEvent *e5)  {
    cout << "Change model "
         << e5->object
         << endl;
}


void console::ConsoleRenderer::render() {
    while(true) {
        auto *eventType = eventBus.peak<EventType>();
        switch (*eventType) {
            case NEW_OBJECT:
                newObjectt(eventBus.readNext<NewObjectEvent>());
                break;
            case UPDATE_OBJECT:
                updateObjectt(eventBus.readNext<UpdateObjectEvent>());
                break;
            case CHANGE_MODEL:
                changeModel(eventBus.readNext<ChangeModelEvent>());
                break;
            case COMMIT:
                eventBus.readNext<CommitEvent>();
                eventBus.commitReads();
                std::cout << "Rendered scene" << std::endl;
                SDL_Delay(1000);
                return;
            default:
                throw runtime_error("Unkown event " + to_string(*eventType));
        }
    }
}

void console::ConsoleRenderer::loadModel(engine::ModelData &model) {
    std::cout << "Load model" << std::endl;
}