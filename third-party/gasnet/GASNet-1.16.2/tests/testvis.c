/*   $Source: /var/local/cvs/gasnet/tests/testvis.c,v $
 *     $Date: 2009/04/01 18:38:54 $
 * $Revision: 1.26 $
 * Description: GASNet Vector, Indexed & Strided correctness tests
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>
#include <gasnet_vis.h>
#include <gasnet_coll.h>

#ifndef TEST_SEGSZ
  /* select a larger than default segment, 
     because this test needs plenty of memory to play with */
  #define TEST_SEGSZ_EXPR          (16*1048576)
#endif
#include <test.h>

#ifndef MAX_VECLEN
#define MAX_VECLEN  100
#endif
#ifndef MAX_IDXLEN
#define MAX_IDXLEN  8192
#endif

/* VEC_SZ sets the size/offset alignment of all data accesses
   we support sizes 1, 4 and 8
   1 provides the most general test of aligned and unaligned accesses
   8 provides the best detection and diagnostics of erroneous data movements by the conduit
 */
#ifndef VEC_SZ
#define VEC_SZ 1
#endif
#if VEC_SZ == 8
  #define VEC_T       uint64_t
  #define SEG_VALUE(node, idx)   (((uint64_t)0x8000000000000000ull) | ((uint64_t)(node) << 32) | (uint64_t)(idx))
  #define HEAP_VALUE(node, idx)  (((uint64_t)0x0000000000000000ull) | ((uint64_t)(node) << 32) | (uint64_t)(idx))
  #define VEC_FMT      "0x%08x %08x"
  #define VEC_STR(val) (int)TEST_HIWORD(val), (int)TEST_LOWORD(val)
#elif VEC_SZ == 4
  #define VEC_T       uint32_t
  #define SEG_VALUE(node, idx)   (((uint32_t)0x8000000) | (((uint32_t)(node)&0xF) << 24) | (((uint32_t)(idx))&0x00FFFFFF))
  #define HEAP_VALUE(node, idx)  (((uint32_t)0x0000000) | (((uint32_t)(node)&0xF) << 24) | (((uint32_t)(idx))&0x00FFFFFF))
  #define VEC_FMT      "0x%08x"
  #define VEC_STR(val) (int)(val)
#elif VEC_SZ == 1
  #define VEC_T       uint8_t
  #define SEG_VALUE(node, idx)   (((uint8_t)0x80) | (((uint32_t)(node)&0x7) << 4) | (((uint8_t)(idx))&0xF))
  #define HEAP_VALUE(node, idx)  (((uint8_t)0x00) | (((uint32_t)(node)&0x7) << 4) | (((uint8_t)(idx))&0xF))
  #define VEC_FMT      "0x%02x"
  #define VEC_STR(val) (int)(val)
#else
  #error unknown VEC_SZ
#endif

#define NUM_AREAS   4
#ifndef MAX_STRIDEDIM
#define MAX_STRIDEDIM  20
#endif
#ifndef MAX_CHUNKSZ
#define MAX_CHUNKSZ 256
#endif
#ifndef MAX_INFLIGHT_OPS
#define MAX_INFLIGHT_OPS 16
#endif

#define RUN_VECTOR   1
#define RUN_INDEXED  2
#define RUN_STRIDED  4
#define RUN_NB       8

#if GASNET_SEGMENT_EVERYTHING
  #define segeverything 1
#else
  #define segeverything 0
#endif

/* shuffle the ordering in a list */
#define SHUFFLE_ARRAY(T, list, cnt) do {  \
    size_t _i;                            \
    for (_i=0;_i<(cnt);_i++) {            \
      size_t _a = TEST_RAND(_i, (cnt)-1); \
      T _tmp = (list)[_a];                \
      (list)[_a] = (list)[_i];            \
      (list)[_i] = _tmp;                  \
    }                                     \
  } while(0)

#define SHUFFLE_LIST(T, v) SHUFFLE_ARRAY(T, (v)->list, (v)->count)

int verify = 1;
int showtiming = 0;

/* ------------------------------------------------------------------------------------ */
typedef struct {
  uint64_t checksum;
  size_t count; /* in segments */
  uintptr_t totalsz; /* in bytes */
  gasnet_memvec_t *list;
} test_memvec_list;

void _verify_memvec_list(test_memvec_list *mv, const char *file, int line) {
  size_t i, sum;
  if (mv->checksum != test_checksum(mv->list, mv->count*sizeof(gasnet_memvec_t)))
    FATALERR("Checksum mismatch in verify_memvec_list at %s:%i", file, line);
  sum = 0;
  for (i=0; i < mv->count; i++) {
    sum += mv->list[i].len;
  }
  if (mv->totalsz != sum)
    FATALERR("totalsz mismatch in verify_memvec_list at %s:%i", file, line);
}
#define verify_memvec_list(mv) do {                            \
    if (verify) _verify_memvec_list((mv), __FILE__, __LINE__); \
  } while (0)

/* build a memvec over the area [addr...addr+len*VEC_SZ]
   note elemlen is a VEC_T element count
 */
test_memvec_list *rand_memvec_list(void *addr, size_t elemlen, int allowoverlap) {
  test_memvec_list *mv;
  size_t count = TEST_RAND_PICK(TEST_RAND(1, MIN(MAX_VECLEN,elemlen)),
                 TEST_RAND(1, TEST_RAND(1, TEST_RAND(1, MIN(MAX_VECLEN,elemlen)))));
  size_t per = 0;
  if (TEST_RAND_ONEIN(20)) count = 0;
  if (count > 0) per = elemlen / count; 
  mv = test_malloc(sizeof(test_memvec_list)+count*sizeof(gasnet_memvec_t));
  mv->count = count;
  mv->list = (gasnet_memvec_t *)(mv+1);
  mv->totalsz = 0;

  if (allowoverlap) {
    size_t i;
    for (i = 0; i < count; i++) {
      size_t offset = TEST_RAND(0, elemlen-1);
      size_t len = TEST_RAND(0, MIN(per,elemlen-offset));
      if (TEST_RAND_ONEIN(20)) {
        mv->list[i].addr = NULL;
        mv->list[i].len = 0;
      } else {
        mv->list[i].addr = ((VEC_T*)addr)+offset;
        mv->list[i].len = VEC_SZ*len;
        mv->totalsz += VEC_SZ*len;
      }
    }
  } else {
    /* build non-overlapping, monotonically increasing vectors */
    if (TEST_RAND_PICK(0,1)) {
      size_t i;
      size_t lim = 0;
      for (i = 0; i < count; i++) {
        if (TEST_RAND_ONEIN(20)) {
          mv->list[i].addr = NULL;
          mv->list[i].len = 0;
        } else {
          size_t offset = TEST_RAND(lim, per*(i+1)-1);
          size_t len = TEST_RAND(0, per*(i+1)-offset);
          mv->list[i].addr = ((VEC_T*)addr)+offset;
          mv->list[i].len = VEC_SZ*len;
          mv->totalsz += VEC_SZ*len;
          lim = offset + len;
          assert(lim <= per*(i+1) && lim <= elemlen);
        }
      }
    } else {
      size_t i;
      size_t lim = 0;
      for (i = 0; i < count; i++) {
        if (TEST_RAND_ONEIN(20)) {
          mv->list[i].addr = NULL;
          mv->list[i].len = 0;
        } else {
          size_t offset = TEST_RAND(lim, lim+(elemlen-lim)/4);
          size_t len = TEST_RAND(0, (elemlen-offset)/2);
          mv->list[i].addr = ((VEC_T*)addr)+offset;
          mv->list[i].len = VEC_SZ*len;
          mv->totalsz += VEC_SZ*len;
          lim = offset + len;
          assert(lim <= elemlen);
        }
      }
    }
    SHUFFLE_LIST(gasnet_memvec_t,mv);
  }

  mv->checksum = test_checksum(mv->list, mv->count*sizeof(gasnet_memvec_t));
  verify_memvec_list(mv);
  return mv;
}


