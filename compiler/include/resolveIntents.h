#ifndef _RESOLVEINTENTS_H_
#define _RESOLVEINTENTS_H_

#include "symbol.h"
#include "type.h"

IntentTag blankIntentForType(Type* t);
void resolveArgIntent(ArgSymbol* arg);

#endif

