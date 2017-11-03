#include <iostream>
#include "engine/Scene.hpp"
#include "engine/Camera.hpp"
#include "vulkan/ModelData.hpp"
#include "vulkan/Renderer.hpp"
#include "vulkan/MemoryManager.hpp"
#include "lego/BrickModel.hpp"
#include "lego/TriangleModel.hpp"

using namespace engine;
using namespace vulkan;
using namespace OVR;

int main() {
    std::cout << "Hello, World!" << std::endl;

    lego::BrickModel<ModelData> brick{4, 2, 3};
    brick.init();
    lego::TriangleModel<ModelData> triangle;

    auto orientation = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
    engine::Object<ModelData, ObjectData> object {brick, Vector3f{0.0f,0.0f,-0.2f}, orientation, Vector4f{1.0f,1.0f,1.0f, 1.0f}};

    Camera camera;
    Scene<ModelData, ObjectData> scene(camera);
    Renderer renderer;
    renderer.init();
    renderer.loadModel(brick);
    renderer.loadModel(triangle);
    renderer.loadObject(object);

    scene.objects.push_back(object);

    while(true) {
        renderer.render(scene);
    }
}

template class lego::TriangleModel<vulkan::ModelData>;