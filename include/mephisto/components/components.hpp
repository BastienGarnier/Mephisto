#pragma once
#ifndef _MEPHISTO_CORE_COMPONENTS_TYPESENUM_HPP_INCLUDED
#define _MEPHISTO_CORE_COMPONENTS_TYPESENUM_HPP_INCLUDED

#include <stdint.h>
#include <iostream>

#include <type_traits>
#include <tuple>

#include <mephisto/utilities.hpp>
#include <mephisto/components/constants.hpp>

extern size_t _components_start;
extern size_t _components_end;

#define RegisterComponent(COMPONENT) template class mephisto::Component<COMPONENT>; template<> uint8_t mephisto::Component<COMPONENT>::id = 0

#pragma GCC diagnostic ignored "-Wnon-template-friend"

namespace mephisto {
	template <typename T = void>
	class Component
	{
	public:
		inline static constexpr const uint64_t get_id()
		{
			return ((uint64_t)(&id) - (uint64_t)(&_components_start));
		}
	private:
		static uint8_t __attribute__((section(".componentsSection"))) id;
	};

	typedef Component<void> Components;
	template <>
	class Component<void>
	{
	public:
		inline static size_t count() {
			return (size_t)(&_components_end) - (size_t)(&_components_start);
		}
	};
}

#endif