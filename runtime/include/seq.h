#ifndef _seq_H_
#define _seq_H_

#define _FOR_S(e, s, type, uid)                                         \
  type _##uid;                                                          \
  for (_##uid = s->_first, e = _##uid->_element;                        \
       _##uid != nil;                                                   \
       _##uid = _##uid->_next, e = (_##uid != nil) ? _##uid->_element : e)

#define _FORALL_S(e, s, type, uid)              \
  _FOR_S(e, s, type, uid)

#endif
