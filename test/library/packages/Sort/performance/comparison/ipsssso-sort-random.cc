// git clone https://github.com/SaschaWitt/ips4o
// g++ -O3 ipsssso-sort-random.cc  -I ips4o/ -fopenmp -march=native -latomic

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cinttypes>
#include <cassert>
#include <algorithm>
#include "ips4o.hpp"

int main() {

  int err = 0;
  int n = 1024*1024*128;
  uint64_t* arr = (uint64_t*) malloc(n*sizeof(uint64_t));
  uint64_t* arr2 = (uint64_t*) malloc(n*sizeof(uint64_t));
  //uint32_t* arr = (uint32_t*) malloc(n*sizeof(uint32_t));
  //uint32_t* arr2 = (uint32_t*) malloc(n*sizeof(uint32_t));
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
    arr2[i] = key;
  }

  err = clock_gettime(CLOCK_REALTIME, &startts);
  assert(!err);
  ips4o::parallel::sort(arr, arr+n);
  err = clock_gettime(CLOCK_REALTIME, &endts);
  assert(!err);

  start = startts.tv_sec;
  start += startts.tv_nsec / 1000000000.0;
  end = endts.tv_sec;
  end += endts.tv_nsec / 1000000000.0;

  printf("Sorted %i elements in %f seconds\n", n, end-start);
  printf("%f MiB/s\n", sizeof(*arr) *n / (end-start) / 1024.0 / 1024.0);

  std::sort(arr2, arr2+n);
  for (int i = 0; i < n-1; i++) {
    assert(arr[i] <= arr[i+1]);
    assert(arr[i] == arr2[i]);
  }
  return 0;
}
