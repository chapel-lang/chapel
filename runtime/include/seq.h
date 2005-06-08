#ifndef _seq_H_
#define _seq_H_

#define _FOR_S(e, s, type) \
  type __tmp; \
  for (__tmp = s->_first; (__tmp != nil && (e = __tmp->_element)); __tmp = __tmp->_next)

#endif
