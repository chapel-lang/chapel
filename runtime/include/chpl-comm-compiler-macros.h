#ifndef _chpl_comm_compiler_macros_h_
#define _chpl_comm_compiler_macros_h_

#ifndef LAUNCHER

//
// Multi-locale macros used for compiler code generation
//
// Note: Macros starting with CHPL_COMM involve some kind of communication
//

// Storing the node field in a wide pointer allows the comm routines to find
// the node on which the data are stored.  Storing the sublocale index, however,
// is non-essential.  It is an "optimization" which is somewhat questionable:
//
// Use of this field is apt to be fairly rare, so computing and storing it in
// every wide pointer will hurt performance.  An alternative approach would be
// to keep a map which supports converting an object address (within a node)
// back into the index of the sublocale that allocated it.
#define CHPL_WIDEN(wide, ref)                               \
  do {                                                      \
    (wide).locale.node = chpl_localeID;                     \
    (wide).locale.subloc = chpl_task_getSubLoc();           \
    (wide).addr = (ref);                                    \
  } while (0)

// This version sets the sublocale field to zero (useful during initialization).
#define CHPL_WIDEN_TO_ROOT(wide, ref)                                 \
  do {                                                                \
    (wide).locale.node = 0;                                           \
    (wide).locale.subloc = 0;                                         \
    (wide).addr = (ref);                                              \
  } while (0)

#define CHPL_WIDEN_NULL(wide)                   \
  CHPL_WIDEN_TO_ROOT(wide, NULL)

#define CHPL_WIDEN_STRING(wide, str)                                    \
  do {                                                                  \
    const char* chpl_macro_tmp = str;                                   \
    size_t chpl_macro_len = strlen(chpl_macro_tmp) + 1;                 \
    (wide).locale.node = chpl_localeID;                                 \
    (wide).locale.subloc = chpl_task_getSubLoc();                       \
    (wide).addr = chpl_mem_allocMany(chpl_macro_len, sizeof(char),      \
                                     CHPL_RT_MD_SET_WIDE_STRING, 0, 0); \
    strncpy((char*)(wide).addr, chpl_macro_tmp, chpl_macro_len);        \
    (wide).size = chpl_macro_len;                                       \
  } while (0)

#define CHPL_WIDEN_STRING_TO_ROOT(wide, str)                            \
  do {                                                                  \
    const char* chpl_macro_tmp = str;                                   \
    size_t chpl_macro_len = strlen(chpl_macro_tmp) + 1;                 \
    (wide).locale.node = 0;                                             \
    (wide).locale.subloc = 0;                                           \
    (wide).addr = chpl_mem_allocMany(chpl_macro_len, sizeof(char),      \
                                     CHPL_RT_MD_SET_WIDE_STRING, 0, 0); \
    strncpy((char*)(wide).addr, chpl_macro_tmp, chpl_macro_len);        \
    (wide).size = chpl_macro_len;                                       \
  } while (0)

#define CHPL_NARROW(ref, wide)  \
  (ref) = (wide).addr

// Two wide addresses compare equal if the node-local addresses compare equal
// and either they are null or their node indices also compare equal.
// That is, a null pointer on one node compares equal to a null pointer on a different node,
// but all other wide pointers have to match exactly.
// Note that we ignore the sublocale index in the comparison.
// We assume that all sublocales on a given node share the same (flat) address space.
#define CHPL_WIDE_TEST_EQ(wide1, wide2)             \
  (((wide1).addr == (wide2).addr) &&                \
   ((wide1).addr == 0 ||                            \
    ((wide1).locale.node == (wide2).locale.node)))

#define CHPL_WIDE_TEST_NEQ(wide1, wide2)                                \
  (! CHPL_WIDE_TEST_EQ(wide1, wide2))
// Duh.

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
    (wide).locale.node = chpl_localeID;                                 \
    (wide).locale.subloc = 0;                                           \
    (wide).addr = chpl_getPrivatizedClass(id);                          \
  } while (0)
// That the sublocale ID is set to zero does not matter, we hope.
// If it does matter, we have to increase the width of the privatized class table,
// and store the sublocale ID too.

#ifdef CHPL_TASK_COMM_GET
#define CHPL_COMM_GET(localvar, node, addr, type, tid, len, ln, fn)     \
  chpl_task_comm_get((void*)(&(localvar)), node, (void*)addr, sizeof(type), tid, len, ln, fn)
#else
#define CHPL_COMM_GET(localvar, node, addr, type, tid, len, ln, fn)     \
  chpl_comm_get((void*)(&(localvar)), node, (void*)addr, sizeof(type), tid, len, ln, fn)
#endif

#ifdef CHPL_TASK_COMM_PUT
#define CHPL_COMM_PUT(localvar, node, addr, type, tid, len, ln, fn)     \
  chpl_task_comm_put((void*)(&(localvar)), node, (void*)addr, sizeof(type), tid, len, ln, fn)
