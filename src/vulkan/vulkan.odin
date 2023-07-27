// For now, the official bindings works. I will use this if the binding doesn't work.
package vk

import "core:c"

// Get the dynamic library from
// https://sdk.lunarg.com/sdk/download/1.3.250.1/windows/VulkanRT-1.3.250.1-Components.zip
// You need the pdb file to profile in the Intel vtune or inspector.
// So keep the pdb.
// Get the definition symbols from
// https://sdk.lunarg.com/sdk/download/1.3.250.1/windows/VulkanSDK-1.3.250.1-Installer.exe
// C:\VulkanSDK\1.3.250.1\Lib
// If it doesn't run,
// please check the if compiler can reach vulkan-1.lib
when ODIN_OS == .Windows {
	foreign import lib "vulkan-1.lib"
}
// GLFW
GLFW_NO_API :: 0
GLFW_RESIZABLE :: 0x00020003
GLFW_FALSE :: 0

// Vulkan
MAX_EXTENSION_NAME_SIZE       :: 256

Result :: enum c.int {
	VK_SUCCESS = 0,
	VK_NOT_READY = 1,
	VK_TIMEOUT = 2,
	VK_EVENT_SET = 3,
	VK_EVENT_RESET = 4,
	VK_INCOMPLETE = 5,
	VK_ERROR_OUT_OF_HOST_MEMORY = -1,
	VK_ERROR_OUT_OF_DEVICE_MEMORY = -2,
	VK_ERROR_INITIALIZATION_FAILED = -3,
	VK_ERROR_DEVICE_LOST = -4,
	VK_ERROR_MEMORY_MAP_FAILED = -5,
	VK_ERROR_LAYER_NOT_PRESENT = -6,
	VK_ERROR_EXTENSION_NOT_PRESENT = -7,
	VK_ERROR_FEATURE_NOT_PRESENT = -8,
	VK_ERROR_INCOMPATIBLE_DRIVER = -9,
	VK_ERROR_TOO_MANY_OBJECTS = -10,
	VK_ERROR_FORMAT_NOT_SUPPORTED = -11,
	VK_ERROR_FRAGMENTED_POOL = -12,
	VK_ERROR_UNKNOWN = -13,
	VK_ERROR_OUT_OF_POOL_MEMORY = -1000069000,
	VK_ERROR_INVALID_EXTERNAL_HANDLE = -1000072003,
	VK_ERROR_FRAGMENTATION = -1000161000,
	VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS = -1000257000,
	VK_PIPELINE_COMPILE_REQUIRED = 1000297000,
	VK_ERROR_SURFACE_LOST_KHR = -1000000000,
	VK_ERROR_NATIVE_WINDOW_IN_USE_KHR = -1000000001,
	VK_SUBOPTIMAL_KHR = 1000001003,
	VK_ERROR_OUT_OF_DATE_KHR = -1000001004,
	VK_ERROR_INCOMPATIBLE_DISPLAY_KHR = -1000003001,
	VK_ERROR_VALIDATION_FAILED_EXT = -1000011001,
	VK_ERROR_INVALID_SHADER_NV = -1000012000,
	VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR = -1000023000,
	VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR = -1000023001,
	VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR = -1000023002,
	VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR = -1000023003,
	VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR = -1000023004,
	VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR = -1000023005,
	VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT = -1000158000,
	VK_ERROR_NOT_PERMITTED_KHR = -1000174001,
	VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT = -1000255000,
	VK_THREAD_IDLE_KHR = 1000268000,
	VK_THREAD_DONE_KHR = 1000268001,
	VK_OPERATION_DEFERRED_KHR = 1000268002,
	VK_OPERATION_NOT_DEFERRED_KHR = 1000268003
}

ExtensionProperties :: struct {
	extensionName: [MAX_EXTENSION_NAME_SIZE]byte,
	specVersion:   u32,
}

@(default_calling_convention="c", link_prefix="vk")
foreign lib {
	EnumerateInstanceExtensionProperties :: proc(pLayerName: cstring, pPropertyCount: ^u32, pProperties: [^]ExtensionProperties) -> Result ---
}
