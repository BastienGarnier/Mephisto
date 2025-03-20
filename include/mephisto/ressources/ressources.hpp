#ifndef _MEPHISTO_CORE_RESSOURCES_TYPESENUM_HPP_INCLUDED
#define _MEPHISTO_CORE_RESSOURCES_TYPESENUM_HPP_INCLUDED

#include <type_traits>
#include <tuple>

#define RegisterAsRessource(RESSOURCE) [[maybe_unused]] constinit const static bool __mephisto_ressource_declaration_hack = (mephisto::Ressources::declare<RESSOURCE>(), true)

#pragma GCC diagnostic ignored "-Wnon-template-friend"

namespace mephisto {
	namespace Ressources {
		namespace _details {
			template<class...> struct type_list {};

			template<auto> struct nth {
				consteval auto friend gett(nth);
			};
			template<auto N, class T> struct sett {
				consteval auto friend gett(nth<N>) {
					return T{};
				}
			}; 

			template<class> struct numbering {
				consteval auto friend getn(numbering);
			};

			template<auto N, class T> struct setn {
				consteval auto friend getn(numbering<T>) {
					return N;
				}
			};

			template<auto> struct nthl {
				consteval auto friend getl(nthl);
			};

			template<auto N, class T> struct setl {
				consteval auto friend getl(nthl<N>) {
					return T{};
				}
			}; 
			
			template<class T, template<class...> class TList, class... Ts> auto append(TList<Ts...>) -> TList<Ts..., T>;
		} // namespace _details

		template<class T, auto N = 0, auto unique = []{}>
		consteval auto declare() {
			if constexpr (requires { getl(_details::nthl<N>{}); }) {
				declare<T, N+1, unique>();
			} else if constexpr (N == 0) {
				void(_details::sett<N, T>{});
				void(_details::setn<N, T>{});
				void(_details::setl<N, _details::type_list<T>>{});
			} else {
				void(_details::sett<N, T>{});
				void(_details::setn<N, T>{});
				void(_details::setl<N, decltype(_details::append<T>(getl(_details::nthl<N-1>{})))>{});
			}
		}

		template<auto unique = []{}, auto N = 0>
		consteval auto get_list() {
			if constexpr (requires { getl(_details::nthl<N>{}); }) {
				return get_list<unique, N+1>();
			} else if constexpr (N == 0) {
				return _details::type_list{};
			} else {
				return getl(_details::nthl<N>{});
			}
		}

		template<auto N = 0, auto unique = []{}>
		consteval auto get_count() {
			if constexpr (requires{getl(_details::nthl<N>{}); }) {
				return get_count<N+1, unique>();
			} else {
				return N;
			}
		}

		template<class T>
		consteval auto get_enum() {
			return getn(_details::numbering<T>{});
		}

		template<auto N, auto unique = []{}>
		consteval auto get_type() {
			return gett(_details::nth<N>{});
		}
		
	}
}

#endif