test_memvec_list *buildcontig_memvec_list(void *addr, size_t elemlen, size_t areasz) {
  test_memvec_list *mv = test_malloc(sizeof(test_memvec_list)+sizeof(gasnet_memvec_t));
  mv->count = 1;
  mv->list = (gasnet_memvec_t*)(mv+1);
  mv->totalsz = ((uintptr_t)elemlen)*VEC_SZ;
  mv->list[0].addr = ((VEC_T*)addr) + TEST_RAND(0,areasz-elemlen);
  mv->list[0].len = elemlen*VEC_SZ;
  mv->checksum = test_checksum(mv->list, sizeof(gasnet_memvec_t));
  return mv;
}

/* make same size */
void trim_memvec_list(test_memvec_list *one, test_memvec_list *two) {
  test_memvec_list *p;
  uintptr_t totalsz = MIN(one->totalsz, two->totalsz);
  if (one->totalsz > totalsz) p = one;
  else p = two;

  while (p->totalsz > totalsz) {
    uintptr_t diff = p->totalsz - totalsz;
    if (diff < p->list[p->count-1].len) {
      p->list[p->count-1].len -= diff;
      p->totalsz -= diff;
    } else {
      p->totalsz -= p->list[p->count-1].len;
      p->count--;
    }
  }
  p->checksum = test_checksum(p->list, p->count*sizeof(gasnet_memvec_t));
  verify_memvec_list(p);
  assert(one->totalsz == two->totalsz);
}

void _verify_memvec_data_both(test_memvec_list *src, void *result, 
                              gasnet_node_t nodeid, VEC_T *areaptr,
                              const char *context, const char *file, int line) {
  VEC_T *p = result;
  size_t i,j;
  for (i = 0; i < src->count; i++) {
    for (j = 0; j < src->list[i].len/VEC_SZ; j ++) {
      VEC_T srcval;
      VEC_T resval = *p;
      if (areaptr == NULL) /* local src */
        srcval = ((VEC_T *)(src->list[i].addr))[j];
      else { /* remote src */
        size_t offset = (VEC_T *)(src->list[i].addr) + j - areaptr;
        srcval = SEG_VALUE(nodeid, offset);
      }
      if (srcval != resval) {
        ERR("ERROR: mismatch at memvec segment %i, element %i\n"
            "  expected val="VEC_FMT"\n"
            "    actual val="VEC_FMT"\n"
            "  at %s:%s:%i\n",
          (int)i, (int)j, VEC_STR(srcval), VEC_STR(resval),
          context, file, line);
        { size_t sz = gasnett_format_memveclist_bufsz(src->count);
          char *buf = test_malloc(sz);
          gasnett_format_memveclist(buf, src->count, src->list);
          ERR("memvec: %s\n", buf);
        }
        FATALERR("testvis failed.");
      }
      p++;
    }
  }
}
#define verify_memvec_data(src,result,context) do {                                             \
  if (verify) _verify_memvec_data_both((src),(result),mynode,NULL,(context),__FILE__,__LINE__); \
} while (0)

#define verify_memvec_data_remote(src,result,node,areaptr,context) do {                              \
  if (verify) _verify_memvec_data_both((src),(result),(node),(areaptr),(context),__FILE__,__LINE__); \
} while (0)

/* ------------------------------------------------------------------------------------ */
typedef struct {
  uint64_t checksum;
  uintptr_t totalsz; /* in bytes */
  size_t count; /* in segments */
  size_t chunklen; /* in bytes */
  void * *list;
} test_addr_list;

void _verify_addr_list(test_addr_list *al, const char *file, int line) {
  if (al->totalsz != al->count*al->chunklen)
    FATALERR("Inconsistent totalsz in verify_addr_list at %s:%i", file, line);
  if (al->checksum != test_checksum(al->list, al->count*sizeof(void *)))
    FATALERR("Checksum mismatch in verify_addr_list at %s:%i", file, line);
}
#define verify_addr_list(al) do {                          \
  if (verify) _verify_addr_list((al), __FILE__, __LINE__); \
} while (0)

/* generate two chunksizes */
void rand_chunkelem(size_t *one, size_t *two) {
  if (TEST_RAND_ONEIN(2)) { size_t *tmp = one; one = two; two = tmp; }
  switch (TEST_RAND(0,2)) {
    case 0: { /* same chunksz */
      *one = TEST_RAND(1, TEST_RAND(1, MAX_CHUNKSZ));
      *two = *one;
      break;
    }
    case 1: { /* one a multiple of another */
      size_t factor = TEST_RAND(1, TEST_RAND(1, TEST_RAND(1, MAX_CHUNKSZ)));
      *one = (TEST_RAND(factor, MAX_CHUNKSZ) / factor);
      *two = *one * factor;
      break;
    }
    case 2: { /* both a small multiple of underlying factor */
      size_t onemult = TEST_RAND(1, 5);
      size_t twomult = TEST_RAND(1, 5);
      size_t factor = TEST_RAND(1, MAX_CHUNKSZ/MAX(onemult, twomult));
      *one = factor * onemult;
      *two = factor * twomult;
      break;
    }
    default: FATALERR("TEST_RAND failure");
  }
  assert(*one >= 1 && *one <= MAX_CHUNKSZ);
  assert(*two >= 1 && *two <= MAX_CHUNKSZ);
}

/* build a addrlist over the area [addr...addr+len*VEC_SZ]
   note elemlen is a VEC_T element count
 */
