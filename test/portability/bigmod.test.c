#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

#define INT64( i)  ((int64_t)(INT64_C(i)))

#define UINT64( i) ((uint64_t)(UINT64_C(i)))

int main(int argc, char** argv)
{
  uint64_t useed = ((uint64_t)(INT64(314159265)));
  uint64_t two_46 = UINT64(70368744177664);
  uint64_t umod1 = INT64(314159265);
  uint64_t umod2 = (useed % two_46);
  uint64_t umod3 = (useed % UINT64(70368744177664));
  uint64_t umod4 = (useed % (UINT64_C(70368744177664)));
  uint64_t umod5 = (useed % ((uint64_t)70368744177664));
  uint64_t umod6 = (useed % 70368744177664ul);
  // Expect all 3 of these to be the same
  printf("%lu\n", (unsigned long) umod1);
  printf("%lu\n", (unsigned long) umod2);
  printf("%lu\n", (unsigned long) umod3);
  printf("%lu\n", (unsigned long) umod4);
  printf("%lu\n", (unsigned long) umod5);
  printf("%lu\n", (unsigned long) umod6);
  return 0;
}
