#ifndef _chpl_comm_compiler_macros_h_
#define _chpl_comm_compiler_macros_h_

#ifndef LAUNCHER

//
// Multi-locale macros used for compiler code generation
//
// Note: Macros starting with CHPL_COMM involve some kind of communication
//

#define CHPL_WIDEN(wide, ref)                   \
  do {                                          \
    (wide).locale = chpl_localeID;              \
    (wide).addr = (ref);                        \
  } while (0)

#define CHPL_WIDEN_NULL(wide)                   \
  do {                                          \
    (wide).locale = 0;                          \
    (wide).addr = NULL;                         \
  } while (0)

#define CHPL_WIDEN_STRING(wide, str)                                    \
  do {                                                                  \
    const char* chpl_macro_tmp = str;                                   \
    size_t chpl_macro_len = strlen(chpl_macro_tmp) + 1;                 \
    (wide).locale = chpl_localeID;                                      \
    (wide).addr = chpl_mem_allocMany(chpl_macro_len, sizeof(char),      \
                                     CHPL_RT_MD_SET_WIDE_STRING, 0, 0); \
    strncpy((char*)(wide).addr, chpl_macro_tmp, chpl_macro_len);        \
    (wide).size = chpl_macro_len;                                       \
  } while (0)

#define CHPL_NARROW(ref, wide)                  \
  (ref) = (wide).addr

#define CHPL_WIDE_TEST_NEQ(wide1, wide2)                                \
  (((wide1).addr != (wide2).addr) ||                                    \
   (((wide1).addr != 0) && ((wide1).locale != (wide2).locale)))

#define CHPL_WIDE_TEST_EQ(wide1, wide2)         \
  (!CHPL_WIDE_TEST_NEQ(wide1, wide2))

#define CHPL_WIDE_CAST(wide1, type, wide2)                              \
  do {                                                                  \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = (type)((wide2).addr);                                \
  } while (0)

#define CHPL_COMM_WIDE_DYNAMIC_CAST(wide1, type, cond, wide2, ln, fn)   \
  do {                                                                  \
    chpl__class_id chpl_macro_tmp;                                      \
    CHPL_COMM_WIDE_GET_FIELD_VALUE(chpl_macro_tmp, wide2,               \
                                   object,  chpl__cid,                  \
                                   chpl__class_id, -1, ln, fn);         \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = (cond) ? (type)((wide2).addr) : NULL;                \
  } while (0)

#define CHPL_WIDE_GET_PRIVATIZED_CLASS(wide, id)                        \
  do {                                                                  \
    (wide).locale = chpl_localeID;                                      \
    (wide).addr = chpl_getPrivatizedClass(id);                          \
  } while (0)


#ifdef CHPL_TASK_COMM_GET
#define CHPL_COMM_GET(localvar, locale, addr, type, tid, len, ln, fn)  \
  CHPL_TASK_COMM_GET(localvar, locale, (addr), type, tid, len, ln, fn)
#else
#define CHPL_COMM_GET(localvar, locale, addr, type, tid, len, ln, fn)  \
  chpl_comm_get((void*)(&(localvar)), locale, (void*)addr, sizeof(type), tid, len, ln, fn)
#endif

#ifdef CHPL_TASK_COMM_PUT
#define CHPL_COMM_PUT(localvar, locale, addr, type, tid, len, ln, fn)  \
  CHPL_TASK_COMM_PUT(localvar, locale, (addr), tid, sizeof(type) len, ln, fn)
#else
#define CHPL_COMM_PUT(localvar, locale, addr, type, tid, len, ln, fn)  \
  chpl_comm_put((void*)(&(localvar)), locale, (void*)addr, sizeof(type), tid, len, ln, fn)
#endif

#ifdef CHPL_TASK_COMM_GETS
#define CHPL_COMM_GETS(localvar, dststr, locale, raddr, srcstr, count, strlevels, type, tid, ln, fn) \
  CHPL_TASK_COMM_GETS(localvar, dststr, locale, (raddr), srcstr, count, strlevels, type, tid, ln, fn)
#else
#define CHPL_COMM_GETS(localvar, dststr, locale, raddr, srcstr, count, strlevels, type, tid, ln, fn) \
  chpl_comm_gets((void*)(&(localvar)), (void*)(&(dststr)), locale, (void*)raddr, (void*)(&srcstr), (void*)(&count), strlevels, sizeof(type), tid, ln, fn)
#endif

#ifdef CHPL_TASK_COMM_PUTS
#define CHPL_COMM_PUTS(localvar, dststr, locale, raddr, srcstr, count, strlevels, type, tid, ln, fn) \
  CHPL_TASK_COMM_PUTS(localvar, dststr, locale, (raddr), srcstr, count, strlevels, type, tid, ln, fn)
#else
#define CHPL_COMM_PUTS(localvar, dststr, locale, raddr, srcstr, count, strlevels, type, tid, ln, fn) \
  chpl_comm_puts((void*)(&(localvar)), (void*)(&(dststr)), locale, (void*)raddr, (void*)(&srcstr), (void*)(&count), strlevels, sizeof(type), tid, ln, fn)
