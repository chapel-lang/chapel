#include <stdio.h>

typedef struct testrec {
  long long int x;
  long long int y;
} testrec;

static inline
void init_testrec(testrec* t, int x, int y)
{
  t->x = x;
  t->y = y;
}

static inline
void debug_print_testrec(testrec* t)
{
  printf("testret = %lli %lli\n",
         t->x, t->y);
}