test_addr_list *rand_addr_list(void *addr, size_t chunkelem, size_t elemlen, int allowoverlap) {
  test_addr_list *al;
  size_t count;
  size_t maxchunks = MIN(MAX_IDXLEN, elemlen/chunkelem);
  count = TEST_RAND_PICK(TEST_RAND(1, maxchunks),
                         TEST_RAND(1, TEST_RAND(1, TEST_RAND(1, maxchunks))));
  if (TEST_RAND_ONEIN(20)) count = 0;
  al = test_malloc(sizeof(test_addr_list)+count*sizeof(void *));
  al->count = count;
  al->list = (void **)(al+1);

  if (allowoverlap) {
    size_t i;
    for (i = 0; i < count; i++) {
      size_t offset = TEST_RAND(0, elemlen-chunkelem);
      al->list[i] = ((VEC_T*)addr)+offset;
    }
  } else {
    size_t i;
    size_t lim = 0;
    size_t per = 0;
    if (count > 0) { per = elemlen / count; assert(chunkelem <= per); }
    /* build non-overlapping, monotonically increasing vectors */
    for (i = 0; i < count; i++) {
      size_t offset = TEST_RAND(lim, per*(i+1)-chunkelem);
      al->list[i] = ((VEC_T*)addr)+offset;
      lim = offset + chunkelem;
    }
    assert(lim <= elemlen);
    SHUFFLE_LIST(void *,al);
  }

  al->chunklen = chunkelem*VEC_SZ;
  al->totalsz = al->count*al->chunklen;
  al->checksum = test_checksum(al->list, al->count*sizeof(void *));
  verify_addr_list(al);
  return al;
}

test_addr_list *buildcontig_addr_list(void *addr, size_t elemlen, size_t areasz) {
  test_addr_list *al = test_malloc(sizeof(test_addr_list)+sizeof(void *));
  al->list = (void **)(al+1);
  al->list[0] = ((VEC_T*)addr) + TEST_RAND(0,areasz-elemlen);
  if (elemlen == 0) {
    al->count = 0;
    al->chunklen = VEC_SZ;
  } else { 
    al->count = 1;
    al->chunklen = elemlen*VEC_SZ;
  }
  al->totalsz = ((uintptr_t)al->count)*al->chunklen;
  al->checksum = test_checksum(al->list, al->count*sizeof(void *));
  verify_addr_list(al);
  return al;
}

/* make same size */
void trim_addr_list(test_addr_list *one, test_addr_list *two) {
  while (one->totalsz != two->totalsz) {
    test_addr_list *p;
    if (one->totalsz > two->totalsz) p = one;
    else p = two;
    p->count--;
    p->totalsz -= p->chunklen;
  }
  one->checksum = test_checksum(one->list, one->count*sizeof(void *));
  two->checksum = test_checksum(two->list, two->count*sizeof(void *));
  verify_addr_list(one);
  verify_addr_list(two);
  assert(one->totalsz == two->totalsz);
}

void _verify_addr_list_data_both(test_addr_list *src, void *result, 
                                gasnet_node_t nodeid, VEC_T *areaptr,
                            const char *context, const char *file, int line) {
  VEC_T *p = result;
  size_t chunkelem = src->chunklen / VEC_SZ;
  size_t i,j;
  for (i = 0; i < src->count; i++) {
    for (j = 0; j < chunkelem; j++) {
      VEC_T srcval;
      VEC_T resval = *p;
      if (areaptr == NULL) /* local src */
        srcval = ((VEC_T *)(src->list[i]))[j];
      else { /* remote src */
        size_t offset = ((VEC_T *)(src->list[i]))+j - areaptr;
        srcval = SEG_VALUE(nodeid, offset);
      }
      if (srcval != resval) {
        ERR("ERROR: mismatch at chunk %i, element %i\n"
            "  expected val="VEC_FMT"\n"
            "    actual val="VEC_FMT"\n"
            "  at %s:%s:%i\n",
          (int)i, (int)j, VEC_STR(srcval), VEC_STR(resval),
          context, file, line);
        { size_t sz = gasnett_format_addrlist_bufsz(src->count);
          char *buf = test_malloc(sz);
          gasnett_format_addrlist(buf, src->count, src->list, src->chunklen);
          ERR("addrlist: %s\n", buf);
        }
        FATALERR("testvis failed.");
      }
      p++;
    }
  }
}
#define verify_addr_list_data(src,result,context) do {                                             \
  if (verify) _verify_addr_list_data_both((src),(result),mynode,NULL,(context),__FILE__,__LINE__); \
} while (0)

#define verify_addr_list_data_remote(src,result,node,areaptr,context) do {                              \
  if (verify) _verify_addr_list_data_both((src),(result),(node),(areaptr),(context),__FILE__,__LINE__); \
} while (0)

/* ------------------------------------------------------------------------------------ */
typedef struct {
  uint64_t checksum;
  size_t _descsz; /* in bytes */
  uintptr_t totalsz; /* in bytes */
  size_t srcvolume; /* in bytes */
  size_t dstvolume; /* in bytes */
  void *srcaddr;
  void *dstaddr;
  size_t *srcextents; /* in elem */
  size_t *dstextents; /* in elem */
  size_t *srcstrides; /* in bytes */
  size_t *dststrides; /* in bytes */
  size_t *contigstrides; /* in bytes */
  size_t *count; /* count[0] in bytes, count[1+] in elem */
  size_t stridelevels;
} test_strided_desc;

void _verify_strided_desc(test_strided_desc *sd, const char *file, int line) {
  size_t i;
  size_t sz = 1;
  size_t srcvol = VEC_SZ;
  size_t dstvol = VEC_SZ;
  if (sd->checksum != test_checksum(((uint64_t*)sd)+1, sd->_descsz-8))
    FATALERR("Checksum mismatch in verify_strided_desc at %s:%i", file, line);
  sz = 1;
  for (i=0; i < sd->stridelevels; i++) {
    sz *= sd->count[i];
    srcvol *= sd->srcextents[i];
    dstvol *= sd->dstextents[i];
    assert_always(sd->contigstrides[i] == sz);
    if (i == 0)
      assert_always(sd->count[i]/VEC_SZ <= sd->srcextents[i] && sd->count[i]/VEC_SZ <= sd->dstextents[i]);
    else
      assert_always(sd->count[i] <= sd->srcextents[i] && sd->count[i] <= sd->dstextents[i]);
    assert_always(sd->srcstrides[i] == srcvol);
    assert_always(sd->dststrides[i] == dstvol);
  }
  sz *= sd->count[i];
  srcvol *= sd->srcextents[i];
  dstvol *= sd->dstextents[i];
  if (i == 0)
    assert_always(sd->count[i]/VEC_SZ <= sd->srcextents[i] && sd->count[i]/VEC_SZ <= sd->dstextents[i]);
  else
    assert_always(sd->count[i] <= sd->srcextents[i] && sd->count[i] <= sd->dstextents[i]);

  assert_always(sz == sd->totalsz);
  assert_always(sd->srcvolume == srcvol);
  assert_always(sd->dstvolume == dstvol);
  if (sz != 0) assert_always(sd->count[0] >= VEC_SZ && sd->count[0] % VEC_SZ == 0);
  if (sd->stridelevels > 0) {
    assert_always(sd->srcvolume == sd->srcstrides[sd->stridelevels-1]*sd->srcextents[sd->stridelevels]);
    assert_always(sd->dstvolume == sd->dststrides[sd->stridelevels-1]*sd->dstextents[sd->stridelevels]);
  }
}
#define verify_strided_desc(sd) do {                        \
  if (verify) _verify_strided_desc((sd), __FILE__, __LINE__); \
} while (0)

