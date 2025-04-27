#pragma once
#ifndef _MEPHISTO_WINDOW_HANDLER_HPP_INCLUDED
#define _MEPHISTO_WINDOW_HANDLER_HPP_INCLUDED

#include <mephisto/processus/processus.hpp>
#include <mephisto/application/application.hpp>

#include <mephisto/window/window.hpp>

namespace mephisto {
	class WindowHandler : Processus
	{
	public:
		WindowHandler(Application *app) : app(app) {

		}
		~WindowHandler();

		void destroy() {
			
		}

		void start() {

		}

		void update() {
			for (auto itr : app->ressourcesmanager.all<Window>()) {
				Window* win = (Window*)itr.second;
				win->update_events();

				if (win->inputevent.type & InputEvent::Closed) { // supprime la fenêtre si reçoit l'évènemment pour quitter
					app->close();
				}
			}
		}

		void stop() {
		}

		Application* app;
	};
}

#endif