#include "mephisto/components/constants.hpp"
#include "mephisto/ressources/constants.hpp"

SECTIONS
{
	.mephisto_components MEPHISTO_COMPONENTS_ID_OFFSET :
	{
		. = ALIGN(1);
		_components_start = .;
		KEEP(*(.componentsSection)) 
		_components_end = .;
	}

	.mephisto_ressources MEPHISTO_RESSOURCES_ID_OFFSET :
	{
		KEEP(*(.ressourcesSection)) 
	}
}