/* build a strided set over the area 
   [srcaddr...srcaddr+elemlen*VEC_SZ] and [dstaddr...dstaddr+elemlen*VEC_SZ]
   note elemlen is a VEC_T element count
 */
test_strided_desc *rand_strided_desc(void *srcaddr, void *dstaddr, void *contigaddr, size_t elemlen) {
  size_t dim = TEST_RAND(2, TEST_RAND(2, TEST_RAND(2, MAX_STRIDEDIM)));
  size_t sz;
  test_strided_desc *sd;
  size_t i;
  if (TEST_RAND_ONEIN(10)) dim = 1; /* 1-dim (fully contiguous) */
  sz = sizeof(test_strided_desc)+6*dim*sizeof(size_t);
  sd = test_malloc(sz);
  sd->_descsz = sz;
  sd->srcstrides =    (size_t *)(sd+1);
  sd->dststrides =    ((size_t *)(sd+1))+dim;
  sd->srcextents =    ((size_t *)(sd+1))+2*dim;
  sd->dstextents =    ((size_t *)(sd+1))+3*dim;
  sd->count =         ((size_t *)(sd+1))+4*dim;
  sd->contigstrides = ((size_t *)(sd+1))+5*dim;
  sd->stridelevels = dim-1;

  { size_t volume = TEST_RAND(elemlen*7/8, elemlen); /* in elem */
    size_t srcmax = 0;
    size_t dstmax = 0;
    sd->srcextents[0] = volume;
    sd->dstextents[0] = volume;
    /* choose dimensional extents */
    for (i = 1; i < dim; i++) {
      size_t prev;
      prev = sd->srcextents[srcmax];
      sd->srcextents[i] = TEST_RAND(1,TEST_RAND(1, prev-1));
      sd->srcextents[srcmax] /= sd->srcextents[i];
      sd->srcextents[i] = prev / sd->srcextents[srcmax];
      if (sd->srcextents[i] > sd->srcextents[srcmax]) srcmax = i;
      prev = sd->dstextents[dstmax];
      sd->dstextents[i] = TEST_RAND(1,TEST_RAND(1, prev-1));
      sd->dstextents[dstmax] /= sd->dstextents[i];
      sd->dstextents[i] = prev / sd->dstextents[dstmax];
      if (sd->dstextents[i] > sd->dstextents[dstmax]) dstmax = i;
    }
    SHUFFLE_ARRAY(size_t,sd->srcextents, dim);
    SHUFFLE_ARRAY(size_t,sd->dstextents, dim);
 
    /* select counts */
    for (i = 0; i < dim; i++) {
      size_t maxcnt = MIN(sd->srcextents[i],sd->dstextents[i]);
      sd->count[i] = TEST_RAND(TEST_RAND(1, maxcnt), maxcnt);
    }
    /* multiply up dimensions */
    sd->srcvolume = VEC_SZ;
    sd->dstvolume = VEC_SZ;
    for (i = 0; i < dim; i++) { /* strides[dim-1] is calculated but ignored (harmless) */
      sd->srcvolume *= sd->srcextents[i];
      sd->srcstrides[i] = sd->srcvolume;
      sd->dstvolume *= sd->dstextents[i];
      sd->dststrides[i] = sd->dstvolume;
    }
    assert(sd->srcvolume <= volume*VEC_SZ);
    assert(sd->dstvolume <= volume*VEC_SZ);
    sd->srcaddr = ((VEC_T*)srcaddr) + TEST_RAND(0, elemlen-(sd->srcvolume/VEC_SZ));
    sd->dstaddr = ((VEC_T*)dstaddr) + TEST_RAND(0, elemlen-(sd->dstvolume/VEC_SZ));

    if (TEST_RAND_ONEIN(20)) { /* zero sz */
      sd->count[TEST_RAND(0, dim-1)] = 0;
    } else if (TEST_RAND_ONEIN(20)) { /* some null high dims */
      size_t cnt = TEST_RAND(1, dim);
      for (i=dim-cnt; i < dim; i++) 
        sd->count[i] = 1;
    } else if (TEST_RAND_ONEIN(20)) { /* some null low dims */
      size_t cnt = TEST_RAND(1, dim);
      for (i=0; i < cnt; i++) 
        sd->count[i] = 1;
    } else if (TEST_RAND_ONEIN(20)) { /* maximal counts */
      for (i=0; i < dim; i++) 
        sd->count[i] = MIN(sd->srcextents[i],sd->dstextents[i]);
    }

    /* compute count-dependent quantities */
    sd->count[0] *= VEC_SZ;
    sd->totalsz = 1;
    for (i = 0; i < dim; i++) {
      sd->totalsz *= sd->count[i];
      sd->contigstrides[i] = sd->totalsz;
    }

    sd->checksum = test_checksum(((uint64_t*)sd)+1, sd->_descsz-8);
    verify_strided_desc(sd);
    return sd;
  }
}

void _verify_strided_desc_data_both(test_strided_desc *desc, void *result, 
                            gasnet_node_t nodeid, VEC_T *areaptr,
                            const char *context, const char *file, int line) {
  size_t dim = (desc->stridelevels+1);
  size_t * idx;
  size_t * const count = desc->count;
  size_t * const srcstrides = desc->srcstrides;
  /* size_t * const dststrides = desc->dststrides; */
  VEC_T *resultp = result;
  size_t srcoffset = 0; /* in bytes */
  size_t curdim = 0;
  
  if (desc->totalsz == 0) return; /* empty */

  idx = test_calloc(sizeof(size_t), dim);

  while (curdim < dim) {

    /* body */
    VEC_T srcval;
    VEC_T resval = *resultp;
    if (areaptr == NULL) /* local src */
      srcval = *(VEC_T *)(((char*)desc->srcaddr) + srcoffset);
    else { /* remote src */
      size_t fulloffset = ((VEC_T *)(((char*)desc->srcaddr) + srcoffset)) - areaptr;
      srcval = SEG_VALUE(nodeid, fulloffset);
    }
    if (srcval != resval) {
      char idxstr[255];
      char *p = idxstr;
      size_t i;
      for (i=0; i < dim; i++) {
        sprintf(p, "%i", (int)idx[i]);
        if (i < dim-1) strcat(p, ", ");
        p+=strlen(p);
      }
      ERR("mismatch at location [%s]\n"
          "  expected val="VEC_FMT"\n"
          "    actual val="VEC_FMT"\n"
          "  at %s:%s:%i\n",
          idxstr, VEC_STR(srcval), VEC_STR(resval),
        context, file, line);
      { size_t sz = gasnett_format_putsgets_bufsz(desc->stridelevels);
        char *buf = test_malloc(sz);
        gasnett_format_putsgets(buf, NULL, nodeid,
          desc->dstaddr, desc->dststrides,
          desc->srcaddr, desc->srcstrides,
          desc->count, desc->stridelevels);
        ERR("strided desc: %s\n", buf);
      }
      FATALERR("testvis failed.");
    }

    /* increment */
    if (idx[0] < count[0]-VEC_SZ) { 
      idx[0] += VEC_SZ; 
      srcoffset += VEC_SZ;
      assert(idx[0] < count[0]);
    } else {
      idx[0] = 0;
      srcoffset -= count[0]-VEC_SZ;
      for (curdim = 1; curdim < dim; curdim++) {
        if (idx[curdim] < count[curdim]-1) {
          idx[curdim]++;
          srcoffset += srcstrides[curdim-1];
          assert(idx[curdim] < count[curdim]);
          break;
        } else {
          idx[curdim] = 0;
          srcoffset -= srcstrides[curdim-1]*(count[curdim]-1);
        }
      }
    }
    resultp++;
  }
  test_free(idx);
}
#define verify_strided_desc_data(desc,result,context) do {                                             \
  if (verify) _verify_strided_desc_data_both((desc),(result),mynode,NULL,(context),__FILE__,__LINE__); \
} while (0)

