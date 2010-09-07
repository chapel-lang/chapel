#define CHPL_DUMMY_STM_LOAD_REF(tx, dst, src, type, ln, fn)	\
  dst = *(src)

#define CHPL_DUMMY_STM_LOAD(tx, dst, src, type, ln, fn)	\
  dst = src

#define CHPL_DUMMY_STM_LOAD_FIELD_VALUE(tx, dst, src, type, ln, fn)	\
  dst = src

#define CHPL_DUMMY_STM_LOAD_FIELD_VALUE_SVEC(tx, dst, src, type, ln, fn)      \
  CHPL_ASSIGN_SVEC(dst, src)

#define CHPL_DUMMY_STM_LOAD_TUPLE_COMPONENT_VALUE(tx, dst, src, type, ln, fn) \
  dst = src

#define CHPL_DUMMY_STM_LOAD_TUPLE_COMPONENT_VALUE_SVEC(tx, dst, src, type, ln, fn) \
  CHPL_ASSIGN_SVEC(dst, src)

#define CHPL_DUMMY_STM_ARRAY_LOAD(tx, dst, src, ind, type, ln, fn)	\
  dst = _ARRAY_GET(src, ind)

#define CHPL_DUMMY_STM_COMM_WIDE_GET(tx, local, wide, type, ln, fn) \
  CHPL_COMM_WIDE_GET(local, wide, type, ln, fn)

#define CHPL_DUMMY_STM_COMM_WIDE_GET_FIELD_VALUE(tx, local, wide, stype, sfield, type, ln, fn) \
  CHPL_COMM_WIDE_GET_FIELD_VALUE(local, wide, stype, sfield, type, ln, fn)

#define CHPL_DUMMY_STM_COMM_WIDE_GET_FIELD_VALUE_SVEC(tx, local, wide, stype, sfield, type, ln, fn) \
  CHPL_COMM_WIDE_GET_FIELD_VALUE_SVEC(local, wide, stype, sfield, type, ln, fn)

#define CHPL_DUMMY_STM_COMM_WIDE_ARRAY_GET(tx, wide, cls, ind, stype, sfield, etype, ln, fn) \
  CHPL_COMM_WIDE_ARRAY_GET(wide, cls, ind, stype, sfield, etype, ln, fn)

#define CHPL_DUMMY_STM_TX_ALLOC_PERMIT_ZERO(tx, size , description, ln, fn) \
  CHPL_ALLOC_PERMIT_ZERO(size, description, ln, fn)

