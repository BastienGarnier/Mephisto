#pragma once
#ifndef _MEPHISTO_GAME_HPP_INCLUDED
#define _MEPHISTO_GAME_HPP_INCLUDED

#include <vulkan/vulkan.hpp>

#include <mephisto/application/application.hpp>

#include <mephisto/vulkan/context.hpp>


#include <mephisto/window/window.hpp>
#include <mephisto/window/handler.hpp>

namespace mephisto {
	class GraphicalApplication : public Application
	{
	public:
		GraphicalApplication() {
			_context = ressourcesmanager.add<vulkan::Context>("VULKANCONTEXT", new vulkan::Context());
			_window = ressourcesmanager.add<Window>("main", new Window(640, 480, "Test", Window::NoFlags, _context->instance()));
			
			_context->initialize(_window->surface());
			_window->initialize(_context);

			processusmanager.add((Processus*)(new WindowHandler(this)), 0);
		}
		~GraphicalApplication() {
			vkDeviceWaitIdle(_context->logical_device());

			_window->destroy(_context);
			_context->destroy();
		}

		inline vulkan::Context* context() {
			return _context;
		}

		inline Window* window() {
			return _window;
		}
	private:
		vulkan::Context* _context;
		Window* _window;
	};
}
#endif