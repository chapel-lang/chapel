/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#ifndef _clone_h_
#define _clone_h_

#include "geysa.h"

class FA;
class EntrySet;
class AType;

int clone(FA *fa, Fun *top);
void fixup_clone(Fun *f, Vec<EntrySet *> *ess);
Sym *basic_type(FA *fa, AType *t, Sym *fail);
void log_test_fa(FA *fa);


#endif
