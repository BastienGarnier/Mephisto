#ifndef _MEPHISTO_GAME_HPP_INCLUDED
#define _MEPHISTO_GAME_HPP_INCLUDED

#include <vulkan/vulkan.hpp>

#include <mephisto/application/application.hpp>

#include <mephisto/vulkan/context.hpp>
#include <mephisto/window/window.hpp>
#include <mephisto/window/handler.hpp>

namespace mephisto {
	class Game : public Application
	{
	public:
		Game() {
			_vk_context = ressourcesmanager.add<vulkan::Context>("VULKANCONTEXT", new vulkan::Context());
			_vk_window = ressourcesmanager.add<Window>("Main", new Window(640, 480, "Test", Window::NoFlags, _vk_context->instance()));
			_vk_context->initialize(_vk_window->surface());
			_vk_window->initialize(_vk_context->physical_device(), _vk_context->logical_device());

			add_processus((Processus*)(new WindowHandler(this)), 0); 
		}
		~Game() {
			vkDeviceWaitIdle(_vk_context->logical_device());
			_vk_window->destroy(_vk_context->logical_device(), _vk_context->instance());
		}
	private:
		vulkan::Context* _vk_context;
		Window* _vk_window;
	};
}
#endif