#endif


#define CHPL_COMM_WIDE_GET(local, wide, type, tid, len, ln, fn)  \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      memcpy(&local, (wide).addr, len*sizeof(type) );                   \
    else                                                                \
      CHPL_COMM_GET(local, (wide).locale, (wide).addr,                  \
                    type, tid, len, ln, fn);                            \
  } while (0)

#define CHPL_COMM_WIDE_GET_LOCALE(local, wide, type, tid, ln, fn)       \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      local = (wide).addr->locale;                                      \
    else                                                                \
      CHPL_COMM_GET(local, (wide).locale, (wide).addr,                  \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_PUT(type, tid, len, wide, local, ln, fn)         \
  do {                                                                  \
    type chpl_macro_tmp2 = local;                                       \
    if (chpl_localeID == (wide).locale)                                 \
      memcpy((wide).addr, &chpl_macro_tmp2, len*sizeof(type) );         \
    else                                                                \
      CHPL_COMM_PUT(chpl_macro_tmp2, (wide).locale, (wide).addr,        \
                    type, tid, len, ln, fn);                            \
  } while (0)

#define CHPL_COMM_WIDE_PUT_SVEC(type, tid, len, wide, local, ln, fn)    \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      memcpy((wide).addr, &local, len*sizeof(type));                    \
    else                                                                \
      CHPL_COMM_PUT(local, (wide).locale, (wide).addr,                  \
                    type, tid, len, ln, fn);                            \
  } while (0)

#define CHPL_COMM_WIDE_GET_STRING(local, wide, tid, ln, fn)             \
  do {                                                                  \
    char* chpl_macro_tmp =                                              \
      chpl_mem_allocMany((wide).size, sizeof(char),                     \
                         CHPL_RT_MD_GET_WIDE_STRING, -1, "<internal>"); \
    if (chpl_localeID == (wide).locale)                                 \
      memcpy(chpl_macro_tmp, (wide).addr, (wide).size);                 \
    else                                                                \
      CHPL_COMM_GET(*chpl_macro_tmp, (wide).locale,                     \
                    ((void*)(wide).addr),                               \
                    char, tid, (wide).size, ln, fn);                    \
    local = chpl_macro_tmp;                                             \
  } while (0)

#define CHPL_WIDE_GET_FIELD(wide1, wide2, stype, sfield)                \
  do {                                                                  \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = &((stype)((wide2).addr))->sfield;                    \
  } while (0)

#define CHPL_WIDE_GET_TUPLE_COMPONENT(wide1, wide2,  index)             \
  do {                                                                  \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = &(*(wide2).addr)[index];                             \
  } while (0)

