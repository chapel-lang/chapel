// g++ -O3 stl-sort-random.cc

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cinttypes>
#include <cassert>
#include <algorithm>

int main() {

  int err = 0;
  int n = 1024*1024*128;
  uint64_t* arr = (uint64_t*) malloc(n*sizeof(uint64_t));
  struct timespec startts;
  struct timespec endts;
  double start, end;

  for (int i = 0; i < n; i++) {
    uint64_t key = 0;
    for (size_t j = 0; j < 8; j++) {
      key <<= 8;
      key ^= rand();
    }
    arr[i] = key;
  }

  err = clock_gettime(CLOCK_REALTIME, &startts);
  assert(!err);
  std::sort(arr, arr+n);
  err = clock_gettime(CLOCK_REALTIME, &endts);
  assert(!err);

  start = startts.tv_sec;
  start += startts.tv_nsec / 1000000000.0;
  end = endts.tv_sec;
  end += endts.tv_nsec / 1000000000.0;

  printf("Sorted %i elements in %f seconds\n", n, end-start);
  printf("%f MiB/s\n", 8*n / (end-start) / 1024.0 / 1024.0);

  return 0;
}
