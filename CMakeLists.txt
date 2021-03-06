cmake_minimum_required(VERSION 3.5)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(Lego)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DLEGO_DEBUG")

find_package(SDL2 REQUIRED)
find_package(Vulkan QUIET)
find_package(Lua REQUIRED)

set(ENGINE_SRC
        engine/Math.hpp
        engine/Engine.cpp engine/Engine.hpp
        engine/RenderingEngine.hpp
        engine/Model.hpp
        engine/Object.hpp
        engine/Camera.cpp engine/Camera.hpp
        engine/InputManager.cpp engine/InputManager.hpp
        engine/InputMapping.hpp
        engine/EventBus.cpp engine/EventBus.hpp

        engine/Game.hpp
        engine/Input.hpp
        engine/InputGeneratingInputMapping.cpp engine/InputGeneratingInputMapping.hpp

        lua/LuaGame.cpp lua/LuaGame.hpp
        lua/ScriptEngine.cpp lua/ScriptEngine.hpp
        engine/game_engine.hpp)
set(ENGINE_LIBS
        ${SDL2_LIBRARY}
        ${LUA_LIBRARIES}
)
set(ENGINE_INC
        ${SDL2_INCLUDE_DIR}
        lib/OVR
        ${LUA_INCLUDE_DIR}
)

include_directories(${ENGINE_INC})

set(LEGO_SRC
        lego/BrickModel.cpp lego/BrickModel.hpp
        lego/TriangleModel.hpp
        lego/BrickCache.cpp lego/BrickCache.hpp
        lego/GridUtil.hpp)

add_custom_target(Scripts)
add_custom_command(
        TARGET Scripts POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/scripts
        ${CMAKE_CURRENT_BINARY_DIR}/scripts
)

if (Vulkan_FOUND)
    find_package(X11_XCB REQUIRED)
    message("Found Vulkan")

    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/vert.spv
            DEPENDS shaders/vert.vert
            COMMAND glslangValidator -V vert.vert -o ${CMAKE_CURRENT_BINARY_DIR}/shaders/vert.spv
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
            COMMENT "Compile vertex shader"
    )
    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/frag.spv
            DEPENDS shaders/frag.frag
            COMMAND glslangValidator -V frag.frag -o ${CMAKE_CURRENT_BINARY_DIR}/shaders/frag.spv
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
            COMMENT "Compile fragment shader"
    )
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders)
    add_custom_target(
            Shaders
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/shaders/vert.spv ${CMAKE_CURRENT_BINARY_DIR}/shaders/frag.spv
    )

    set(VULKAN_SRC
            vulkan/vulkanExt.hpp vulkan/vulkanExt.cpp
            vulkan/ModelData.hpp
            vulkan/Renderer.cpp vulkan/Renderer.hpp
            vulkan/MemoryManager.cpp vulkan/MemoryManager.hpp
            vulkan/Shader.cpp vulkan/Shader.hpp
            vulkan/Context.cpp vulkan/Context.hpp
            vulkan/Display.cpp vulkan/Display.hpp
            vulkan/RenderPass.cpp vulkan/RenderPass.hpp
            vulkan/Frame.hpp vulkan/Frame.cpp engine/Events.hpp vulkan/vulkanRenderer.hpp)

    include_directories(${Vulkan_INCLUDE_DIR})

    add_executable(LegoV
            ${ENGINE_SRC}
            ${VULKAN_SRC}
            ${LEGO_SRC}
            mainVulkan.cpp
    )

    target_link_libraries(LegoV ${ENGINE_LIBS} ${Vulkan_LIBRARIES} ${X11_XCB_LIBRARIES})
    add_dependencies(LegoV Shaders Scripts)


else (Vulkan_FOUND)
    message("No Vulkan")
endif (Vulkan_FOUND)

add_executable(LegoC
        ${ENGINE_SRC}
        ${LEGO_SRC}
        console/ConsoleRenderer.cpp console/ConsoleRenderer.hpp
        mainConsole.cpp
)
target_link_libraries(LegoC ${ENGINE_LIBS})
add_dependencies(LegoC Scripts)

find_package(GTest REQUIRED)
if(GTEST_FOUND)
    message("Found googletest")

    add_executable(EventBusTest
            engine/EventBusTest.cpp
            engine/EventBus.cpp engine/EventBus.hpp
    )
    target_link_libraries(EventBusTest
            ${GTEST_BOTH_LIBRARIES}
            ${SDL2_LIBRARY}
    )
else(GTEST_FOUND)
    message("No gtest found")
endif(GTEST_FOUND)