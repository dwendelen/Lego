//
// Created by xtrit on 14/11/17.
//

#include <iostream>
#include "Game.hpp"
#include "BrickModel.hpp"

#include "../engine/Object.hpp"
#include "../engine/RenderingEngine.hpp"
#include "../engine/Scene.hpp"
#include "GridUtil.hpp"

using namespace OVR;
using namespace std;
using namespace engine;


const vector<Vector3f> colors = {
        Vector3f(1.0f, 1.0f, 1.0f),
        Vector3f(1.0f, 0.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 0.0f, 1.0f)
};

void lego::Game::init() {
    std::cout << "Hello, World!" << std::endl;

    BrickModel* brick = brickCache.getBrick(4, 2, 3);
    BrickModel* brick2 = brickCache.getBrick(4, 2, 3);

    auto orientation = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
    const Vector3f &position = Vector3f{0.0f, -0.2f, -0.2f};
    pseudoPosition = position;
    currentBrickDimensions = {4,2,3};
    brickDimensions = {4,3,2};
    modelVelocity = {0,0,0};
    std::unique_ptr<engine::Object> transparentBrick = unique_ptr<Object> {new Object{brick, position, orientation, colors[colorIndex]}};
    renderingEngine.loadObject(*transparentBrick);
    scene.controllingObject = std::move(transparentBrick);

    inputManager.setMapping(*controllBrickInputMapping);

    for(int i = 0; i < 250; i++) {
        placeBlock();
    }
}

void lego::Game::setVelocity(OVR::Vector3f velocity) {
    this->velocity = velocity;
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
                 lego::GridUtil::SIZE_GRID * translation.x,
                 lego::GridUtil::HEIGHT_GRID * translation.y,
                 lego::GridUtil::SIZE_GRID * translation.z
         });
}

void lego::Game::tick(float secondsPassed) {
    this->move(velocity * secondsPassed);
    
    brickDimensions += modelVelocity * secondsPassed;
    int width = lego::GridUtil::snapSize(brickDimensions.x);
    int height = lego::GridUtil::snapSize(brickDimensions.y);
    int depth = lego::GridUtil::snapSize(brickDimensions.z);

    currentBrickDimensions.width = width;
    currentBrickDimensions.height = height;
    currentBrickDimensions.depth = depth;

    scene.controllingObject->model = brickCache.getBrick(width,depth, height);
}

void lego::Game::move(Vector3f translation) {
    pseudoPosition += translation;

    Vector3f position;
    position.x = lego::GridUtil::snapStub(pseudoPosition.x);
    position.y = lego::GridUtil::snapHeight(pseudoPosition.y);
    position.z = lego::GridUtil::snapStub(pseudoPosition.z);

    scene.controllingObject->position = position;
}

void lego::Game::placeBlock() {
    unique_ptr<Object> newObject = unique_ptr<Object>(new Object(*scene.controllingObject));
    renderingEngine.loadObject(*newObject);
    scene.placedObjects.push_back(std::move(newObject));
}

void lego::Game::changeBrick() {
    brickDimensions.x = currentBrickDimensions.width;
    brickDimensions.y = currentBrickDimensions.height;
    brickDimensions.z = currentBrickDimensions.depth;

    inputManager.setMapping(*changeModelInputMapping);
}

void lego::Game::controlBrick() {
    inputManager.setMapping(*controllBrickInputMapping);
}

void lego::Game::setModelVelocity(OVR::Vector3f velocity) {
    this->modelVelocity = velocity;
}

void lego::Game::changeColor(int delta) {
    int size = colors.size();
    colorIndex = (colorIndex + delta + size) % size;
    this->scene.controllingObject->color = colors[colorIndex];
}
