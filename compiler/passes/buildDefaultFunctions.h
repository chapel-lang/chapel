#ifndef _BUILD_DEFAULT_FUNCTIONS_H_
#define _BUILD_DEFAULT_FUNCTIONS_H_

#include "pass.h"
#include "type.h"

void buildDefaultFunctions(void);
void buildDefaultClassTypeMethods(ClassType* ct);
void buildDefaultIOFunctions(Type* type);

#endif
