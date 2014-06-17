/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _LEXYACC_H_
#define _LEXYACC_H_

#include "build.h"
#include "countTokens.h"
#include "driver.h"
#include "expr.h"
#include "parser.h"
#include "processTokens.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "yy.h"

// Make sure exit is undefined:
#ifdef exit
#undef exit
#endif
// And redefine it to call our exit routine:
#define exit(x) clean_exit(x)

extern int captureTokens;
extern char captureString[1024];

#endif
