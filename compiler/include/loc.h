#ifndef _LOC_H_
#define _LOC_H_

#include <stdio.h>
#include "chplalloc.h"

class Loc : public gc {
 public:
  char* filename;
  int lineno;

  Loc(void);

  char* stringLoc(void);
  void printLoc(FILE* outfile);
};

#endif

