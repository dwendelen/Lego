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
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

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

    checkSurface();

    SwapchainCreateInfoKHR swapCreateInfo;
    swapCreateInfo.surface = surface;
    swapCreateInfo.minImageCount = 2;
    swapCreateInfo.imageFormat = Format::eB8G8R8A8Srgb;
    swapCreateInfo.imageColorSpace = ColorSpaceKHR::eSrgbNonlinear;
    swapCreateInfo.imageExtent = Extent2D{800, 600};
    swapCreateInfo.imageArrayLayers = 1;
    swapCreateInfo.imageUsage = ImageUsageFlagBits::eColorAttachment;
    swapCreateInfo.imageSharingMode = SharingMode::eExclusive;
    swapCreateInfo.preTransform = SurfaceTransformFlagBitsKHR::eIdentity;
    swapCreateInfo.compositeAlpha = CompositeAlphaFlagBitsKHR::ePostMultiplied;
    swapCreateInfo.clipped = VK_TRUE;
    swapCreateInfo.presentMode = PresentModeKHR::eFifoRelaxed;

    swapChain = device.createSwapchainKHR(swapCreateInfo);
}

void checkFormat(vector<SurfaceFormatKHR> formats) {
    for(auto format: formats) {
        if(format.format == Format::eB8G8R8A8Srgb && format.colorSpace == ColorSpaceKHR::eSrgbNonlinear) {
            return;
        }
    }
    
    throw runtime_error("Surface does not support format");
}

void checkPresentationMode(vector<PresentModeKHR > presentationModes) {
    for(auto presentationMode: presentationModes) {
        if(presentationMode == PresentModeKHR::eFifo) {
            return;
        }
    }

    throw runtime_error("FIFO not supported");
}

void vulkan::Display::checkSurface() {
    checkFormat(physical.getSurfaceFormatsKHR(surface));
    checkPresentationMode(physical.getSurfacePresentModesKHR(surface));
    SurfaceCapabilitiesKHR capabilities = physical.getSurfaceCapabilitiesKHR(surface);
    cout << "//TODO: check capabilities";
    cout << "//TODO: choose a format and store";
    cout << "//TODO: store width, height and use it";
    cout << capabilities.currentExtent.width
         << capabilities.currentExtent.height
         << endl;
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

