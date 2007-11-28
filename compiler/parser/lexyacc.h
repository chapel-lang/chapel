#ifndef _LEXYACC_H_
#define _LEXYACC_H_

#include "build.h"
#include "countTokens.h"
#include "driver.h"
#include "expr.h"
#include "processTokens.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "yy.h"

extern int captureTokens;
extern char captureString[1024];

#endif
