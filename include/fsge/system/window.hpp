#ifndef _FSGE_SYSTEM_WINDOW_HPP_INCLUDED
#define _FSGE_SYSTEM_WINDOW_HPP_INCLUDED

#include <iostream>
#include <cstring>
#include <string>

extern "C" {
	#if defined (_WIN32)
	#elif defined(__linux__)
		#include <xcb/xcb.h>
		#include <xcb/sync.h>
		#include <fsge/system/input/xkb.h>
	#elif defined(__ANDROID__)
	#endif
}

#include <fsge/core/components/components.hpp>
#include <fsge/system/input/input.hpp>

namespace fsge {
	class Window
	{
		RegisterAsComponent(Window);
		public:
		enum Flags {
			NoFlags = 0x0,
			FullScreen = 0x1,
			Resizable = 0x2
		};

			Window() {
			}
			~Window() {
				
			}

			xcb_connection_t* connection() {
				return _connection;
			}

			xcb_window_t window() {
				return _winref;
			}

			void destroy() {
				#if defined(__linux__)
				xcb_destroy_window(_connection, _winref);
				xcb_disconnect(_connection);
				#endif
			}
			Window(unsigned short width, unsigned short height, const char *caption, Flags flags);
			
			unsigned short int get_width();
			void set_width(unsigned short int width);
			unsigned short int get_height();
			void set_height(unsigned short int height);
			void rename(const char *caption);

			void update_events();
			
			const char* _caption;
			InputEvent inputevent;
		private:
		
			#if defined(__linux__)
			xcb_connection_t* _connection;
			xcb_window_t _winref;
			xcb_screen_t* _screen;
			const xcb_setup_t* _setup;
			struct AtomProperties {
				xcb_atom_t wm_protocols;
				xcb_atom_t wm_delete_window;
				xcb_atom_t _net_wm_sync_request;
				xcb_atom_t _net_wm_sync_request_counter;
				unsigned int active_properties_count;
			} _props;
			struct Sync {
				xcb_sync_counter_t counter;
				xcb_sync_int64_t value;
			} _sync;
			
			#endif

			uint32_t _width;
			uint32_t _height;
			
	};
}
#endif