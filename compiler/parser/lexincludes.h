#ifndef _LEX_INCLUDES_H_
#define _LEX_INCLUDES_H_

#ifndef MODULE_FINDER
#include "lexyacc.h"
#include "chapel.tab.h"
#else
#include "chplalloc.h"
#include "processTokens.h"
#include "modulefinder.tab.h"
#include "yy.h"
#define countToken(x)
#define countNewline()
#define countSingleLineComment(x)
#define countMultiLineComment(x)
#define countCommentLine()
//#define MALLOC(x) malloc(x)
//#define REALLOC(x, y) realloc(x,y)
//#define FREE(x) free(x)
#endif

#endif
