#ifndef _seq_H_
#define _seq_H_

#define _FOR_S(e, s, type) \
  type __tmp; \
  for (__tmp = s->first; (__tmp != nil && (e = __tmp->element)); __tmp = __tmp->next)

#endif
