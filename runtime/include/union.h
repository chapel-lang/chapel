#ifndef _union_H_
#define _union_H_

#define _UNION_SET(u, tag) \
  (u)._chpl_union_tag = (tag);

#define _UNION_CHECK_QUIET(u, tag) \
  ((u)._chpl_union_tag == (tag))

#define _UNION_CHECK(u, tag, filename, lineno) \
  if (!_UNION_CHECK_QUIET(u, tag)) { \
    fflush(stdout); \
    fprintf(stderr, "ERROR: Union type error (%s:%d)\n", filename, lineno); \
    exit(0); \
  }

 
#endif
