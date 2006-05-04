#ifndef _chplrt_H_
#define _chplrt_H_

#define array_index(x, i) ((x)->_data[i])
#define array_set(x, i, v) ((x)->_data[i] = v)
#define array_init(type, x, size, v)                            \
  {                                                             \
    (x)->_data = _chpl_malloc(size, sizeof(type), "_data");     \
    int _a_i;                                                   \
    for (_a_i = 0; _a_i < size; _a_i++)                         \
      (x)->_data[_a_i] = v;                                     \
  }

#define _noop(x)

#define malloc  dont_use_malloc_use_chpl_malloc_instead
#define calloc  dont_use_calloc_use_chpl_calloc_instead
#define free    dont_use_free_use_chpl_free_instead
#define realloc dont_use_realloc_use_chpl_realloc_instead

#endif
