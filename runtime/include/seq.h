#ifndef _seq_H_
#define _seq_H_

#define _SEQ_APPEND(s, t, type) \
  if (s->length == 0) { \
    s->last = s->first = (type##_node)_chpl_malloc(1, sizeof(_##type##_node), "seq element"); \
  } else { \
    s->last = s->last->next = (type##_node)_chpl_malloc(1, sizeof(_##type##_node), "seq element"); \
  } \
  s->last->element = t; \
  s->last->next = NULL; \
  s->length++

#define _FOR_S(e, s, type) \
  type##_node __tmp; \
  for (__tmp = s->first; (__tmp && (e = __tmp->element)); __tmp = __tmp->next)

#endif
