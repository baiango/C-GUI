#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>
using std::vector;


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
) {
	if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT < messageSeverity) {
		// Message is important enough to show
		std::cerr << "Validation layer: " << pCallbackData->pMessage << "\n";
	}

	return VK_FALSE;
}

int main() {
	using std::cout;
	std::cout << "Hello World!\n";
	glfwInit();
	// GLFW_NO_API means no OpenGL api. That leaves Vulkan api to you only.
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	uint32_t WIDTH = 800, HEIGHT = 600;
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Vulkan!", nullptr, nullptr);


	// 2a: Validation layers
	vector<const char*> validation_layers = { "VK_LAYER_KHRONOS_validation" };
	bool is_validation_layer_supported = false;
#ifdef NDEBUG
	bool is_validation_layers_on = false;
#else
	bool is_validation_layers_on = true;
#endif
	// checkValidationLayerSupport()
	{
		uint32_t layer_cout;
		vkEnumerateInstanceLayerProperties(&layer_cout, nullptr);

		vector<VkLayerProperties> available_layers(layer_cout);
		vkEnumerateInstanceLayerProperties(&layer_cout, available_layers.data());

		for (const char* layer_name : validation_layers) {
			for (const auto& layer_properties : available_layers) {
				if (strcmp(layer_name, layer_properties.layerName) == 0) {
					is_validation_layer_supported = true;
					goto checked_validation_layer_support;
				}
			}
		}
	}
checked_validation_layer_support:
	if (is_validation_layers_on && is_validation_layer_supported) {
		cout << "Validation layers is available!\n";
	}
	else {
		cout << "Validation layers requested, but not available!\n";
	}


	// 1: Instance
	VkInstance instance;
	VkApplicationInfo app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Hello Triangle";
	app_info.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
	app_info.pEngineName = "No engine";
	app_info.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo create_info{};
	uint32_t glfw_ext_cout = 0;
	const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_cout);
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

	create_info.enabledExtensionCount = glfw_ext_cout;
	create_info.ppEnabledExtensionNames = glfw_extensions;

	create_info.enabledLayerCount = 0;


	// 2b: Validation layers
	VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
	if (is_validation_layers_on) {
		create_info.enabledLayerCount = (uint32_t)(validation_layers.size());
		create_info.ppEnabledLayerNames = validation_layers.data();

		debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debug_create_info.pfnUserCallback = debugCallback;
		create_info.pNext = &debug_create_info;
	} else {
		create_info.enabledLayerCount = 0;
	}

	// getRequiredExtensions()
	vector<const char*> extensions;
{
	uint32_t glfw_ext_cout = 0;
	const char** glfw_exts;
	glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_cout);

	vector<const char*> extensions(glfw_exts, glfw_exts + glfw_ext_cout);

	if (is_validation_layers_on) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
}

	create_info.enabledExtensionCount = (uint32_t)(extensions.size());
	create_info.ppEnabledExtensionNames = extensions.data();

	// setupDebugMessenger()
	VkDebugUtilsMessengerEXT debug_messenger;
	if (!is_validation_layers_on) { goto skip_debug_messenger; }

	debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debug_create_info.pfnUserCallback = debugCallback;

	//VkDebugUtilsMessengerCreateInfoEXT messenger_create_info;
	//auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	//if (nullptr == func) {
	//	throw std::runtime_error("failed to set up debug messenger!");
	//} else {
	//	func(instance, &messenger_create_info, nullptr, &debug_messenger);
	//}

skip_debug_messenger:
	// Window
	VkResult result = vkCreateInstance(&create_info, nullptr, &instance);
	if (VK_SUCCESS != result) { cout << "Failed to create instance! " << result; }

	// 3a: Physical devices and queue families
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
{
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
	vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

	int32_t max_score = 0;
	for (const auto &device : devices) {
		VkPhysicalDeviceProperties device_properties;
		vkGetPhysicalDeviceProperties(device, &device_properties);
		VkPhysicalDeviceFeatures device_features;
		vkGetPhysicalDeviceFeatures(device, &device_features);

		int32_t score = 0;

		if (VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU == device_properties.deviceType) { score += 1000; }
		score += device_properties.limits.maxImageDimension2D;
		if (!device_features.geometryShader) { score = 0; }

		if (score > max_score) {
			max_score = score;
			cout << device << "\n";
			physical_device = device;
		}
	}
}
	if (VK_NULL_HANDLE == physical_device) {
		std::runtime_error("Failed to find GPUs with Vulkan support!\n");
	} else {
		cout << "Found Vulkan supported GPU!\n";
	}
	//
	struct QueueFamilyIndices {
		uint32_t graphicsFamily;
	};


	uint32_t extCout = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extCout, nullptr);
	using std::cout;
	cout << extCout << " extensions supported.\n";

	glm::mat4 matrix;
	glm::vec4 vec;
	auto test = matrix * vec;

	//while (!glfwWindowShouldClose(window)) {
	//	glfwPollEvents();
	//}

	// Cleanup
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();
}
