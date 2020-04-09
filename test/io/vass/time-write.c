// Tests of printing a newline a bunch of times.

#include <stdio.h>
#include "time-write.h"

// printf() a newline, 'numLines' times
void cf_trial(int numLines) {
  int i;
  for (i = 0; i < numLines; i++)
    printf("\n");
}

// puts(), with an implicit newline, 'numLines' times
void cs_trial(int numLines) {
  int i;
  for (i = 0; i < numLines; i++)
    puts("");
}

//////////////////////////////////////////////////////////////////

#ifdef PROVIDE_MAIN

#include <sys/time.h>
#include <stdlib.h>

static double gettime(void) {
  struct timeval t;
  gettimeofday(&t, 0);
  return t.tv_sec + t.tv_usec*1e-6;
}

int main(int argc, char **argv)
{
  int numLines;
  double t1, t2;
  if (argc < 2) {
    fprintf(stderr, "USAGE: %s loop-iterations\n", argv[0]);
    return 1;
  }
  numLines = atoi(argv[1]);
  fprintf(stderr, "started testing with %d lines\n", numLines);

  t1 = gettime();
  cf_trial(numLines);
  t2 = gettime();
  fprintf(stderr, "printf-ed %d lines in %7.3f seconds\n", numLines, t2-t1);

  t1 = gettime();
  cs_trial(numLines);
  t2 = gettime();
  fprintf(stderr, "puts-ed   %d lines in %7.3f seconds\n", numLines, t2-t1);

  return 0;
}

#endif // PROVIDE_MAIN
