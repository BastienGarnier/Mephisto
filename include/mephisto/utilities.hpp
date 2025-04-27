#pragma once
#ifndef _MEPHISTO_MISCELLANEOUS_HPP_INCLUDED
#define _MEPHISTO_MISCELLANEOUS_HPP_INCLUDED

namespace mephisto {
	#define UNUSED(x) (void)(x)

	class Object {
	public:

		template <typename T>
		static Object* make(T *t) {
			return (Object*)t;
		}
	};
	
	template <class T>
	class Passer
	{
	public:
		union {T value;};

		template<typename ...Args>
		Passer(Args... input_args) : value(input_args...) {}
		~Passer() {}
	};

}

#endif
