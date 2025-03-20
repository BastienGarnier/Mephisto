#include "mephisto/vulkan/context.hpp"

using namespace mephisto;

using namespace vulkan;

const std::vector<const char*> device_extensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


void Context::create_instance() {
	{
		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, NULL);
		if (layer_count > 0) {
			std::cout << layer_count << " Layers" << std::endl;
			std::vector<VkLayerProperties> layer_properties;
			layer_properties.resize(layer_count);
			vkEnumerateInstanceLayerProperties(&layer_count, layer_properties.data());
			for (uint32_t i = 0; i < layer_count; i++) {
				std::cout << "\t" << layer_properties[i].layerName << " : " << layer_properties[i].description << std::endl;
			}
		} else {
			std::cout << "No layers" << std::endl;
		}
	}

	VkApplicationInfo app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "MEPHISTO App";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "MEPHISTO";
	app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	app_info.apiVersion = VK_API_VERSION_1_0;
	app_info.pNext = nullptr;

	std::vector<const char*> enabled_extensions;

	#if defined(_WIN32)
		enabled_extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
	#elif defined(__ANDROID__)
		enabled_extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
	#elif defined(__linux__)
		enabled_extensions.push_back("VK_KHR_surface");
		enabled_extensions.push_back("VK_KHR_xcb_surface");
	#endif

	VkInstanceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;
	create_info.pNext = nullptr;
	create_info.flags = 0;
	create_info.enabledExtensionCount = enabled_extensions.size();
	create_info.ppEnabledExtensionNames = enabled_extensions.data();
	create_info.enabledLayerCount = 0;

	VkResult result = vkCreateInstance(&create_info, nullptr, &_instance);

	if (result == VK_ERROR_INCOMPATIBLE_DRIVER) {
		throw std::runtime_error(	"Cannot find a compatible Vulkan installable client "
									"driver (ICD). Please make sure your driver supports "
									"Vulkan before continuing. The call to vkCreateInstance failed.");
	} else if (result != VK_SUCCESS) {
		throw std::runtime_error(	"The call to vkCreateInstance failed. Please make sure "
									"you have a Vulkan installable client driver (ICD) before "
									"continuing.");
	}
}

void Context::setup_debug_messenger() {

}

void Context::pick_physical_device(VkSurfaceKHR surface) {
	_physical_device = VK_NULL_HANDLE;
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(_instance, &device_count, nullptr); // stocke nulle part les appareils
	if (device_count == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support !");
	}

	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(_instance, &device_count, devices.data()); // mtnt qu'on sait qu'y en a, on les stockes dans le tab

	std::multimap<int, VkPhysicalDevice> candidates;
	for (const auto& device : devices) {
		int score = rate_device_suitability(device, surface);
		candidates.insert(std::make_pair(score, device));
	}
	if (candidates.rbegin()->first > 0) {
		_physical_device = candidates.rbegin()->second;
	} else {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(_physical_device, &deviceProperties);
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(_physical_device, &deviceFeatures);
	std::cout << "Find GPU : " << deviceProperties.deviceName << std::endl;
	_qfi = find_queue_families(_physical_device, surface);
}

int Context::rate_device_suitability(VkPhysicalDevice device, VkSurfaceKHR surface) {
	int score = 0;
	QueueFamilyIndices indices = find_queue_families(device, surface);

	if (!indices.is_complete()) {
		return 0;
	}

	bool extensions_supported = check_device_extensions(device);
	if (!extensions_supported) {
		return 0;
	}

	bool swapChainAdequate = false;
	if (extensions_supported) {
		SwapChainSupportDetails swapChainSupport = Swapchain::query_swap_chain_support_details(device, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	if (!swapChainAdequate) {
		return 0;
	}

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	if (!deviceFeatures.geometryShader) {
		return 0;
	}

	score += (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) * 1000;
	score += deviceProperties.limits.maxImageDimension2D;
	return score;
}

void Context::create_logical_device() {
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {_qfi.graphicsFamily.value(), _qfi.presentFamily.value()};

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures device_features{};

	VkDeviceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.pQueueCreateInfos = queueCreateInfos.data();
	create_info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	create_info.pEnabledFeatures = &device_features;
	create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
	create_info.ppEnabledExtensionNames = device_extensions.data();
	create_info.enabledLayerCount = 0;

	if (vkCreateDevice(_physical_device, &create_info, nullptr, &_logical_device) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create logical device");
	}

	vkGetDeviceQueue(_logical_device, _qfi.graphicsFamily.value(), 0, &_graphics_queue);
	vkGetDeviceQueue(_logical_device, _qfi.presentFamily.value(), 0, &_present_queue);
	std::cout << "Graphic bit on queue family " << _qfi.graphicsFamily.value() << std::endl;
	std::cout << "Present queue on queue family " << _qfi.presentFamily.value() << std::endl;
}

bool Context::check_device_extensions(VkPhysicalDevice device) {
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> available_extensions(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

	// std::cout << extension_count << " extensions" << std::endl;
	// for (uint32_t i = 0; i < extension_count; i++) {
	// 	std::cout << "\t" << available_extensions[i].extensionName << std::endl;
	// }

	std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

	for (const auto& extension : available_extensions) {
		required_extensions.erase(extension.extensionName);
	}

	return required_extensions.empty();
}