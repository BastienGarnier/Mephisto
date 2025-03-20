#ifndef _MEPHISTO_SYSTEM_EVENT_HPP_INCLUDED
#define _MEPHISTO_SYSTEM_EVENT_HPP_INCLUDED

#include <mephisto/input/keyboard.hpp>

namespace mephisto {
	class InputEvent
	{
	public:
		enum Type {
			Unknown = 0,
			Closed = 2 << 0,
			KeyPressed = 2 << 1,
			KeyReleased = 2 << 2,
			MouseEntered = 2 << 3,
			MouseLeft = 2 << 4,
			Resized = 2 << 5,
			MouseMoved = 2 << 6,
			MouseButtonPressed = 2 << 7,
			MouseButtonReleased = 2 << 8
		};

		InputEvent(){

		}

		~InputEvent() {
			
		}

		union EventData {
			struct {
				// Input::Scan scancode;
				Keyboard::ScanCode scancode;
			} key;
			struct {
				int x;
				int y;
				int x_root;
				int y_root;
				unsigned int state;
			} mouse;
		};
		Type type;
	private:
		
	};
}

#endif
