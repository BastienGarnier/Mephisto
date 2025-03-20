#include "mephisto/window/window.hpp"

using namespace mephisto;

Window::Window(unsigned short width, unsigned short height, const char *caption, Flags flags, VkInstance instance) : caption(caption) {
	#if defined(__linux__)
	int screen_prefer = 0;
	_connection = xcb_connect(NULL, &screen_prefer);
	#if defined DEBUG
	if (xcb_connection_has_error(_connection)) {
		throw std::runtime_error("XCB connection error");
	}
	#endif
	_setup = xcb_get_setup(_connection);
	// xcb_show_keyboard_mapping(_connection, _setup);
	//Initialize the X Keyboard (xkb) support extention
	xcb_xkb_use_extension(_connection, XCB_XKB_MAJOR_VERSION, XCB_XKB_MINOR_VERSION);

	//Set the per client detectable auto repeat flag
	xcb_xkb_per_client_flags(_connection, XCB_XKB_ID_USE_CORE_KBD, XCB_XKB_PER_CLIENT_FLAG_DETECTABLE_AUTO_REPEAT, 1,0,0,0);
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(_setup);
	for (int s = screen_prefer; s > 0; s--)
		xcb_screen_next(&iter);
	_screen = iter.data;

	_winref = xcb_generate_id(_connection);

	uint32_t event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

	uint32_t values_list[] = {_screen->black_pixel, 0 };

	xcb_create_window(
		_connection,
		XCB_COPY_FROM_PARENT,
		_winref,
		_screen->root, // window parent
		0, // x coord
		0, // y coord
		width,
		height,
		0, // border width
		XCB_WINDOW_CLASS_INPUT_OUTPUT, // class : in 'XCB_WINDOW_CLASS_COPY_FROM_PARENT', 'XCB_WINDOW_CLASS_INPUT_OUTPUT', or 'XCB_WINDOW_CLASS_INPUT_ONLY'
		_screen->root_visual,
		event_mask,
		values_list
	);

	xcb_change_property(
		_connection,
		// XCB_PROP_MODE_REPLACE (Discard the previous property value and store the new data)
		// XCB_PROP_MODE_PREPEND (Insert the new data before the beginning of existing data)
		// XCB_PROP_MODE_APPEND (Insert the new data after the beginning of existing data)
		XCB_PROP_MODE_REPLACE, 
		_winref,
		XCB_ATOM_WM_NAME, // property we want to change (an atom)
		XCB_ATOM_STRING, // type of the property (an atom)
		8, // format -> specify 'caption' data size (8, 16 or 32)
		strlen(caption),
		caption
	);

	xcb_intern_atom_cookie_t wmDeleteCookie = xcb_intern_atom(_connection, 0, strlen("WM_DELETE_WINDOW"), "WM_DELETE_WINDOW");
	xcb_intern_atom_cookie_t wmProtocolsCookie = xcb_intern_atom(_connection, 0, strlen("WM_PROTOCOLS"), "WM_PROTOCOLS");
	xcb_intern_atom_cookie_t wmNetSyncRequestCookie = xcb_intern_atom(_connection, 0, strlen("_NET_WM_SYNC_REQUEST"), "_NET_WM_SYNC_REQUEST");
	xcb_intern_atom_cookie_t wmNetSyncRequestCounterCookie = xcb_intern_atom(_connection, 0, strlen("_NET_WM_SYNC_REQUEST_COUNTER"), "_NET_WM_SYNC_REQUEST_COUNTER");

	xcb_intern_atom_reply_t *wmDeleteReply = xcb_intern_atom_reply(_connection, wmDeleteCookie, NULL);
	xcb_intern_atom_reply_t *wmProtocolsReply = xcb_intern_atom_reply(_connection, wmProtocolsCookie, NULL);
	xcb_intern_atom_reply_t *wmNetSyncRequestReply = xcb_intern_atom_reply(_connection, wmNetSyncRequestCookie, NULL);
	xcb_intern_atom_reply_t *wmNetSyncRequestCounterReply = xcb_intern_atom_reply(_connection, wmNetSyncRequestCounterCookie, NULL);

	_props.wm_delete_window = wmDeleteReply->atom;
	_props.wm_protocols = wmProtocolsReply->atom;
	_props._net_wm_sync_request_counter = wmNetSyncRequestReply->atom;
	_props._net_wm_sync_request = wmNetSyncRequestCounterReply->atom;

	xcb_atom_t properties[2] = {_props.wm_delete_window, _props._net_wm_sync_request};

	xcb_change_property(_connection, XCB_PROP_MODE_REPLACE, _winref, _props.wm_protocols, XCB_ATOM_ATOM, 32, 2, properties);
	// {
	// 	_sync.counter = xcb_generate_id(_connection);
	// 	_sync.value.hi = 0;
	// 	_sync.value.lo = 0;
	// 	xcb_sync_create_counter_checked(_connection, _sync.counter, _sync.value);
	// }
	// xcb_change_property(_connection, XCB_PROP_MODE_REPLACE, _winref, _props._net_w_sync_request_counter, XCB_ATOM_CARDINAL, 32, 1, &_sync.counter);


	// voir l. 341 de xproto.h dans l'include de XCB pour l'énumération
	uint32_t mask = XCB_EVENT_MASK_EXPOSURE
				  | XCB_EVENT_MASK_KEY_PRESS
				  | XCB_EVENT_MASK_KEY_RELEASE
				  | XCB_EVENT_MASK_KEYMAP_STATE
				  | XCB_EVENT_MASK_ENTER_WINDOW
				  | XCB_EVENT_MASK_LEAVE_WINDOW
				  | XCB_EVENT_MASK_STRUCTURE_NOTIFY
				  | XCB_EVENT_MASK_POINTER_MOTION
				  | XCB_EVENT_MASK_PROPERTY_CHANGE;
	xcb_change_window_attributes(_connection, _winref, XCB_CW_EVENT_MASK, &mask);

	xcb_map_window(_connection, _winref);
	xcb_flush(_connection);
	#endif

	create_surface(instance);
}

