/* -*-Mode: c++;-*-
 Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _internal_h_
#define _internal_h_

enum Internal {
  Internal_NONE = 0,
  Internal_Domain, 
  Internal_Sequence
};

#define CPP_IS_LAME {						\
  "NONE", "Domain", "Sequence"}
EXTERN char *internal_string[] EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

#endif
