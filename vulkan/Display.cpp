//
// Created by xtrit on 4/11/17.
//

#include "Display.hpp"
#include <SDL_syswm.h>

#ifdef VK_USE_PLATFORM_XCB_KHR
#include <X11/Xlib-xcb.h>
#endif


#include <iostream>
using namespace std;
using namespace vk;

void vulkan::Display::init() {
    window = SDL_CreateWindow("Vulkan window", 100, 100, 800, 600, 0);
    if (window == nullptr) {
        throw runtime_error("Could not create window:" + string(SDL_GetError()));
    }

    SDL_SysWMinfo windowInfo{};
    SDL_VERSION(&windowInfo.version);
    SDL_GetWindowWMInfo(window, &windowInfo);
#ifdef VK_USE_PLATFORM_XLIB_KHR
    if (windowInfo.subsystem != SDL_SYSWM_X11) {
        throw runtime_error("Not Xlib");
    }

    XlibSurfaceCreateInfoKHR xlibInfo;
    xlibInfo.dpy = windowInfo.info.x11.display;
    xlibInfo.window = windowInfo.info.x11.window;

    surface = instance.createXlibSurfaceKHR(xlibInfo);

#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
    if(windowInfo.subsystem != SDL_SYSWM_X11) {
        throw runtime_error("Not Xlib");
    }

    XcbSurfaceCreateInfoKHR xcbInfo;
    xcbInfo.connection = XGetXCBConnection(windowInfo.info.x11.display);
    xcbInfo.window = windowInfo.info.x11.window;

    surface = instance.createXcbSurfaceKHR(xcbInfo);

#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
    if(windowInfo.subsystem != SDL_SYSWM_WINDOWS) {
        throw runtime_error("Not Windows");
    }

    Win32SurfaceCreateInfoKHR winInfo;
    winInfo.hwnd = windowInfo.info.win.window;
    winInfo.hinstance = GetModuleHandle(NULL);
    surface = instance.createWin32SurfaceKHR(winInfo);
#endif

    SwapchainCreateInfoKHR swapCreateInfo;
    swapCreateInfo.surface = surface;
    swapCreateInfo.minImageCount = 3;
    swapCreateInfo.imageFormat = Format::eB8G8R8A8Srgb;
    swapCreateInfo.imageColorSpace = ColorSpaceKHR::eSrgbNonlinear;
    swapCreateInfo.imageExtent = Extent2D{800, 600};
    swapCreateInfo.imageArrayLayers = 1;
    swapCreateInfo.imageUsage = ImageUsageFlagBits::eColorAttachment;
    swapCreateInfo.imageSharingMode = SharingMode::eExclusive;
    swapCreateInfo.preTransform = SurfaceTransformFlagBitsKHR::eIdentity;
    swapCreateInfo.compositeAlpha = CompositeAlphaFlagBitsKHR::ePostMultiplied;
    swapCreateInfo.clipped = VK_TRUE;
    swapCreateInfo.presentMode = PresentModeKHR::eMailbox;

    swapChain = device.createSwapchainKHR(swapCreateInfo);
}

std::vector<vk::Image> vulkan::Display::getImages() {
    return device.getSwapchainImagesKHR(swapChain);
}

void vulkan::Display::display(vk::Semaphore semaphore, uint32_t frameBufferIdx) {
    PresentInfoKHR presentInfoKHR;
    presentInfoKHR.waitSemaphoreCount = 1;
    presentInfoKHR.pWaitSemaphores = &semaphore;
    presentInfoKHR.swapchainCount = 1;
    presentInfoKHR.pSwapchains = &swapChain;
    presentInfoKHR.pImageIndices = &frameBufferIdx;

    queue.presentKHR(presentInfoKHR);
}

uint32_t vulkan::Display::acquireNextFrameBufferId(vk::Semaphore semaphore) {
    ResultValue<uint32_t> imageIdxx = device.acquireNextImageKHR(swapChain, 30000000000, semaphore, {});
    return imageIdxx.value;
}

vulkan::Display::~Display() {
    if(swapChain) {
        device.destroySwapchainKHR(swapChain);
    }
    if(surface) {
        instance.destroySurfaceKHR(surface);
    }
    if(window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

