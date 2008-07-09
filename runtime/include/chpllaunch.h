#ifndef _CHPL_LAUNCH_H_
#define _CHPL_LAUNCH_H_

#include "chpltypes.h"

char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales);

#endif
