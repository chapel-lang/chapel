/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#ifndef _clone_h_
#define _clone_h_

int clone(FA *fa, Fun *top);
void fixup_clone(Fun *f, Vec<EntrySet *> *ess);

#endif
