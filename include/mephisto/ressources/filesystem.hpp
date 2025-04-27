#pragma once
#ifndef _MEPHISTO_RESSOURCES_FILESYSTEM_HPP_INCLUDED
#define _MEPHISTO_RESSOURCES_FILESYSTEM_HPP_INCLUDED

#include <vector>
#include <iostream>
#include <fstream>

namespace mephisto {
	std::vector<char> read_file(const std::string &filename);
}

#endif