#define CHPL_COMM_WIDE_GET_FIELD_VALUE(local, wide, stype, sfield, type, tid, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      local = ((stype)((wide).addr))->sfield;                           \
    else                                                                \
      CHPL_COMM_GET(local,                                              \
                    (wide).locale,                                      \
                    &((stype)((wide).addr))->sfield,                    \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_GET_FIELD_VALUE_SVEC(local, wide, stype, sfield, type, tid, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      CHPL_ASSIGN_SVEC(local, ((stype)((wide).addr))->sfield);          \
    else                                                                \
      CHPL_COMM_GET(local,                                              \
                    (wide).locale,                                      \
                    &((stype)((wide).addr))->sfield,                    \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_GET_TUPLE_COMPONENT_VALUE(local, wide, index, type, tid, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      local = (*(wide).addr)[index];                                    \
    else                                                                \
      CHPL_COMM_GET(local,                                              \
                    (wide).locale,                                      \
                    &(*(wide).addr)[index],                             \
                    type, tid, 1 /*length*/, ln, fn);                    \
  } while (0)

#define CHPL_COMM_WIDE_GET_TUPLE_COMPONENT_VALUE_SVEC(local, wide, index, type, tid, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      CHPL_ASSIGN_SVEC(local, (*(wide).addr)[index]);                   \
    else                                                                \
      CHPL_COMM_GET(local,                                              \
                    (wide).locale,                                      \
                    &(*(wide).addr)[index],                             \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_SET_FIELD_VALUE(type, tid, wide, local, stype, sfield, ln, fn) \
  do {                                                                  \
    type chpl_macro_tmp = local;                                        \
    if (chpl_localeID == (wide).locale)                                 \
      ((stype)((wide).addr))->sfield = chpl_macro_tmp;                  \
    else                                                                \
      CHPL_COMM_PUT(chpl_macro_tmp,                                     \
                    (wide).locale, &((stype)((wide).addr))->sfield,     \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_SET_FIELD_VALUE_SVEC(type, tid, wide, local, stype, sfield, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      CHPL_ASSIGN_SVEC(((stype)((wide).addr))->sfield, local);          \
    else                                                                \
      CHPL_COMM_PUT(local,                                              \
                    (wide).locale,                                      \
                    &((stype)((wide).addr))->sfield,                    \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_SET_TUPLE_COMPONENT_VALUE(type, tid, wide, local, stype, index, ln, fn) \
  do {                                                                  \
    type chpl_macro_tmp = local;                                        \
    if (chpl_localeID == (wide).locale)                                 \
      (*(wide).addr)[index] = chpl_macro_tmp;                           \
    else                                                                \
      CHPL_COMM_PUT(chpl_macro_tmp,                                     \
                    (wide).locale,                                      \
                    &(*(wide).addr)[index],                             \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_SET_TUPLE_COMPONENT_VALUE_SVEC(type, tid, wide, local, stype, index, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      CHPL_ASSIGN_SVEC((*(wide).addr)[index], local);                   \
    else                                                                \
      CHPL_COMM_PUT(local,                                              \
                    (wide).locale,                                      \
                    &(*(wide).addr)[index],                             \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_GET(wide, cls, ind, ln, fn)                \
  do {                                                                  \
    (wide).locale = (cls).locale;                                       \
    (wide).addr = (cls).addr + ind;                                     \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_GET_VALUE(wide_type, local, cls, ind, etype2, etid2, ln, fn) \
  do {                                                                  \
    wide_type chpl_macro_tmp;                                           \
    CHPL_COMM_WIDE_ARRAY_GET(chpl_macro_tmp, cls, ind, ln, fn);         \
    CHPL_COMM_WIDE_GET(local, chpl_macro_tmp, etype2, etid2, 1, ln, fn);\
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_SET_VALUE(wide_type, cls, ind, etype, etid, val, ln, fn) \
  do {                                                                  \
    wide_type chpl_macro_tmp;                                           \
    CHPL_COMM_WIDE_ARRAY_GET(chpl_macro_tmp, cls, ind, ln, fn);         \
    CHPL_COMM_WIDE_PUT(etype, etid, 1, chpl_macro_tmp, val, ln, fn);    \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_SET_VALUE_SVEC(wide_type, cls, ind, etype, etid, val, ln, fn) \
  do {                                                                    \
    wide_type chpl_macro_tmp;                                             \
    CHPL_COMM_WIDE_ARRAY_GET(chpl_macro_tmp, cls, ind, ln, fn);           \
    CHPL_COMM_WIDE_PUT_SVEC(etype, etid, 1, chpl_macro_tmp, val, ln, fn); \
  } while (0)

#define CHPL_COMM_WIDE_CLASS_GET_CID(local, wide, stype, type, tid, ln, fn) \
  CHPL_COMM_WIDE_GET_FIELD_VALUE(local, wide,                           \
                                 stype, chpl__cid,                      \
                                 type, tid, ln, fn)

#define CHPL_WIDE_CLASS_GET_SUPER(type, local, wide)                    \
  do {                                                                  \
    (local).locale = (wide).locale;                                     \
    (local).addr = (type)((wide).addr);                                 \
  } while (0)

#define CHPL_COMM_WIDE_CLASS_TEST_CID(local, wide, cid, stype, type, tid, ln, fn) \
  do {                                                                  \
    chpl__class_id chpl_macro_tmp;                                      \
    CHPL_COMM_WIDE_GET_FIELD_VALUE(chpl_macro_tmp, wide,                \
                                   stype, chpl__cid,                    \
                                   type, tid, ln, fn);                  \
    local = chpl_macro_tmp == cid;                                      \
  } while (0)

#define CHPL_TEST_LOCAL(wide, ln, fn, str)                              \
  do {                                                                  \
    if ((wide).locale != chpl_localeID)                                 \
      chpl_error(str, ln, fn);                                          \
  } while (0)

#define CHPL_HEAP_REGISTER_GLOBAL_VAR(i, wide)            \
  do {                                                    \
    (wide).locale = 0;                                    \
    chpl_globals_registry[i] = (&((wide).addr));          \
    CHPL_HEAP_REGISTER_GLOBAL_VAR_EXTRA(i, wide)          \
  } while (0)

//
// If we're in serial mode, we should use blocking rather than
// non-blocking comm forks in order to serialize the forks.
// See test/parallel/serial/bradc/serialDistributedForall.chpl
// for a motivating example that didn't work before this change.
//
#define CHPL_COMM_NONBLOCKING_ON(locale, fid, arg, arg_size, arg_tid) \
  if (chpl_task_getSerial()) {                                        \
    chpl_comm_fork(locale, fid, arg, arg_size, arg_tid);              \
  } else {                                                            \
    chpl_comm_fork_nb(locale, fid, arg, arg_size, arg_tid);           \
  }

#ifdef DEBUG_COMM_INIT
#define CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals) \
  for (int i = 0; i < numGlobals; i++) \
    printf("[%d] chpl_globals_registry[%d] = %p\n", chpl_localeID, i, *((void **)chpl_globals_registry[i]));
#else
#define CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals) ;
#endif

#define CHPL_COMM_BROADCAST_GLOBAL_VARS(numGlobals)             \
  do {                                                          \
    chpl_comm_barrier("barrier before broadcasting globals");   \
    chpl_comm_broadcast_global_vars(numGlobals);                \
    CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals);          \
    chpl_comm_barrier("barrier after broadcasting globals");    \
  } while (0)

#endif // LAUNCHER

#endif
