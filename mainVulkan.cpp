#include <iostream>
#include "engine/Scene.hpp"
#include "engine/Camera.hpp"
#include "vulkan/ModelData.hpp"
#include "vulkan/Renderer.hpp"
#include "vulkan/MemoryManager.hpp"
#include "lego/BrickModel.hpp"
#include "lego/TriangleModel.hpp"
#include "engine/Engine.hpp"
#include "engine/InputManager.hpp"

using namespace engine;
using namespace vulkan;
using namespace OVR;

int main() {
    std::cout << "Hello, World!" << std::endl;


    lego::BrickModel brick{4, 2, 3};
    lego::BrickModel brick2{2, 2, 3};
    brick.init();
    brick2.init();
    lego::TriangleModel triangle;

    auto orientation = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
    engine::Object object1 {brick, Vector3f{-0.032f,-0.2f,-0.2f}, orientation, Vector3f{1.0f,1.0f,1.0f}};
    engine::Object object2 {brick2, Vector3f{0.0f,-0.2f,-0.216f}, orientation, Vector3f{1.0f,0.0f,0.0f}};
    engine::Object transparentBrick {brick, Vector3f{0.0f,-0.2f,-0.2f}, orientation, Vector3f{1.0f,1.0f,1.0f}};

    Camera camera;
    Scene scene(camera, transparentBrick);
    InputManager inputManager {scene};
    Renderer renderer;

    engine::Engine engine{renderer};
    engine.init();
    inputManager.init();

    renderer.init();
    renderer.loadModel(brick);
    renderer.loadModel(brick2);
    renderer.loadModel(triangle);
    renderer.loadObject(object1);
    renderer.loadObject(object2);
    renderer.loadObject(transparentBrick);

    scene.objects.push_back(object1);
    scene.objects.push_back(object2);

    while(!inputManager.isShouldQuit()) {//for(int i = 0; i < 100; i++) {
        inputManager.processInput();
        renderer.render(scene);
    }
}