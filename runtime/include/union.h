#ifndef _union_H_
#define _union_H_

#define _UNION_SET(u, tag) \
  (u)._chpl_union_tag = (tag);

#define _UNION_CHECK(u, tag, filename, lineno) \
  if ((u)._chpl_union_tag != (tag)) { \
    printf("ERROR: Union type error (%s:%d)\n", filename, lineno); \
    exit(0); \
  }
 
#endif