#define verify_strided_desc_data_remote(desc,result,node,areaptr,context) do {                              \
  if (verify) _verify_strided_desc_data_both((desc),(result),(node),(areaptr),(context),__FILE__,__LINE__); \
} while (0)

/* ------------------------------------------------------------------------------------ */
VEC_T *myseg = NULL;
VEC_T *partnerseg = NULL;
VEC_T *heapseg = NULL;
size_t areasz = TEST_SEGSZ/NUM_AREAS/VEC_SZ; /* in elem */
int mynode, partner;
VEC_T *my_seg_read_area;
VEC_T *my_seg_write1_area;
VEC_T *my_seg_write2_area;
VEC_T *my_seg_remotewrite_area;
VEC_T *my_heap_read_area;
VEC_T *my_heap_write1_area;
VEC_T *my_heap_write2_area;
VEC_T *my_heap_unused_area;
VEC_T *partner_seg_read_area;
VEC_T *partner_seg_remotewrite_area;

void checkmem(void) {
 if (verify) {
  /* check for corruption of read-only memory segments */
  size_t i;
  for (i = 0; i < areasz; i++) {
    if (my_seg_read_area[i] != SEG_VALUE(mynode, i))
      FATALERR("detected corruption in my_seg_read_area[%i]\n", (int)i);
    if (my_heap_read_area[i] != HEAP_VALUE(mynode, i))
      FATALERR("detected corruption in my_heap_read_area[%i]\n", (int)i);
  }
 }
}

typedef struct {
  test_memvec_list *vsrc;
  test_memvec_list *vdst;
  test_memvec_list *vtmp;
  test_addr_list *isrc;
  test_addr_list *idst;
  test_addr_list *itmp;
  test_strided_desc *sdesc;
  VEC_T *stmpbuf;
} test_op;

#define TIME_DECL()                               \
    struct {                                      \
      uint64_t iters;                             \
      uint64_t minsz, maxsz, sumsz;               \
      gasnett_tick_t sumtm;                       \
      double minbw, maxbw;                        \
    } putinfo = { 0,(uint64_t)-1,0,0,0,1E300,0 }, \
      getinfo = { 0,(uint64_t)-1,0,0,0,1E300,0 }

