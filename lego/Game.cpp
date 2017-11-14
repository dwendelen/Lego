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

void lego::Game::init() {

    std::cout << "Hello, World!" << std::endl;


    brick.init();
    brick2.init();


    auto orientation = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
    std::unique_ptr<engine::Object> object1  = unique_ptr<Object>{new Object {brick, Vector3f{-0.032f,-0.2f,-0.2f}, orientation, Vector3f{1.0f,1.0f,1.0f}}};
    std::unique_ptr<engine::Object> object2  = unique_ptr<Object>{new Object {brick2, Vector3f{0.0f,-0.2f,-0.216f}, orientation, Vector3f{1.0f,0.0f,0.0f}}};
    std::unique_ptr<engine::Object> transparentBrick = unique_ptr <Object> {new Object{brick, Vector3f{0.0f, -0.2f, -0.2f}, orientation, Vector3f{1.0f, 1.0f, 1.0f}}};



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
