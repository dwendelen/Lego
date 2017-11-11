#ifndef LEGO_VULKANEXT_HPP
#define LEGO_VULKANEXT_HPP

#include <SDL_platform.h>

#ifdef __WINDOWS__
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif
#ifdef __LINUX__
#define VK_USE_PLATFORM_XCB_KHR 1
//#define VK_USE_PLATFORM_XLIB_KHR 1
#endif

#include <vulkan/vulkan.hpp>


#endif //LEGO_VULKANEXT_HPP