#define _TIMED_OP(op,sz,pg) do {                              \
      gasnett_tick_t _op_tm = gasnett_ticks_now();            \
      int64_t const _op_sz = (sz);                            \
      { op; }                                                 \
      _op_tm = gasnett_ticks_now() - _op_tm;                  \
      pg##info.sumsz += _op_sz;                               \
      pg##info.sumtm += _op_tm;                               \
      if (_op_sz > 0 &&                                       \
          _op_sz < pg##info.minsz) pg##info.minsz = _op_sz;   \
      if (_op_sz > pg##info.maxsz) pg##info.maxsz = _op_sz;   \
      { double _op_bw = ((double)_op_sz)/_op_tm;              \
        if (_op_sz > 0 &&                                     \
            _op_bw < pg##info.minbw) pg##info.minbw = _op_bw; \
        if (_op_tm > 0 &&                                     \
            _op_bw > pg##info.maxbw) pg##info.maxbw = _op_bw; \
      }                                                       \
      pg##info.iters++;                                       \
    } while (0)
#define TIMED_PUT(op,sz) _TIMED_OP(op,sz,put)
#define TIMED_GET(op,sz) _TIMED_OP(op,sz,get)
#define _TIME_OUTPUT(flavor,pg) do {                                     \
    double _tickspersec = 1.0E18 / gasnett_ticks_to_ns(1000000000);      \
    double _totaldata = ((double)pg##info.sumsz) / (1024*1024);          \
    double _maxbw = pg##info.maxbw * _tickspersec / (1024*1024);         \
    double _minbw = pg##info.minbw * _tickspersec / (1024*1024);         \
    double _avgbw = (_totaldata / pg##info.sumtm) * _tickspersec;        \
    MSG(#pg #flavor" bandwidth (MB/s): %8.6f avg, %8.6f max, %8.6f min", \
        _avgbw, _maxbw, _minbw);                                         \
} while (0)

#define TIME_OUTPUT(flavor) do {                                               \
      if (showtiming) _TIME_OUTPUT(flavor,put);                                \
      if (showtiming) _TIME_OUTPUT(flavor,get);                                \
      if (showtiming)                                                          \
        MSG("data size: %8.3fMB total, %8.3fKB avg, %8.3fKB min, %8.3fKB max", \
            putinfo.sumsz/1048576.0, putinfo.sumsz/1024.0/putinfo.iters,       \
            putinfo.minsz/1024.0, putinfo.maxsz/1024.0);                       \
    } while (0)

void doit(int iters, int runtests) {
  GASNET_BEGIN_FUNCTION();
  /* break up the segments and hook up our area pointers */
  my_seg_read_area = myseg;
  my_seg_write1_area = myseg+areasz;
  my_seg_write2_area = myseg+2*areasz;
  my_seg_remotewrite_area = myseg+3*areasz;
  my_heap_read_area = heapseg;
  my_heap_write1_area = heapseg+areasz;
  my_heap_write2_area = heapseg+2*areasz;
  my_heap_unused_area = heapseg+3*areasz;
  partner_seg_read_area = partnerseg;
  partner_seg_remotewrite_area = partnerseg+3*areasz;
  
  if (verify) { /* init memory segments to known values */
    size_t i;
    for (i = 0; i < areasz; i++) {
      my_seg_read_area[i] =    SEG_VALUE(mynode, i);
      my_heap_read_area[i] = HEAP_VALUE(mynode, i);
    }
  }

  BARRIER();
  /*---------------------------------------------------------------------------------*/
  if (runtests & RUN_VECTOR) { 
    int iter;
    TIME_DECL();
    MSG("Vector...");
    for (iter = 0; iter < iters; iter++) {
      /* put test */
      { test_memvec_list *src;
        test_memvec_list *dst;
        test_memvec_list *tmp;

        src = rand_memvec_list(TEST_RAND_PICK(my_heap_read_area, my_seg_read_area), areasz, 1);
        dst = rand_memvec_list(partner_seg_remotewrite_area, areasz, 0);
        trim_memvec_list(src, dst);
        tmp = buildcontig_memvec_list(TEST_RAND_PICK(my_heap_write2_area, my_seg_write2_area), dst->totalsz/VEC_SZ, areasz);

        TIMED_PUT(gasnet_putv_bulk(partner, dst->count, dst->list, src->count, src->list),dst->totalsz);
        verify_memvec_list(src);
        verify_memvec_list(dst);
        TIMED_GET(gasnet_getv_bulk(tmp->count, tmp->list, partner, dst->count, dst->list),dst->totalsz);
        verify_memvec_list(tmp);
        verify_memvec_list(dst);
        verify_memvec_data(src, tmp->list[0].addr, "gasnet_putv_bulk/gasnet_getv_bulk test");
        test_free(src);
        test_free(dst);
        test_free(tmp);
      }

      /* get test */
      { test_memvec_list *src;
        test_memvec_list *dst;
        test_memvec_list *tmp;
        void *dstarea = TEST_RAND_PICK(my_heap_write1_area, my_seg_write1_area);

        src = rand_memvec_list(partner_seg_read_area, areasz, 1);
        dst = rand_memvec_list(dstarea, areasz, 0);
        trim_memvec_list(src, dst);
        tmp = buildcontig_memvec_list(my_seg_write2_area, dst->totalsz/VEC_SZ, areasz);

        gasnet_getv_bulk(dst->count, dst->list, partner, src->count, src->list);
        verify_memvec_list(src);
        verify_memvec_list(dst);
        if ((segeverything || dstarea == my_seg_write1_area) && 
            TEST_RAND_PICK(0,1)) {
          gasnet_getv_bulk(tmp->count, tmp->list, mynode, dst->count, dst->list);
        } else {
          gasnet_putv_bulk(mynode, tmp->count, tmp->list, dst->count, dst->list);
        }
        verify_memvec_list(tmp);
        verify_memvec_list(dst);
        verify_memvec_data_remote(src, tmp->list[0].addr, partner, partner_seg_read_area, "gasnet_getv_bulk test");
        test_free(src);
        test_free(dst);
        test_free(tmp);
      }
      TEST_PROGRESS_BAR(iter, iters);
    }
    checkmem();
    TIME_OUTPUT(v);
  }
  BARRIER();
  /*---------------------------------------------------------------------------------*/
  if (runtests & RUN_INDEXED) { 
    int iter;
    TIME_DECL();
    MSG("Indexed...");
    for (iter = 0; iter < iters; iter++) {
      /* put test */
      { test_addr_list *src;
        test_addr_list *dst;
        test_addr_list *tmp;
        size_t srcchunkelem, dstchunkelem;

        rand_chunkelem(&srcchunkelem, &dstchunkelem);
        src = rand_addr_list(TEST_RAND_PICK(my_heap_read_area, my_seg_read_area), srcchunkelem, areasz, 1);
        dst = rand_addr_list(partner_seg_remotewrite_area, dstchunkelem, areasz, 0);
        trim_addr_list(src, dst);
        tmp = buildcontig_addr_list(TEST_RAND_PICK(my_heap_write2_area, my_seg_write2_area), dst->totalsz/VEC_SZ, areasz);

        TIMED_PUT(gasnet_puti_bulk(partner, dst->count, dst->list, dst->chunklen, src->count, src->list, src->chunklen),dst->totalsz);
        verify_addr_list(src);
        verify_addr_list(dst);
        TIMED_GET(gasnet_geti_bulk(tmp->count, tmp->list, tmp->chunklen, partner, dst->count, dst->list, dst->chunklen),dst->totalsz);
        verify_addr_list(tmp);
        verify_addr_list(dst);
        verify_addr_list_data(src, tmp->list[0], "gasnet_puti_bulk/gasnet_geti_bulk test");
        test_free(src);
        test_free(dst);
        test_free(tmp);
      }

      /* get test */
      { test_addr_list *src;
        test_addr_list *dst;
        test_addr_list *tmp;
        void *dstarea = TEST_RAND_PICK(my_heap_write1_area, my_seg_write1_area);
        size_t srcchunkelem, dstchunkelem;

        rand_chunkelem(&srcchunkelem, &dstchunkelem);
        src = rand_addr_list(partner_seg_read_area, srcchunkelem, areasz, 1);
        dst = rand_addr_list(dstarea, dstchunkelem, areasz, 0);
        trim_addr_list(src, dst);
        tmp = buildcontig_addr_list(my_seg_write2_area, dst->totalsz/VEC_SZ, areasz);

        gasnet_geti_bulk(dst->count, dst->list, dst->chunklen, partner, src->count, src->list, src->chunklen);
        verify_addr_list(src);
        verify_addr_list(dst);
        if ((segeverything || dstarea == my_seg_write1_area) && 
            TEST_RAND_PICK(0,1)) {
          gasnet_geti_bulk(tmp->count, tmp->list, tmp->chunklen, mynode, dst->count, dst->list, dst->chunklen);
        } else {
          gasnet_puti_bulk(mynode, tmp->count, tmp->list, tmp->chunklen, dst->count, dst->list, dst->chunklen);
        }
        verify_addr_list(tmp);
        verify_addr_list(dst);
        verify_addr_list_data_remote(src, tmp->list[0], partner, partner_seg_read_area, "gasnet_geti_bulk test");
        test_free(src);
        test_free(dst);
        test_free(tmp);
      }
      TEST_PROGRESS_BAR(iter, iters);
    }
    checkmem();
    TIME_OUTPUT(i);
  }
  BARRIER();
  /*---------------------------------------------------------------------------------*/
  if (runtests & RUN_STRIDED) { 
    int iter;
    TIME_DECL();
    MSG("Strided...");
    for (iter = 0; iter < iters; iter++) {
      /* put test */
      { test_strided_desc *desc;
        void *srcarea = TEST_RAND_PICK(my_heap_read_area, my_seg_read_area);
        void *dstarea = partner_seg_remotewrite_area;
        void *tmparea = TEST_RAND_PICK(my_heap_write2_area, my_seg_write2_area);
        VEC_T *tmpbuf;

        desc = rand_strided_desc(srcarea, dstarea, tmparea, areasz);
        tmpbuf = ((VEC_T*)tmparea) + TEST_RAND(0,areasz - desc->totalsz/VEC_SZ);

        TIMED_PUT(gasnet_puts_bulk(partner, desc->dstaddr, desc->dststrides, desc->srcaddr, desc->srcstrides, desc->count, desc->stridelevels),desc->totalsz);
        verify_strided_desc(desc);
        TIMED_GET(gasnet_gets_bulk(tmpbuf, desc->contigstrides, partner, desc->dstaddr, desc->dststrides, desc->count, desc->stridelevels),desc->totalsz);
        verify_strided_desc(desc);
        verify_strided_desc_data(desc, tmpbuf, "gasnet_puts_bulk/gasnet_gets_bulk test");
        test_free(desc);
      }

      /* get test */
      { test_strided_desc *desc;
        void *srcarea = partner_seg_read_area;
        void *dstarea = TEST_RAND_PICK(my_heap_write1_area, my_seg_write1_area);
        void *tmparea = my_seg_write2_area;
        VEC_T *tmpbuf;

        desc = rand_strided_desc(srcarea, dstarea, tmparea, areasz);
        tmpbuf = ((VEC_T*)tmparea) + TEST_RAND(0,areasz - desc->totalsz/VEC_SZ);

        gasnet_gets_bulk(desc->dstaddr, desc->dststrides, partner, desc->srcaddr, desc->srcstrides, desc->count, desc->stridelevels);
        verify_strided_desc(desc);
        if ((segeverything || dstarea == my_seg_write1_area) && 
            TEST_RAND_PICK(0,1)) {
          gasnet_gets_bulk(tmpbuf, desc->contigstrides, mynode, desc->dstaddr, desc->dststrides, desc->count, desc->stridelevels);
        } else {
          gasnet_puts_bulk(mynode, tmpbuf, desc->contigstrides, desc->dstaddr, desc->dststrides, desc->count, desc->stridelevels);
        }
        verify_strided_desc(desc);
        verify_strided_desc_data_remote(desc, tmpbuf, partner, partner_seg_read_area, "gasnet_gets_bulk test");
        test_free(desc);
      }
      TEST_PROGRESS_BAR(iter, iters);
    }
    checkmem();
    TIME_OUTPUT(s);
  }
  BARRIER();
  /*---------------------------------------------------------------------------------*/
  if (runtests & RUN_NB) { 
    int iter;
    MSG("Non-blocking tests...");
    for (iter = 0; iter < iters; iter++) {
      size_t numops = TEST_RAND(1, MAX_INFLIGHT_OPS);
      gasnet_handle_t *handles = test_calloc(sizeof(gasnet_handle_t), numops);
      test_op *ops = test_calloc(sizeof(test_op), numops);
      size_t opareasz = areasz / numops;
      size_t i;

      /* puts */
      for(i=0; i < numops; i++) {
        /* VEC_T *op_my_seg_write1_area = my_seg_write1_area + opareasz*i; */
        VEC_T *op_my_seg_write2_area = my_seg_write2_area + opareasz*i;
        /* VEC_T *op_my_heap_write1_area = my_heap_write1_area + opareasz*i; */
        VEC_T *op_my_heap_write2_area = my_heap_write2_area + opareasz*i;
        VEC_T *op_partner_seg_remotewrite_area = partner_seg_remotewrite_area + opareasz*i;

        switch (TEST_RAND(1,3)) {
          case 1: {
            ops[i].vsrc = rand_memvec_list(TEST_RAND_PICK(my_heap_read_area, my_seg_read_area), areasz, 1);
            ops[i].vdst = rand_memvec_list(op_partner_seg_remotewrite_area, opareasz, 0);
            trim_memvec_list(ops[i].vsrc, ops[i].vdst);
            ops[i].vtmp = buildcontig_memvec_list(TEST_RAND_PICK(op_my_heap_write2_area, op_my_seg_write2_area), ops[i].vdst->totalsz/VEC_SZ, opareasz);

            if (TEST_RAND_ONEIN(2)) 
              handles[i] = gasnet_putv_nb_bulk(partner, ops[i].vdst->count, ops[i].vdst->list, ops[i].vsrc->count, ops[i].vsrc->list);
            else gasnet_putv_nbi_bulk(partner, ops[i].vdst->count, ops[i].vdst->list, ops[i].vsrc->count, ops[i].vsrc->list);

            verify_memvec_list(ops[i].vsrc);
            verify_memvec_list(ops[i].vdst);
            break;
          }
          case 2: {
            size_t srcchunkelem, dstchunkelem;
            rand_chunkelem(&srcchunkelem, &dstchunkelem);

            ops[i].isrc = rand_addr_list(TEST_RAND_PICK(my_heap_read_area, my_seg_read_area), srcchunkelem, areasz, 1);
            ops[i].idst = rand_addr_list(op_partner_seg_remotewrite_area, dstchunkelem, opareasz, 0);
            trim_addr_list(ops[i].isrc, ops[i].idst);
            ops[i].itmp = buildcontig_addr_list(TEST_RAND_PICK(op_my_heap_write2_area, op_my_seg_write2_area), ops[i].idst->totalsz/VEC_SZ, opareasz);

            if (TEST_RAND_ONEIN(2)) 
              handles[i] = gasnet_puti_nb_bulk(partner, ops[i].idst->count, ops[i].idst->list, ops[i].idst->chunklen, ops[i].isrc->count, ops[i].isrc->list, ops[i].isrc->chunklen);
            else gasnet_puti_nbi_bulk(partner, ops[i].idst->count, ops[i].idst->list, ops[i].idst->chunklen, ops[i].isrc->count, ops[i].isrc->list, ops[i].isrc->chunklen);

            verify_addr_list(ops[i].isrc);
            verify_addr_list(ops[i].idst);
            break;
          }
          case 3: {
            void *srcarea = TEST_RAND_PICK(my_heap_read_area, my_seg_read_area);
            void *dstarea = op_partner_seg_remotewrite_area;
            void *tmparea = TEST_RAND_PICK(op_my_heap_write2_area, op_my_seg_write2_area);

            ops[i].sdesc = rand_strided_desc(srcarea, dstarea, tmparea, opareasz);
            ops[i].stmpbuf = ((VEC_T*)tmparea) + TEST_RAND(0,opareasz - ops[i].sdesc->totalsz/VEC_SZ);

            if (TEST_RAND_ONEIN(2)) 
              handles[i] = gasnet_puts_nb_bulk(partner, ops[i].sdesc->dstaddr, ops[i].sdesc->dststrides, ops[i].sdesc->srcaddr, ops[i].sdesc->srcstrides, ops[i].sdesc->count, ops[i].sdesc->stridelevels);
            else gasnet_puts_nbi_bulk(partner, ops[i].sdesc->dstaddr, ops[i].sdesc->dststrides, ops[i].sdesc->srcaddr, ops[i].sdesc->srcstrides, ops[i].sdesc->count, ops[i].sdesc->stridelevels);

            verify_strided_desc(ops[i].sdesc);
            break;
          }
        }
      }

      /* sync */
      gasnet_wait_syncnb_all(handles, numops);
      gasnet_wait_syncnbi_all();

      /* gets */
      for(i=0; i < numops; i++) {
        if (ops[i].vsrc != NULL) {
          assert(ops[i].vdst != NULL && ops[i].vtmp != NULL);

          if (TEST_RAND_ONEIN(2)) 
            handles[i] = gasnet_getv_nb_bulk(ops[i].vtmp->count, ops[i].vtmp->list, partner, ops[i].vdst->count, ops[i].vdst->list);
          else gasnet_getv_nbi_bulk(ops[i].vtmp->count, ops[i].vtmp->list, partner, ops[i].vdst->count, ops[i].vdst->list);

          verify_memvec_list(ops[i].vtmp);
          verify_memvec_list(ops[i].vdst);
        } else if (ops[i].isrc != NULL) {
          assert(ops[i].idst != NULL && ops[i].itmp != NULL);

          if (TEST_RAND_ONEIN(2)) 
            handles[i] = gasnet_geti_nb_bulk(ops[i].itmp->count, ops[i].itmp->list, ops[i].itmp->chunklen, partner, ops[i].idst->count, ops[i].idst->list, ops[i].idst->chunklen);
          else gasnet_geti_nbi_bulk(ops[i].itmp->count, ops[i].itmp->list, ops[i].itmp->chunklen, partner, ops[i].idst->count, ops[i].idst->list, ops[i].idst->chunklen);

          verify_addr_list(ops[i].itmp);
          verify_addr_list(ops[i].idst);
        } else {
          assert(ops[i].sdesc != NULL);

          if (TEST_RAND_ONEIN(2)) 
            handles[i] = gasnet_gets_nb_bulk(ops[i].stmpbuf, ops[i].sdesc->contigstrides, partner, ops[i].sdesc->dstaddr, ops[i].sdesc->dststrides, ops[i].sdesc->count, ops[i].sdesc->stridelevels);
          else gasnet_gets_nbi_bulk(ops[i].stmpbuf, ops[i].sdesc->contigstrides, partner, ops[i].sdesc->dstaddr, ops[i].sdesc->dststrides, ops[i].sdesc->count, ops[i].sdesc->stridelevels);

          verify_strided_desc(ops[i].sdesc);
        }
      }

      /* sync */
      gasnet_wait_syncnb_all(handles, numops);
      gasnet_wait_syncnbi_all();

      for(i=0; i < numops; i++) {
        if (ops[i].vsrc != NULL) {
          assert(ops[i].vdst != NULL && ops[i].vtmp != NULL);

          verify_memvec_list(ops[i].vsrc);
          verify_memvec_list(ops[i].vdst);
          verify_memvec_list(ops[i].vtmp);
          verify_memvec_data(ops[i].vsrc, ops[i].vtmp->list[0].addr, "non-blocking gasnet_putv_bulk/gasnet_getv_bulk test");

          test_free(ops[i].vsrc);
          test_free(ops[i].vdst);
          test_free(ops[i].vtmp);
        } else if (ops[i].isrc != NULL) {
          assert(ops[i].idst != NULL && ops[i].itmp != NULL);

          verify_addr_list(ops[i].isrc);
          verify_addr_list(ops[i].idst);
          verify_addr_list(ops[i].itmp);
          verify_addr_list_data(ops[i].isrc, ops[i].itmp->list[0], "non-blocking gasnet_puti_bulk/gasnet_geti_bulk test");

          test_free(ops[i].isrc);
          test_free(ops[i].idst);
          test_free(ops[i].itmp);
        } else {
          assert(ops[i].sdesc != NULL && ops[i].stmpbuf != NULL);

          verify_strided_desc_data(ops[i].sdesc, ops[i].stmpbuf, "gasnet_puts_bulk/gasnet_gets_bulk test");

          test_free(ops[i].sdesc);
        }
      }
      test_free(handles);
      test_free(ops);
      TEST_PROGRESS_BAR(iter, iters);
    }
    checkmem();
  }
  /*---------------------------------------------------------------------------------*/
  BARRIER();
}
/* ------------------------------------------------------------------------------------ */
int main(int argc, char **argv) {
  int iters = 100;
  int seedoffset = 0;
  int runtests = 0;
  int halfduplex = 0;
  int i;

  assert_always(VEC_SZ == sizeof(VEC_T));
  GASNET_Safe(gasnet_init(&argc, &argv));
  GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
  test_init("testvis",0, "[options] (iters) (seed)\n"
            " -v/-i/-s/-n  run vector/indexed/strided/non-blocking tests (defaults to all)\n"
            " -d        disable correctness verification checks\n"
            " -o        one-way (half duplex) mode\n"
            " -t        enable timing output\n"
            " iters     number of testing iterations\n"
            " seed      seed offset for PRNG \n"
            );

#if PLATFORM_OS_CATAMOUNT
  if (! gasnett_getenv_yesno_withdefault("GASNET_FORCE_TESTVIS",0)) {
    MSG0("testvis is disabled on Catamount (gasnet bug #2189)");
    gasnet_exit(0);
    return 0;
  }
#endif

  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      int j;
      for (j = 1; argv[i][j]; j++) {
        switch(argv[i][j]) {
          case 'v': case 'V': runtests |= RUN_VECTOR; break;
          case 'i': case 'I': runtests |= RUN_INDEXED; break;
          case 's': case 'S': runtests |= RUN_STRIDED; break;
          case 'n': case 'N': runtests |= RUN_NB; break;
          case 'd': case 'D': verify = 0; break;
          case 'o': case 'O': halfduplex = 1; break;
          case 't': case 'T': showtiming = 1; break;
          default: test_usage();
        }
      }
    } else break;
  }
  if (runtests == 0) runtests = RUN_VECTOR | RUN_INDEXED | RUN_STRIDED | RUN_NB;
  if (i < argc) { iters = atoi(argv[i]); i++; }
  if (i < argc) { seedoffset = atoi(argv[i]); i++; }
  if (i < argc) test_usage();

  mynode = gasnet_mynode();
  myseg = TEST_SEG(mynode);
  partner = (gasnet_mynode() + 1) % gasnet_nodes();
  partnerseg = TEST_SEG(partner);
  heapseg = (VEC_T *)test_malloc(TEST_SEGSZ);

  if (seedoffset == 0) {
    seedoffset = (((unsigned int)TIME()) & 0xFFFF);
    TEST_BCAST(&seedoffset, 0, &seedoffset, sizeof(&seedoffset));
  }
  TEST_SRAND(mynode+seedoffset);
  MSG("running %i iterations of %s%s%s%s%s test (seed=%i)%s...", 
    iters, 
    (halfduplex?"half-duplex ":""),
    (runtests&RUN_VECTOR?"V":""), 
    (runtests&RUN_INDEXED?"I":""), 
    (runtests&RUN_STRIDED?"S":""),
    (runtests&RUN_NB?"N":""),
    mynode+seedoffset,
    (verify?"":" (verification disabled)")
    );

  if (halfduplex && mynode % 2 == 1) runtests = 0; /* odd nodes passive */

  doit(iters, runtests);
  MSG("done.");

  gasnet_exit(0);
  return 0;
}
