//
// Created by xtrit on 06.01.18.
//

#ifndef LEGO_EVENTS_HPP
#define LEGO_EVENTS_HPP

#include "game_engine.hpp"
#include "Math.hpp"

namespace engine {
    enum EventType {
        NEW_OBJECT,
        UPDATE_OBJECT,
        CHANGE_MODEL,
        COMMIT
    };

    class NewObjectEvent {
    public:
        NewObjectEvent(
                Object object,
                ModelData* model,
                OVR::Vector3f position,
                OVR::Quatf orientation,
                OVR::Vector4f color
        ): eventType(NEW_OBJECT),
           object(object),
           model(model),
           position(position),
           orientation(orientation),
           color(color)
        {}
        EventType eventType;
        Object object;
        ModelData* model;
        OVR::Vector3f position;
        OVR::Quatf orientation;
        OVR::Vector4f color;
    };

    class UpdateObjectEvent {
    public:
        UpdateObjectEvent(
                Object object,
                OVR::Vector3f position,
                OVR::Quatf orientation,
                OVR::Vector4f color
        ): eventType(UPDATE_OBJECT),
           object(object),
           position(position),
           orientation(orientation),
           color(color)
        {}
        EventType eventType;
        Object object;
        OVR::Vector3f position;
        OVR::Quatf orientation;
        OVR::Vector4f color;
    };

    class ChangeModelEvent {
    public:
        ChangeModelEvent(Object object, ModelData* model)
        : eventType(CHANGE_MODEL), object(object), model(model)
        {}
        EventType eventType;
        Object object;
        ModelData* model;
    };

    class CommitEvent {
    public:
        CommitEvent(): eventType(COMMIT){};
        EventType eventType;
    };
}

#endif //LEGO_EVENTS_HPP
