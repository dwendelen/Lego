//
// Created by xtrit on 4/11/17.
//

#ifndef LEGO_DISPLAY_HPP
#define LEGO_DISPLAY_HPP

#include <SDL_video.h>
#include "vulkanExt.hpp"

namespace vulkan {
    class Display {
        vk::Instance instance;
        vk::Device device;
        vk::Queue queue;

        SDL_Window *window;
        vk::SurfaceKHR surface;
        vk::SwapchainKHR swapChain;
    public:
        Display(vk::Instance instance, vk::Device device, vk::Queue queue)
                : instance(instance)
                , device(device)
                , queue(queue)  {}
        void init();
        std::vector<vk::Image> getImages();

        uint32_t acquireNextFrameBufferId(vk::Semaphore semaphore);
        void display(vk::Semaphore semaphore, uint32_t frameBufferIdx);

        ~Display();
    };
}

#endif //LEGO_DISPLAY_HPP
