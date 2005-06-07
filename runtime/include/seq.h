#ifndef _seq_H_
#define _seq_H_

#define _FOR_S(e, s, type) \
  type __tmp; \
  for (__tmp = s->_chplSeqFirst; (__tmp != nil && (e = __tmp->_chplSeqElement)); __tmp = __tmp->_chplSeqNext)

#endif
