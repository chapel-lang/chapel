#include <stdio.h>
#include <inttypes.h>

typedef struct testrec {
  int64_t x;
  int64_t y;
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
  printf("testret = %" PRIu64 " %" PRIu64 "\n",
         t->x, t->y);
}
