#ifndef _FSGE_TUPLE_HPP_INCLUDED
#define _FSGE_TUPLE_HPP_INCLUDED

#include <tuple>
#include <type_traits>
#include <utility>
#include <cstddef>
#include <iostream>

namespace fsge {
	namespace Tuple {
		template <typename T, typename Tuple>
		struct has_type;

		template <typename T>
		struct has_type<T, std::tuple<>> : std::false_type {};

		template <typename T, typename U, typename... Ts>
		struct has_type<T, std::tuple<U, Ts...>> : has_type<T, std::tuple<Ts...>> {};

		template <typename T, typename... Ts>
		struct has_type<T, std::tuple<T, Ts...>> : std::true_type {};

		// // swap types at index i and index j in the template argument tuple
		// template <std::size_t i, std::size_t j, class Tuple>
		// class tuple_element_swap
		// {
		// 	template <class IndexSequence>
		// 	struct tuple_element_swap_impl;

		// 	template <std::size_t... indices>
		// 	struct tuple_element_swap_impl<std::index_sequence<indices...>>
		// 	{
		// 		using type = std::tuple
		// 		<
		// 			std::tuple_element_t
		// 			<
		// 				indices != i && indices != j ? indices : indices == i ? j : i, Tuple
		// 			>...
		// 		>;
		// 	};

		// public:
		// 	using type = typename tuple_element_swap_impl
		// 	<
		// 		std::make_index_sequence<std::tuple_size<Tuple>::value>
		// 	>::type;
		// };

		// // selection sort template argument tuple's variadic template's types
		// template <template <class, class> class Comparator, class Tuple>
		// class tuple_selection_sort
		// {
		// 	// selection sort's "loop"
		// 	template <std::size_t i, std::size_t j, std::size_t tuple_size, class LoopTuple>
		// 	struct tuple_selection_sort_impl
		// 	{
		// 		// this is done until we have compared every element in the type list
		// 		using tuple_type = std::conditional_t
		// 		<
		// 			Comparator
		// 			<
		// 				std::tuple_element_t<i, LoopTuple>,
		// 				std::tuple_element_t<j, LoopTuple>
		// 			>::value,
		// 			typename tuple_element_swap<i, j, LoopTuple>::type, // true: swap(i, j)
		// 			LoopTuple										   // false: do nothing
		// 		>;

		// 		using type = typename tuple_selection_sort_impl // recurse until j == tuple_size
		// 		<
		// 			i, j + 1, tuple_size, tuple_type // using the modified tuple
		// 		>::type;
		// 	};

		// 	template <std::size_t i, std::size_t tuple_size, class LoopTuple>
		// 	struct tuple_selection_sort_impl<i, tuple_size, tuple_size, LoopTuple>
		// 	{
		// 		// once j == tuple_size, we increment i and start j at i + 1 and recurse
		// 		using type = typename tuple_selection_sort_impl
		// 		<
		// 			i + 1, i + 2, tuple_size, LoopTuple
		// 		>::type;
		// 	};

		// 	template <std::size_t j, std::size_t tuple_size, class LoopTuple>
		// 	struct tuple_selection_sort_impl<tuple_size, j, tuple_size, LoopTuple>
		// 	{
		// 		// once i == tuple_size, we know that every element has been compared
		// 		using type = LoopTuple;
		// 	};

		// public:
		// 	using type = typename tuple_selection_sort_impl
		// 	<
		// 		0, 1, std::tuple_size<Tuple>::value, Tuple
		// 	>::type;
		// };

		// template <typename T,size_t reps>
		// struct repeated_type {
		// 	using type = T;
		// 	static const size_t N = reps;
		// };

		// template <typename T,std::size_t...n>
		// auto n_tuple_impl(std::index_sequence<n...>){ 
		// 	return std::make_tuple(((void)n,T{})...); 
		// }

		// template <typename repT>
		// using n_tuple_single = decltype( n_tuple_impl<typename repT::type>( std::make_index_sequence<repT::N>() ) );

		// template <typename...repT>
		// using n_tuple = decltype( std::tuple_cat( ( n_tuple_single<repT>() )... ) );

		// template <typename T, typename ...Ts>
		// auto from_templates() {
		// 	using one_T = repeated_type<T, 1>;
		// 	if constexpr (sizeof...(Ts)) {
		// 		return std::tuple_cat(from_templates<Ts...>(), n_tuple_single<one_T>{}); // tuple d'entier avec autant de valeurs qu'il y a de types entrés
		// 	} else {
		// 		return n_tuple<one_T>{};
		// 	}
		// }

		template <class T>
		consteval std::size_t size(T value) {
			return std::tuple_size<T>{};
		}

		template<class Tuple, size_t... I>
		void print(const Tuple& t, std::index_sequence<I...>)
		{
		    std::cout << "(";
		    (..., (std::cout << (I == 0? "" : ", ") << std::get<I>(t)));
		    std::cout << ")\n";
		}

		template<class... T>
		void print (const std::tuple<T...>& t)
		{
		    print(t, std::make_index_sequence<sizeof...(T)>());
		}

	}
}

#endif