#else
#define CHPL_COMM_PUT(localvar, node, addr, type, tid, len, ln, fn)     \
  chpl_comm_put((void*)(&(localvar)), node, (void*)addr, sizeof(type), tid, len, ln, fn)
#endif

#define CHPL_COMM_WIDE_GET(local, wide, type, tid, len, ln, fn)         \
  do {                                                                  \
    if (chpl_localeID == (wide).locale.node)                            \
      memcpy(&local, (wide).addr, len*sizeof(type) );                   \
    else                                                                \
      CHPL_COMM_GET(local, (wide).locale.node, (wide).addr,             \
                    type, tid, len, ln, fn);                            \
  } while (0)

// Extracts the locale field from a wide pointer.
// The result is of type c_locale_t.
#define CHPL_COMM_WIDE_GET_LOCALE(local, wide, type, tid, ln, fn)       \
  do {                                                                  \
    if (chpl_localeID == (wide).locale.node)                            \
      local = (wide).addr->locale;                                      \
    else                                                                \
      CHPL_COMM_GET(local, (wide).locale.node, (wide).addr,             \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

// Extracts just the node ID from the wide pointer.
// The result is the type of c_locale_t.node, which is currently int32_t.
#define CHPL_COMM_WIDE_GET_NODE(local, wide, type, tid, ln, fn)         \
  do {                                                                  \
    if (chpl_localeID == (wide).locale.node)                            \
      local = (wide).addr->locale.node;                                 \
    else                                                                \
      CHPL_COMM_GET(local, (wide).locale.node, (wide).addr,             \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

// Extracts just the sublocale ID from the wide pointer.
// The result is the type of c_locale_t.subloc, which is currently int32_t.
#define CHPL_COMM_WIDE_GET_SUBLOC(local, wide, type, tid, ln, fn)       \
  do {                                                                  \
    if (chpl_localeID == (wide).locale.node)                            \
      local = (wide).addr->locale.subloc;                               \
    else                                                                \
      CHPL_COMM_GET(local, (wide).locale.node, (wide).addr,             \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_PUT(type, tid, len, wide, local, ln, fn)         \
  do {                                                                  \
    type chpl_macro_tmp2 = local;                                       \
    if (chpl_localeID == (wide).locale.node)                            \
      memcpy((wide).addr, &chpl_macro_tmp2, len*sizeof(type) );         \
    else                                                                \
      CHPL_COMM_PUT(chpl_macro_tmp2, (wide).locale.node,                \
                    (wide).addr, type, tid, len, ln, fn);               \
  } while (0)

#define CHPL_COMM_WIDE_PUT_SVEC(type, tid, len, wide, local, ln, fn)    \
  do {                                                                  \
    if (chpl_localeID == (wide).locale.node)                            \
      memcpy((wide).addr, &local, len*sizeof(type));                    \
    else                                                                \
      CHPL_COMM_PUT(local, (wide).locale.node, (wide).addr,             \
                    type, tid, len, ln, fn);                            \
  } while (0)

#define CHPL_COMM_WIDE_GET_STRING(local, wide, tid, ln, fn)             \
  do {                                                                  \
    char* chpl_macro_tmp =                                              \
      chpl_mem_allocMany((wide).size, sizeof(char),                     \
                         CHPL_RT_MD_GET_WIDE_STRING, -1, "<internal>"); \
    if (chpl_localeID == (wide).locale.node)                            \
      memcpy(chpl_macro_tmp, (wide).addr, (wide).size);                 \
    else                                                                \
      CHPL_COMM_GET(*chpl_macro_tmp, (wide).locale.node,                \
                    ((void*)(wide).addr),                               \
                    char, tid, (wide).size, ln, fn);                    \
    local = chpl_macro_tmp;                                             \
  } while (0)

#define CHPL_WIDE_GET_FIELD(wide1, wide2, stype, sfield)                \
  do {                                                                  \
    (wide1).locale.node = (wide2).locale.node;                          \
    (wide1).locale.subloc = (wide2).locale.subloc;                      \
    (wide1).addr = &((stype)((wide2).addr))->sfield;                    \
  } while (0)

#define CHPL_WIDE_GET_TUPLE_COMPONENT(wide1, wide2,  index)             \
  do {                                                                  \
    (wide1).locale.node = (wide2).locale.node;                          \
    (wide1).locale.subloc = (wide2).locale.subloc;                      \
    (wide1).addr = &(*(wide2).addr)[index];                             \
  } while (0)

#define CHPL_COMM_WIDE_GET_FIELD_VALUE(local, wide, stype, sfield, type, tid, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale.node)                            \
      local = ((stype)((wide).addr))->sfield;                           \
    else                                                                \
      CHPL_COMM_GET(local,                                              \
                    (wide).locale.node,                                 \
                    &((stype)((wide).addr))->sfield,                    \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_GET_FIELD_VALUE_SVEC(local, wide, stype, sfield, type, tid, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale.node)                            \
      CHPL_ASSIGN_SVEC(local, ((stype)((wide).addr))->sfield);          \
    else                                                                \
      CHPL_COMM_GET(local,                                              \
                    (wide).locale.node,                                 \
                    &((stype)((wide).addr))->sfield,                    \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_GET_TUPLE_COMPONENT_VALUE(local, wide, index, type, tid, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale.node)                            \
      local = (*(wide).addr)[index];                                    \
    else                                                                \
      CHPL_COMM_GET(local,                                              \
                    (wide).locale.node,                                 \
                    &(*(wide).addr)[index],                             \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_GET_TUPLE_COMPONENT_VALUE_SVEC(local, wide, index, type, tid, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale.node)                            \
      CHPL_ASSIGN_SVEC(local, (*(wide).addr)[index]);                   \
    else                                                                \
      CHPL_COMM_GET(local,                                              \
                    (wide).locale.node,                                 \
                    &(*(wide).addr)[index],                             \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_SET_FIELD_VALUE(type, tid, wide, local, stype, sfield, ln, fn) \
  do {                                                                  \
    type chpl_macro_tmp = local;                                        \
    if (chpl_localeID == (wide).locale.node)                            \
      ((stype)((wide).addr))->sfield = chpl_macro_tmp;                  \
    else                                                                \
      CHPL_COMM_PUT(chpl_macro_tmp, (wide).locale.node,                 \
                    &((stype)((wide).addr))->sfield,                    \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_SET_FIELD_VALUE_SVEC(type, tid, wide, local, stype, sfield, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale.node)                            \
      CHPL_ASSIGN_SVEC(((stype)((wide).addr))->sfield, local);          \
    else                                                                \
      CHPL_COMM_PUT(local,                                              \
                    (wide).locale.node,                                 \
                    &((stype)((wide).addr))->sfield,                    \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_SET_TUPLE_COMPONENT_VALUE(type, tid, wide, local, stype, index, ln, fn) \
  do {                                                                  \
    type chpl_macro_tmp = local;                                        \
    if (chpl_localeID == (wide).locale.node)                            \
      (*(wide).addr)[index] = chpl_macro_tmp;                           \
    else                                                                \
      CHPL_COMM_PUT(chpl_macro_tmp,                                     \
                    (wide).locale.node,                                 \
                    &(*(wide).addr)[index],                             \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_SET_TUPLE_COMPONENT_VALUE_SVEC(type, tid, wide, local, stype, index, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale.node)                            \
      CHPL_ASSIGN_SVEC((*(wide).addr)[index], local);                   \
    else                                                                \
      CHPL_COMM_PUT(local,                                              \
                    (wide).locale.node,                                 \
                    &(*(wide).addr)[index],                             \
                    type, tid, 1 /*length*/, ln, fn);                   \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_GET(wide, cls, ind, ln, fn)                \
  do {                                                                  \
    (wide).locale.node = (cls).locale.node;                             \
    (wide).locale.subloc = (cls).locale.subloc;                         \
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
    (local).locale.node = (wide).locale.node;                           \
    (local).locale.subloc = (wide).locale.subloc;                       \
    (local).addr = (type)((wide).addr);                                 \
  } while (0)
// Since they were allocated as a unit, we can assume that the superclass
// (i.e. the base-class sub-object of this object) lies in the same sublocale.

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
    if ((wide).locale.node != chpl_localeID)                            \
      chpl_error(str, ln, fn);                                          \
  } while (0)

// Globals reside on the very special {root,root} locale.
// Sublocale 0 on any node is assumed to contain all of the resources of that node.
#define CHPL_HEAP_REGISTER_GLOBAL_VAR(i, wide)                      \
  do {                                                              \
    (wide).locale.node = 0;                                         \
    (wide).locale.subloc = 0;                                       \
    chpl_globals_registry[i] = (&((wide).addr));                    \
    CHPL_HEAP_REGISTER_GLOBAL_VAR_EXTRA(i, wide)                    \
  } while (0)

//
// If we're in serial mode, we should use blocking rather than
// non-blocking comm forks in order to serialize the forks.
// See test/parallel/serial/bradc/serialDistributedForall.chpl
// for a motivating example that didn't work before this change.
//

// Assume that we've already added a setSubLoc() call to the body of the ON_BLOCK, 
// so it suffices to pass in just the node ID portion of the locale structure.
#define CHPL_COMM_NONBLOCKING_ON(node, fid, arg, arg_size, arg_tid)   \
  if (chpl_task_getSerial()) {                                        \
    chpl_comm_fork(node, fid, arg, arg_size, arg_tid);                \
  } else {                                                            \
    chpl_comm_fork_nb(node, fid, arg, arg_size, arg_tid);             \
  }

#ifdef DEBUG_COMM_INIT
#define CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals)   \
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
