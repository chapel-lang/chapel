#ifndef CHPL_PM_DATA
#define CHPL_PM_DATA

#include "system.hpp"

namespace nanos {
namespace chapel {

struct ChapelWDData
{
   bool serial;
};

class ChapelPMInterface : public PMInterface {
   int getInternalDataSize() const { return sizeof(ChapelWDData); } 
   int getInternalDataAlignment() const { return __alignof__(ChapelWDData); } 
};

}
}

#endif