void Window::update_events() {
	xcb_generic_event_t *event = xcb_poll_for_event(_connection);
	if (event == nullptr) {
		return;
	}
	inputevent.type = InputEvent::Unknown;
	switch (event->response_type & ~0x80) {
	case XCB_KEY_PRESS: 
	{
		inputevent.type = static_cast<InputEvent::Type>(inputevent.type | InputEvent::KeyPressed);
		// xcb_key_press_event_t *kev = (xcb_key_press_event_t*)event;
	}
	break;
	case XCB_KEY_RELEASE:
	{
		inputevent.type = static_cast<InputEvent::Type>(inputevent.type | InputEvent::KeyReleased);
		// xcb_key_release_event_t *kev = (xcb_key_release_event_t*)event;
	}
	break;
	case XCB_CLIENT_MESSAGE: 
	{
		xcb_client_message_event_t* cm = (xcb_client_message_event_t *)event;
		if (cm->data.data32[0] == _props.wm_delete_window) {
			inputevent.type = static_cast<InputEvent::Type>(inputevent.type | InputEvent::Closed);
		}
	}
	break;
	default:
	{
	}
	}
	free(event);
}

void Window::create_surface(VkInstance instance) {
	#if defined(__linux__)
	VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = NULL;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.connection = _connection;
	surfaceCreateInfo.window = _winref;
	VkResult result = vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, NULL, &_vulkan.surface);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create XCB surface");
	}
	#endif
}

void Window::create_synchronization_objects(VkDevice device) {
	_vulkan.semaphores_image_available.resize(MAX_FRAMES_IN_FLIGHT);
	_vulkan.semaphores_render_finished.resize(MAX_FRAMES_IN_FLIGHT);
	_vulkan.fences_in_flight.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_vulkan.semaphores_image_available[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create image available semaphore for a frame !");
		}

		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_vulkan.semaphores_render_finished[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create render finished semaphore for a frame !");
		}
		if (vkCreateFence(device, &fenceInfo, nullptr, &_vulkan.fences_in_flight[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create in flight fence for a frame !");
		}
	}
}


void Window::create_swapchain(VkPhysicalDevice physical_device, VkDevice logical_device) {
	_vulkan.swapchain.create(physical_device, logical_device, _vulkan.surface);
}