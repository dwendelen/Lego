cmake_minimum_required(VERSION 3.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(Lego)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -std=c++11)

find_package(SDL2 REQUIRED)
find_package(Vulkan QUIET)

set(CORE_SRC engine/Engine.cpp engine/Engine.hpp engine/RenderingEngine.hpp engine/Model.hpp engine/Scene.cpp engine/Scene.hpp engine/Camera.cpp engine/Camera.hpp engine/Object.hpp vulkan/ModelData.hpp vulkan/vulkanExt.cpp lego/BrickModel.cpp lego/BrickModel.hpp lego/TriangleModel.hpp)
set(CORE_LIBS ${SDL2_LIBRARY})
set(CORE_INC ${SDL2_INCLUDE_DIR} lib/OVR)

if (Vulkan_FOUND)
    find_package(X11_XCB REQUIRED)
    message("Found Vulkan")

    add_custom_command(
            OUTPUT shaders/vert.spv
            DEPENDS shaders/vert.vert
            COMMAND glslangValidator -V vert.vert
            WORKING_DIRECTORY shaders
            COMMENT "Compile vertex shader"
    )
    add_custom_command(
            OUTPUT shaders/frag.spv
            DEPENDS shaders/frag.frag
            COMMAND glslangValidator -V frag.frag
            WORKING_DIRECTORY shaders
            COMMENT "Compile fragment shader"
    )
    add_custom_target(
            Shaders
            DEPENDS shaders/vert.spv shaders/frag.spv
    )

    include_directories(${CORE_INC} ${Vulkan_INCLUDE_DIR})

    add_executable(LegoV ${CORE_SRC} mainVulkan.cpp vulkan/Renderer.cpp vulkan/Renderer.hpp vulkan/vulkanExt.hpp vulkan/MemoryManager.cpp vulkan/MemoryManager.hpp vulkan/ObjectData.hpp vulkan/Shader.cpp vulkan/Shader.hpp vulkan/Context.cpp vulkan/Context.hpp vulkan/RenderPass.cpp vulkan/RenderPass.hpp vulkan/SimpleRenderPass.cpp vulkan/SimpleRenderPass.hpp vulkan/Display.cpp vulkan/Display.hpp vulkan/TransparentRenderPass.cpp vulkan/TransparentRenderPass.hpp)

    target_link_libraries(LegoV ${CORE_LIBS} ${Vulkan_LIBRARIES} ${X11_XCB_LIBRARIES})
    add_dependencies(LegoV Shaders)


else(Vulkan_FOUND)
    message("No Vulkan")
endif (Vulkan_FOUND)
