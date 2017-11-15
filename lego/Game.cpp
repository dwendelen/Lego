//
// Created by xtrit on 14/11/17.
//

#include <iostream>
#include "Game.hpp"
#include "BrickModel.hpp"

#include "../engine/Object.hpp"
#include "../engine/RenderingEngine.hpp"
#include "../engine/Scene.hpp"

using namespace OVR;
using namespace std;
using namespace engine;

const float GRID_X = 0.0080f;
const float GRID_Y = 0.0032f;
const float GRID_Z = 0.0080f;

const float HALF_GRID_X = GRID_X / 2;
const float HALF_GRID_Y = GRID_Y / 2;
const float HALF_GRID_Z = GRID_Z / 2;


const vector<Vector3f> colors = {
        Vector3f(1.0f, 1.0f, 1.0f),
        Vector3f(1.0f, 0.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 0.0f, 1.0f)
};

void lego::Game::init() {

    std::cout << "Hello, World!" << std::endl;


    brick.init();
    brick2.init();


    auto orientation = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
    std::unique_ptr<engine::Object> object1  = unique_ptr<Object>{new Object {brick, Vector3f{-0.032f,-0.2f,-0.2f}, orientation, Vector3f{1.0f,1.0f,1.0f}}};
    std::unique_ptr<engine::Object> object2  = unique_ptr<Object>{new Object {brick2, Vector3f{0.0f,-0.2f,-0.216f}, orientation, Vector3f{1.0f,0.0f,0.0f}}};
    std::unique_ptr<engine::MovingObject> transparentBrick = unique_ptr<MovingObject> {new MovingObject{brick, Vector3f{0.0f, -0.2f, -0.2f}, orientation, Vector3f{1.0f, 1.0f, 1.0f}}};



    renderingEngine.loadModel(brick);
    renderingEngine.loadModel(brick2);
    renderingEngine.loadObject(*object1);
    renderingEngine.loadObject(*object2);
    renderingEngine.loadObject(*transparentBrick);

    scene.placedObjects.push_back(std::move(object1));
    scene.placedObjects.push_back(std::move(object2));
    scene.controllingObject = std::move(transparentBrick);

    inputManager.setMapping(*inputMapping);

}

void lego::Game::setVelocity(OVR::Vector3f velocity) {
    scene.controllingObject->velocity = velocity;

}



void lego::Game::rotate(Quatf rotation)
{
    Quatf orientation = rotation * scene.controllingObject->orientation;
    if(!orientation.IsNormalized()) {
        orientation.Normalize();
    }

    scene.controllingObject->orientation = orientation;
}


void lego::Game::moveUnit(Vector3i translation) {
    move({
                 GRID_X * translation.x,
                 GRID_Y * translation.y,
                 GRID_Z * translation.z
         });
}

float calculateMovement(float& value, float delta, float halfGrid, float grid) {
    value += delta;
    if(value > 0) {
        if(value <= halfGrid) {
            return 0.0f;
        } else {
            value = value - grid;
            return grid;
        }
    } else {
        if(value >= -halfGrid) {
            return 0.0f;
        } else {
            value = value + grid;
            return -grid;
        }
    }
}
void lego::Game::move(Vector3f translation) {
    Vector3f movement;
    movement.x = calculateMovement(pseudoPosition.x, translation.x, HALF_GRID_X, GRID_X);
    movement.y = calculateMovement(pseudoPosition.y, translation.y, HALF_GRID_Y, GRID_Y);
    movement.z = calculateMovement(pseudoPosition.z, translation.z, HALF_GRID_Z, GRID_Z);

    controllingObject->position += movement;
}



void lego::Game::placeBlock() {
    //placedObjects.push_back(controllingObject);
}