#pragma once
#ifndef _MEPHISTO_CORE_RESSOURCES_TYPESENUM_HPP_INCLUDED
#define _MEPHISTO_CORE_RESSOURCES_TYPESENUM_HPP_INCLUDED

namespace mephisto {
	template <typename T>
	class Ressource
	{
	public:
	    static constexpr const int* get_id()
	    {
	        return &id;
	    }
	private:
	    static constexpr int id = 0;
	};
}

#endif