#include "mephisto/components/sysconstants.hpp"

SECTIONS
{
  .componentsSegment MEPHISTO_COMPONENT_ID_OFFSET :
  {
    KEEP(*(.componentsSection)) 
  }
}