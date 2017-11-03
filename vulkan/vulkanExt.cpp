#include "vulkanExt.hpp"

// vkCreateDebugReportCallbackEXT self-loader
extern PFN_vkCreateDebugReportCallbackEXT fpCreateDebugReportCallbackEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT_loader(
        VkInstance instance,
        const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugReportCallbackEXT *pCallback
) {
    PFN_vkVoidFunction temp_fp = vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (temp_fp == nullptr) throw "Failed to loadModel vkCreateDebugReportCallbackEXT";

    fpCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>( temp_fp );

    return vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(
        VkInstance instance,
        const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugReportCallbackEXT *pCallback
) {
    return fpCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

PFN_vkCreateDebugReportCallbackEXT fpCreateDebugReportCallbackEXT = &vkCreateDebugReportCallbackEXT_loader;




extern PFN_vkDestroyDebugReportCallbackEXT fpDestroyDebugReportCallbackEXT;

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT_loader(
        VkInstance instance,
        VkDebugReportCallbackEXT callback,
        const VkAllocationCallbacks *pAllocator) {
    PFN_vkVoidFunction temp_fp = vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (temp_fp == nullptr) throw "Failed to loadModel vkDestroyDebugReportCallbackEXT";

    fpDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>( temp_fp );

    return fpDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(
        VkInstance instance,
        VkDebugReportCallbackEXT callback,
        const VkAllocationCallbacks *pAllocator
) {
    return fpDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

PFN_vkDestroyDebugReportCallbackEXT fpDestroyDebugReportCallbackEXT = &vkDestroyDebugReportCallbackEXT_loader;



#ifdef VK_USE_PLATFORM_XCB_KHR
/*
extern PFN_vkCreateXcbSurfaceKHR fpCreateXcbSurfaceKHR;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateXcbSurfaceKHR_loader(
        VkInstance instance,
        const VkXcbSurfaceCreateInfoKHR* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkSurfaceKHR* pSurface
) {
    PFN_vkVoidFunction temp_fp = vkGetInstanceProcAddr(instance, "vkCreateXcbSurfaceKHR");
    if (temp_fp == nullptr) throw "Failed to loadModel vkCreateXcbSurfaceKHR";

    fpCreateXcbSurfaceKHR = reinterpret_cast<PFN_vkCreateXcbSurfaceKHR>( temp_fp );

    return vkCreateXcbSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateXcbSurfaceKHR(
        VkInstance instance,
        const VkXcbSurfaceCreateInfoKHR* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkSurfaceKHR* pSurface
) {
    return fpCreateXcbSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}

PFN_vkCreateXcbSurfaceKHR fpCreateXcbSurfaceKHR = &vkCreateXcbSurfaceKHR_loader;
*/
#endif //VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_XLIB_KHR
/*
extern PFN_vkCreateXlibSurfaceKHR fpCreateXlibSurfaceKHR;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateXlibSurfaceKHR_loader(
        VkInstance instance,
        const VkXlibSurfaceCreateInfoKHR* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkSurfaceKHR* pSurface
) {
    PFN_vkVoidFunction temp_fp = vkGetInstanceProcAddr(instance, "vkCreateXlibSurfaceKHR");
    if (temp_fp == nullptr) throw "Failed to loadModel vkCreateXlibSurfaceKHR";

    fpCreateXlibSurfaceKHR = reinterpret_cast<PFN_vkCreateXlibSurfaceKHR>( temp_fp );

    return vkCreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateXlibSurfaceKHR(
        VkInstance instance,
        const VkXlibSurfaceCreateInfoKHR* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkSurfaceKHR* pSurface
) {
    return fpCreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}

PFN_vkCreateXlibSurfaceKHR fpCreateXlibSurfaceKHR = &vkCreateXlibSurfaceKHR_loader;
*/
#endif //VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR
extern PFN_vkCreateWin32SurfaceKHR fpCreateWin32SurfaceKHR;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateWin32SurfaceKHR_loader(
        VkInstance instance,
        const VkWin32SurfaceCreateInfoKHR* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkSurfaceKHR* pSurface
) {
    PFN_vkVoidFunction temp_fp = vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
    if (temp_fp == nullptr) throw "Failed to loadModel vkCreateWin32SurfaceKHR";

    fpCreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>( temp_fp );

    return vkCreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateWin32SurfaceKHR(
        VkInstance instance,
        const VkWin32SurfaceCreateInfoKHR* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkSurfaceKHR* pSurface
) {
    return fpCreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}

PFN_vkCreateWin32SurfaceKHR fpCreateWin32SurfaceKHR = &vkCreateWin32SurfaceKHR_loader;

#endif //VK_USE_PLATFORM_WIN32_KHR