#ifndef _LOC_H_
#define _LOC_H_

class Loc {
 public:
  char* filename;
  int lineno;

  Loc(void);
};

#endif

