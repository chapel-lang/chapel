#ifndef _chpl_wide_ptr_impl_h_
#define _chpl_wide_ptr_impl_h_

// Make sure that chpltypes will define a wide pointer type.
#ifdef CHPL_WIDE_POINTER_STRUCT
#else

#ifdef CHPL_WIDE_POINTER_PACKED
#else
#error missing either CHPL_WIDE_POINTER_STRUCT or CHPL_WIDE_POINTER_PACKED
#endif

#endif


#include "chpltypes.h"

// chpl_error needed for some of the packed wide pointer error cases.
#include "error.h"


// Methods for working with wide pointers (packed or not) and localeID_t.
// There are several variants of many of these in order to simplify
// different code generation tasks. The versions that take in a pointer
// to a wide_ptr_t generally do so in order to easily be used with
// a variety of wide pointer types with the structure representation,
// since one can't cast a structure...

static ___always_inline
c_subloc_t chpl_local_ptr_to_subloc(void* ptr)
{
  // This should be platform specific.
  return 0;
}

#ifdef CHPL_WIDE_POINTER_PACKED
  #define CHPL_WIDE_PTR_MARK_BITS 1
  #define CHPL_WIDE_PTR_MARK 1

  #define CHPL_PTR_BITS (64-CHPL_WIDE_PTR_MARK_BITS-CHPL_WIDE_POINTER_NODE_BITS)

  #define CHPL_NODE_MASK ( (1ULL << CHPL_WIDE_POINTER_NODE_BITS) - 1ULL)
  #define CHPL_PTR_MASK ( (1ULL << CHPL_PTR_BITS) - 1ULL)
#endif

static ___always_inline
void chpl_build_wide_ptr_node(wide_ptr_t* dst, c_nodeid_t node, void * addr)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  dst->locale.node = node;
  dst->locale.subloc = chpl_local_ptr_to_subloc(addr);
  dst->addr = addr;
#else
  uint64_t uptr = (uint64_t) addr;
  uint64_t unode = node;
  uint64_t mark = CHPL_WIDE_PTR_MARK;
  uint64_t ret;
  if( (uptr & CHPL_PTR_MASK) != uptr ) {
    chpl_internal_error("Local pointer too big to fit into wide pointer");
  }
  if( (unode & CHPL_NODE_MASK) != unode) {
    chpl_internal_error("Node ID too big to fit into wide pointer");
  }
  ret = uptr;
  ret |= unode << CHPL_PTR_BITS;
  ret |= mark << (CHPL_PTR_BITS+CHPL_WIDE_POINTER_NODE_BITS);
  *dst = (wide_ptr_t) ret;
#endif
}

static ___always_inline
void chpl_check_wide_ptr(const wide_ptr_t* ptr)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  if( ptr->locale.node < 0 )
    chpl_internal_error("Bad wide pointer");
#else
  uint64_t uptr = (uint64_t) *ptr;
  uint64_t mark = uptr >> (CHPL_PTR_BITS+CHPL_WIDE_POINTER_NODE_BITS);
  if( uptr == 0 ) return; // NULL is always OK
  if( mark != CHPL_WIDE_PTR_MARK )
    chpl_internal_error("Bad wide pointer");
#endif
}

static ___always_inline
void chpl_build_wide_ptr_loc_ptr(wide_ptr_t* dst, const chpl_localeID_t* loc, void * addr)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  dst->locale.node = loc->node;
  dst->locale.subloc = loc->subloc;
  dst->addr = addr;
#else
  // packed wide pointers do not store sublocale, so we just throw that info away
  // and reconstruct it later from the local address.
  chpl_build_wide_ptr_node(dst, loc->node, addr); 
#endif
}

wide_ptr_t chpl_return_wide_ptr_loc_sym(const chpl_localeID_t* loc, void * addr);

static ___always_inline
c_nodeid_t chpl_localeID_ptr_get_node(const chpl_localeID_t *loc)
{
  return loc->node;
}

static ___always_inline
c_subloc_t chpl_localeID_ptr_get_subloc(const chpl_localeID_t *loc)
{
  return loc->subloc;
}

static ___always_inline
void chpl_build_localeID_node(chpl_localeID_t *loc, c_nodeid_t node)
{
  loc->node = node;
  loc->subloc = 0;
}

static ___always_inline
void chpl_build_localeID(chpl_localeID_t *loc, c_nodeid_t node, c_subloc_t subloc)
{
  loc->node = node;
  loc->subloc = subloc;
}

static ___always_inline
c_nodeid_t chpl_wide_ptr_ptr_get_node(const wide_ptr_t* ptr)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  chpl_check_wide_ptr(ptr);
  return ptr->locale.node;
#else
  uint64_t uptr = (uint64_t) *ptr;
  chpl_check_wide_ptr(ptr);
  return (uptr >> CHPL_PTR_BITS) & CHPL_NODE_MASK;
#endif
}

c_nodeid_t chpl_wide_ptr_get_node_sym(wide_ptr_t ptr);

static ___always_inline
void* chpl_wide_ptr_ptr_get_address(const wide_ptr_t* ptr)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  chpl_check_wide_ptr(ptr);
  return ptr->addr;
#else
  uint64_t uptr = (uint64_t) *ptr;
  chpl_check_wide_ptr(ptr);
  uptr &= CHPL_PTR_MASK;
  return (void*) uptr;
#endif
}

void* chpl_wide_ptr_get_address_sym(wide_ptr_t ptr);

/*
static ___always_inline
c_subloc_t chpl_wide_ptr_get_subloc(const wide_ptr_t* ptr)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  return ptr->locale.subloc;
#else
  // packed wide pointers do not store sublocale - reconstruct it from the address
  return chpl_local_ptr_to_subloc(chpl_wide_ptr_get_address(ptr));
#endif
}
*/

static ___always_inline
void chpl_wide_ptr_ptr_read_localeID(const wide_ptr_t* ptr, chpl_localeID_t* loc)
{
 chpl_check_wide_ptr(ptr);
#ifndef CHPL_WIDE_POINTER_PACKED
  *loc = ptr->locale;
#else
  loc->node = chpl_wide_ptr_ptr_get_node(ptr);
  // packed wide pointers do not store sublocale - reconstruct from the address
  loc->subloc = chpl_local_ptr_to_subloc(chpl_wide_ptr_ptr_get_address(ptr));
#endif
}

void chpl_wide_ptr_read_localeID_sym(wide_ptr_t ptr, chpl_localeID_t* loc);

/*
static ___always_inline
void chpl_wide_ptr_copy(wide_ptr_t *dst, wide_ptr_t *src)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  dst->locale.node = src->locale.node;
  dst->locale.subloc = src->locale.subloc;
  dst->addr = src->addr;
#else
  *dst = *src;
#endif
}
*/

static ___always_inline
int32_t chpl_localeID_equals(chpl_localeID_t a, chpl_localeID_t b)
{
  c_nodeid_t anode, bnode;
  c_subloc_t asub, bsub;
  anode = chpl_localeID_ptr_get_node(&a);
  bnode = chpl_localeID_ptr_get_node(&b);
  asub = chpl_localeID_ptr_get_subloc(&a);
  bsub = chpl_localeID_ptr_get_subloc(&b);
  return anode == bnode && asub == bsub;
}

#endif
