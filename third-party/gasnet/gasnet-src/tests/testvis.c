/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testvis.c $
 * Description: GASNet Vector, Indexed & Strided correctness tests
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include <gasnet_vis.h>
#include <gasnet_coll.h>

/* select a larger than default segment, 
   because this test needs plenty of memory to play with */
uintptr_t segsz = (16*1024*1024);
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR ((uintptr_t)alignup(segsz,PAGESZ))
#endif

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

#include <test.h>

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
  #define SEG_VALUE(node, idx)   (((uint64_t)0x8000000000000000ULL) | ((uint64_t)(node) << 32) | (uint64_t)(idx))
  #define HEAP_VALUE(node, idx)  (((uint64_t)0x0000000000000000ULL) | ((uint64_t)(node) << 32) | (uint64_t)(idx))
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

size_t max_veclen = 100;
size_t max_idxlen = 8192;
size_t max_stridedim = 20;
size_t max_chunksz = 256;
size_t max_inflight_ops = 16;

#define RUN_VECTOR   1
#define RUN_INDEXED  2
#define RUN_STRIDED  4
#define RUN_TRANSPOSE 8 
#define RUN_NB       16

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
  gex_Memvec_t *list;
} test_memvec_list;

void _verify_memvec_list(test_memvec_list *mv, const char *file, int line) {
  size_t i, sum;
  if (mv->checksum != test_checksum(mv->list, mv->count*sizeof(gex_Memvec_t)))
    FATALERR("Checksum mismatch in verify_memvec_list at %s:%i", file, line);
  sum = 0;
  for (i=0; i < mv->count; i++) {
    sum += mv->list[i].gex_len;
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
  size_t count = TEST_RAND_PICK(TEST_RAND(1, MIN(max_veclen,elemlen)),
                 TEST_RAND(1, TEST_RAND(1, TEST_RAND(1, MIN(max_veclen,elemlen)))));
  size_t per = 0;
  if (TEST_RAND_ONEIN(20)) count = 0;
  if (count > 0) per = elemlen / count; 
  mv = test_calloc(1,sizeof(test_memvec_list)+count*sizeof(gex_Memvec_t));
  mv->count = count;
  mv->list = (gex_Memvec_t *)(mv+1);
  mv->totalsz = 0;

  if (allowoverlap) {
    size_t i;
    for (i = 0; i < count; i++) {
      size_t offset = TEST_RAND(0, elemlen-1);
      size_t len = TEST_RAND(0, MIN(per,elemlen-offset));
      if (TEST_RAND_ONEIN(20)) {
        mv->list[i].gex_addr = NULL;
        mv->list[i].gex_len = 0;
      } else {
        mv->list[i].gex_addr = ((VEC_T*)addr)+offset;
        mv->list[i].gex_len = VEC_SZ*len;
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
          mv->list[i].gex_addr = NULL;
          mv->list[i].gex_len = 0;
        } else {
          size_t offset = TEST_RAND(lim, per*(i+1)-1);
          size_t len = TEST_RAND(0, per*(i+1)-offset);
          mv->list[i].gex_addr = ((VEC_T*)addr)+offset;
          mv->list[i].gex_len = VEC_SZ*len;
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
          mv->list[i].gex_addr = NULL;
          mv->list[i].gex_len = 0;
        } else {
          size_t offset = TEST_RAND(lim, lim+(elemlen-lim)/4);
          size_t len = TEST_RAND(0, (elemlen-offset)/2);
          mv->list[i].gex_addr = ((VEC_T*)addr)+offset;
          mv->list[i].gex_len = VEC_SZ*len;
          mv->totalsz += VEC_SZ*len;
          lim = offset + len;
          assert(lim <= elemlen);
        }
      }
    }
    SHUFFLE_LIST(gex_Memvec_t,mv);
  }

  mv->checksum = test_checksum(mv->list, mv->count*sizeof(gex_Memvec_t));
  verify_memvec_list(mv);
  return mv;
}


test_memvec_list *buildcontig_memvec_list(void *addr, size_t elemlen, size_t areasz) {
  test_memvec_list *mv = test_calloc(1,sizeof(test_memvec_list)+sizeof(gex_Memvec_t));
  mv->count = 1;
  mv->list = (gex_Memvec_t*)(mv+1);
  mv->totalsz = ((uintptr_t)elemlen)*VEC_SZ;
  mv->list[0].gex_addr = ((VEC_T*)addr) + TEST_RAND(0,areasz-elemlen);
  mv->list[0].gex_len = elemlen*VEC_SZ;
  mv->checksum = test_checksum(mv->list, sizeof(gex_Memvec_t));
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
    if (diff < p->list[p->count-1].gex_len) {
      p->list[p->count-1].gex_len -= diff;
      p->totalsz -= diff;
    } else {
      p->totalsz -= p->list[p->count-1].gex_len;
      p->count--;
    }
  }
  p->checksum = test_checksum(p->list, p->count*sizeof(gex_Memvec_t));
  verify_memvec_list(p);
  assert(one->totalsz == two->totalsz);
}

void _verify_memvec_data_both(test_memvec_list *src, void *result, 
                              gex_Rank_t nodeid, VEC_T *areaptr,
                              const char *context, const char *file, int line) {
  VEC_T *p = result;
  size_t i,j;
  for (i = 0; i < src->count; i++) {
    for (j = 0; j < src->list[i].gex_len/VEC_SZ; j ++) {
      VEC_T srcval;
      VEC_T resval = *p;
      if (areaptr == NULL) /* local src */
        srcval = ((VEC_T *)(src->list[i].gex_addr))[j];
      else { /* remote src */
        size_t offset = (VEC_T *)(src->list[i].gex_addr) + j - areaptr;
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
      *one = TEST_RAND(1, TEST_RAND(1, max_chunksz));
      *two = *one;
      break;
    }
    case 1: { /* one a multiple of another */
      size_t factor = TEST_RAND(1, TEST_RAND(1, TEST_RAND(1, max_chunksz)));
      *one = (TEST_RAND(factor, max_chunksz) / factor);
      *two = *one * factor;
      break;
    }
    case 2: { /* both a small multiple of underlying factor */
      size_t onemult = TEST_RAND(1, 5);
      size_t twomult = TEST_RAND(1, 5);
      size_t factor = TEST_RAND(1, max_chunksz/MAX(onemult, twomult));
      *one = factor * onemult;
      *two = factor * twomult;
      break;
    }
    default: FATALERR("TEST_RAND failure");
  }
  assert(*one >= 1 && *one <= max_chunksz);
  assert(*two >= 1 && *two <= max_chunksz);
}

/* build a addrlist over the area [addr...addr+len*VEC_SZ]
   note elemlen is a VEC_T element count
 */
test_addr_list *rand_addr_list(void *addr, size_t chunkelem, size_t elemlen, int allowoverlap) {
  test_addr_list *al;
  size_t count;
  size_t maxchunks = MIN(max_idxlen, elemlen/chunkelem);
  count = TEST_RAND_PICK(TEST_RAND(1, maxchunks),
                         TEST_RAND(1, TEST_RAND(1, TEST_RAND(1, maxchunks))));
  if (TEST_RAND_ONEIN(20)) count = 0;
  al = test_calloc(1,sizeof(test_addr_list)+count*sizeof(void *));
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
  test_addr_list *al = test_calloc(1,sizeof(test_addr_list)+sizeof(void *));
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
                                gex_Rank_t nodeid, VEC_T *areaptr,
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
#define PRINT_VECTOR(dest, vector, nelem) do {                                            \
      assert(sizeof(dest) > 16);                                                          \
      char *_p = (dest);                                                                  \
      *_p++ = '[';                                                                        \
      for (size_t i=0; i < (nelem); i++) {                                                \
        snprintf(_p, sizeof(dest) - (_p - (dest))-3, "%lli", (long long int)(vector)[i]); \
        if (i < (nelem)-1) strcat(_p, ", ");                                              \
        _p+=strlen(_p);                                                                   \
      }                                                                                   \
      *_p++ = ']'; *_p = 0;                                                               \
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
  size_t dim = TEST_RAND(2, TEST_RAND(2, TEST_RAND(2, max_stridedim+1)));
  size_t sz;
  test_strided_desc *sd;
  size_t i;
  if (TEST_RAND_ONEIN(10)) dim = 1; /* 1-dim (fully contiguous) */
  sz = sizeof(test_strided_desc)+6*dim*sizeof(size_t);
  sd = test_calloc(1,sz);
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
                            gex_Rank_t nodeid, VEC_T *areaptr,
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
      PRINT_VECTOR(idxstr, idx, dim);
      ERR("mismatch at location %s\n"
          "  expected val="VEC_FMT"\n"
          "    actual val="VEC_FMT"\n"
          "  at %s:%s:%i\n",
          idxstr, VEC_STR(srcval), VEC_STR(resval),
        context, file, line);
      { size_t sz = gasnett_format_putsgets_bufsz(desc->stridelevels);
        char *buf = test_malloc(sz);
        gasnett_format_putsgets(buf, NULL, myteam, nodeid,
          desc->dstaddr, (ptrdiff_t*)desc->dststrides,
          desc->srcaddr, (ptrdiff_t*)desc->srcstrides,
          desc->count[0], desc->count+1, desc->stridelevels);
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
typedef struct {
  uint64_t checksum;
  size_t _descsz; /* in bytes */
  size_t totalsz; /* in bytes */
  size_t srcvolume; /* in bytes */
  size_t dstvolume; /* in bytes */
  void *srcaddr;
  void *dstaddr;
  void *contigaddr;
  size_t *srcextents; /* in elems */
  size_t *dstextents; /* in elems */
  size_t *dim_map; /* dim_map[dst_dim] == src_dim */
  size_t *rev_map; /* inverted dim_map */
  ptrdiff_t *srcstrides; /* in bytes */
  ptrdiff_t *dststrides; /* in bytes */
  ptrdiff_t *contigstrides; /* in bytes */
  size_t *count; 
  size_t    *verify_count;
  ptrdiff_t *verify_srcstrides;
  void  *verify_srcaddr;
  void  *verify_contigaddr;
  size_t stridelevels;
  size_t elemsz;
} test_xpose_desc;

void _verify_xpose_desc(test_xpose_desc const *xd, const char *file, int line) {
  if (xd->checksum != test_checksum(((uint64_t*)xd)+1, xd->_descsz-8))
    FATALERR("Checksum mismatch in verify_xpose_desc at %s:%i", file, line);

  size_t sz = xd->elemsz;
  size_t srcvol = sz;
  size_t dstvol = sz;
  uint8_t *srclo = xd->srcaddr;
  uint8_t *srchi = srclo; 
  uint8_t *dstlo = xd->dstaddr;
  uint8_t *dsthi = dstlo; 
  uint8_t *clo = xd->contigaddr;
  uint8_t *chi = clo; 
  int shuffled = (xd->dim_map == NULL);
  if (shuffled) {
    assert(!xd->dim_map && !xd->rev_map);
    assert(xd->verify_count && xd->verify_srcstrides);
  } else {
    assert(xd->dim_map && xd->rev_map);
    assert(!xd->verify_count && !xd->verify_srcstrides);
  }
  for (size_t i = 0; i < xd->stridelevels; i++) {
    if (!shuffled){
      assert_always(xd->rev_map[xd->dim_map[i]] == i);
      assert_always(xd->dim_map[xd->rev_map[i]] == i);
      assert_always(xd->contigstrides[i] == sz);
    }
    sz *= xd->count[i];
    assert_always(xd->count[i] <= xd->dstextents[i]);
    if (!shuffled) {
      assert_always(xd->count[i] <= xd->srcextents[xd->dim_map[i]]);
      assert_always(labs(xd->srcstrides[xd->rev_map[i]]) == srcvol);
      assert_always(labs(xd->dststrides[i]) == dstvol);
    }
    srcvol *= xd->srcextents[i];
    dstvol *= xd->dstextents[i];
    if (xd->srcstrides[i] >= 0) srchi += xd->srcstrides[i] * ((ptrdiff_t)xd->count[i] - 1);
    else                        srclo += xd->srcstrides[i] * ((ptrdiff_t)xd->count[i] - 1);
    if (xd->dststrides[i] >= 0) dsthi += xd->dststrides[i] * ((ptrdiff_t)xd->count[i] - 1);
    else                        dstlo += xd->dststrides[i] * ((ptrdiff_t)xd->count[i] - 1);
    if (xd->contigstrides[i] >= 0) chi += xd->contigstrides[i] * ((ptrdiff_t)xd->count[i] - 1);
    else                           clo += xd->contigstrides[i] * ((ptrdiff_t)xd->count[i] - 1);
  }

  assert_always(sz == xd->totalsz);
  if (sz != 0) assert_always(xd->elemsz >= VEC_SZ && xd->elemsz % VEC_SZ == 0);
  assert_always(srcvol == xd->srcvolume);
  assert_always(dstvol == xd->dstvolume);

  size_t srcbounds = srchi - srclo + xd->elemsz;
  size_t dstbounds = dsthi - dstlo + xd->elemsz;
  size_t cbounds = chi - clo + xd->elemsz;
  assert_always(srcbounds <= xd->srcvolume);
  assert_always(dstbounds <= xd->dstvolume);
  assert_always(cbounds == xd->totalsz);
}
#define verify_xpose_desc(xd) do {                        \
  if (verify) _verify_xpose_desc((xd), __FILE__, __LINE__); \
} while (0)

/* build an xpose set over the area 
   [srcaddr...srcaddr+elemlen*VEC_SZ] and [dstaddr...dstaddr+elemlen*VEC_SZ]
   note elemlen is a VEC_T element count
 */
test_xpose_desc *rand_xpose_desc(void *srcaddr, void *dstaddr, void *contigaddr, size_t elemlen) {
  size_t dim = TEST_RAND(2, TEST_RAND(2, max_stridedim));
  size_t sz = sizeof(test_xpose_desc)+10*dim*MAX(sizeof(ptrdiff_t),sizeof(size_t));
  test_xpose_desc *xd = test_calloc(1,sz);
  xd->_descsz = sz;
  xd->stridelevels = dim;
  size_t vecs_per_elem = TEST_RAND(1,TEST_RAND(1,16));
  xd->elemsz =        VEC_SZ*vecs_per_elem;
  elemlen /= vecs_per_elem;
  xd->srcstrides =    (ptrdiff_t *)(xd+1);
  xd->dststrides =    xd->srcstrides+dim;
  xd->contigstrides = xd->dststrides+dim;
  xd->count =         (size_t *)(xd->contigstrides+dim);
  xd->srcextents =    xd->count+dim;
  xd->dstextents =    xd->srcextents+dim;
  xd->dim_map =       xd->dstextents+dim;
  xd->rev_map =       xd->dim_map+dim;
  xd->verify_count =  xd->rev_map+dim;
  xd->verify_srcstrides = (ptrdiff_t*)xd->verify_count+dim;

  size_t volume = TEST_RAND(elemlen*7/8, elemlen); /* in elem */
  size_t srcmax = 0;
  size_t dstmax = 0;
  xd->srcextents[0] = volume;
  xd->dstextents[0] = volume;
  /* choose dimensional extents */
  for (size_t i = 1; i < dim; i++) {
    size_t prev,min,max;
    prev = xd->srcextents[srcmax];
    max = MAX(1,prev/2);
    min = MIN(2,max);
    xd->srcextents[i] = TEST_RAND(min,TEST_RAND(min, max));
    xd->srcextents[srcmax] /= xd->srcextents[i];
    xd->srcextents[i] = prev / xd->srcextents[srcmax];
    if (xd->srcextents[i] > xd->srcextents[srcmax]) srcmax = i;
    prev = xd->dstextents[dstmax];
    max = MAX(1,prev/2);
    min = MIN(2,max);
    xd->dstextents[i] = TEST_RAND(min,TEST_RAND(min, max));
    xd->dstextents[dstmax] /= xd->dstextents[i];
    xd->dstextents[i] = prev / xd->dstextents[dstmax];
    if (xd->dstextents[i] > xd->dstextents[dstmax]) dstmax = i;
  }
  SHUFFLE_ARRAY(size_t,xd->srcextents, dim);
  SHUFFLE_ARRAY(size_t,xd->dstextents, dim);

  // generate a transpositional map
  for (size_t i = 0; i < dim; i++) {
    xd->dim_map[i] = i;
  }
  #if !DISABLE_TRANSPOSE
    SHUFFLE_ARRAY(size_t,xd->dim_map, dim);
  #endif
  for (size_t i = 0; i < dim; i++) {
    xd->rev_map[xd->dim_map[i]] = i;
  }

  /* select counts */
  int maximizecnt = TEST_RAND_ONEIN(20);
  for (size_t i = 0; i < dim; i++) {
    size_t maxcnt = MIN(xd->srcextents[xd->dim_map[i]],xd->dstextents[i]);
    size_t mincnt = MIN(2,maxcnt);
    if (maximizecnt) xd->count[i] = maxcnt;
    else xd->count[i] = TEST_RAND(TEST_RAND(mincnt, maxcnt), maxcnt);
  }

  /* multiply up dimensions */
  xd->srcvolume = xd->elemsz;
  xd->dstvolume = xd->elemsz;
  for (size_t i = 0; i < dim; i++) {
    xd->srcstrides[xd->rev_map[i]] = xd->srcvolume;
    xd->srcvolume *= xd->srcextents[i];
    xd->dststrides[i] = xd->dstvolume;
    xd->dstvolume *= xd->dstextents[i];
  }
  assert(xd->srcvolume <= volume*xd->elemsz);
  assert(xd->dstvolume <= volume*xd->elemsz);
  xd->srcaddr = ((VEC_T*)srcaddr) + TEST_RAND(0, (elemlen*vecs_per_elem)-(xd->srcvolume/VEC_SZ));
  xd->dstaddr = ((VEC_T*)dstaddr) + TEST_RAND(0, (elemlen*vecs_per_elem)-(xd->dstvolume/VEC_SZ));

  /* compute count-dependent quantities */
  xd->totalsz = xd->elemsz;
  for (size_t i = 0; i < dim; i++) {
    xd->contigstrides[i] = (ptrdiff_t)xd->totalsz;
    xd->totalsz *= xd->count[i];
  }
  xd->contigaddr = ((VEC_T*)contigaddr) + TEST_RAND(0, (elemlen*vecs_per_elem)-(xd->totalsz/VEC_SZ));
  xd->verify_contigaddr = xd->contigaddr;

  #if !DISABLE_INVERSION
    #define INVERT_STRIDE(d, addr, strides, count) do {              \
        addr = (uint8_t*)addr + strides[d]*(ptrdiff_t)(count[d]-1);  \
        strides[d] = -strides[d];                                    \
    } while (0)
    if (TEST_RAND_ONEIN(2)) { // invert some src dimensions
      for (size_t i = 0; i < dim; i++) {
        if (TEST_RAND_ONEIN(2)) { // flip!
          INVERT_STRIDE(i, xd->srcaddr, xd->srcstrides, xd->count);
        }
      }
    }
  #endif
  xd->verify_srcaddr = xd->srcaddr;

  #if !DISABLE_SHUFFLE
  if (TEST_RAND_ONEIN(8)) { /* shuffle dimensions on all sides */
    // Swapping dimensions on both sides does not affect the semantics of a transfer,
    // it just permutes the canonical order in which elements are visited.
    // Verification uses a fixed linear element-wise traversal for the contig side, 
    // (ie we don't permute the order of the linear verification traversal)
    // so it needs to also traverse the source values element-wise in their original
    // un-permuted order, hence we preserve the unpermuted srcstrides/count for verification.
    memcpy(xd->verify_count,xd->count,sizeof(size_t)*dim);
    memcpy(xd->verify_srcstrides,xd->srcstrides,sizeof(ptrdiff_t)*dim);

    for (size_t a = 0; a < dim-1; a++) {
      size_t const b = TEST_RAND(a+1,dim-1);
      assert(a != b); assert(a < dim-1); assert(b <= dim-1);
      #define SWAP_ENTRY(array, type) do { \
        type tmp = array[a]; array[a] = array[b]; array[b] = tmp; \
      } while(0)
      SWAP_ENTRY(xd->srcstrides, ptrdiff_t);
      SWAP_ENTRY(xd->dststrides, ptrdiff_t);
      SWAP_ENTRY(xd->contigstrides, ptrdiff_t);
      SWAP_ENTRY(xd->count, size_t);
      SWAP_ENTRY(xd->srcextents, size_t);
      SWAP_ENTRY(xd->dstextents, size_t);
      // dim_map would need more significant fixup to remain correct post-shuffle, 
      // and we no longer really need it at this point
      xd->dim_map = NULL;
      xd->rev_map = NULL;
      #undef SWAP_ENTRY
     }
     #if !DISABLE_INVERSION
       if (TEST_RAND_ONEIN(2)) { // invert some dimensions all around
         for (size_t i = 0; i < dim; i++) {
           if (TEST_RAND_ONEIN(2)) { // flip!
             INVERT_STRIDE(i, xd->srcaddr, xd->srcstrides, xd->count);
             INVERT_STRIDE(i, xd->dstaddr, xd->dststrides, xd->count);
             INVERT_STRIDE(i, xd->contigaddr, xd->contigstrides, xd->count);
           }
         }
       }
     #endif
  } else {
    xd->verify_count = NULL;
    xd->verify_srcstrides = NULL;
  }
  #endif

  xd->checksum = test_checksum(((uint64_t*)xd)+1, xd->_descsz-8);
  verify_xpose_desc(xd);
  return xd;
}

void _verify_xpose_desc_data_both(test_xpose_desc *desc,
                            gex_Rank_t nodeid, VEC_T *areaptr,
                            const char *context, const char *file, int line) {
  size_t const dim = desc->stridelevels;
  size_t * const count = (desc->verify_count ? desc->verify_count : desc->count);
  ptrdiff_t * const srcstrides = (desc->verify_srcstrides ? desc->verify_srcstrides : desc->srcstrides);
  VEC_T const *resultp = desc->verify_contigaddr;
  ptrdiff_t srcoffset = 0; /* in bytes */
  size_t curdim = 0;
  size_t const vecs_per_elem = desc->elemsz / VEC_SZ;
  assert(vecs_per_elem * VEC_SZ == desc->elemsz);
  
  if (desc->totalsz == 0) return; /* empty */

  size_t * const idx = test_calloc(sizeof(size_t), dim);

  while (curdim < dim) {

   for (size_t v = 0; v < vecs_per_elem; v++) {
    /* body */
    VEC_T srcval;
    VEC_T resval = *resultp;
    ptrdiff_t srcboffset = srcoffset + v*VEC_SZ;
    ptrdiff_t srcvoffset = srcboffset/VEC_SZ;
    assert(srcvoffset * VEC_SZ == srcboffset);
    VEC_T *srcptr = ((VEC_T*)desc->verify_srcaddr) + srcvoffset;
    ptrdiff_t areavoffset = srcptr - areaptr;
    if (areaptr == NULL) /* local src */
      srcval = *srcptr;
    else { /* remote src */
      srcval = SEG_VALUE(nodeid, areavoffset);
    }
    if (srcval != resval) {
      char idxstr[255];
      PRINT_VECTOR(idxstr, idx, dim);
      char dimstr[255];
      if (desc->dim_map) PRINT_VECTOR(dimstr, desc->dim_map, dim);
      else strcpy(dimstr,"(shuffled)");
      ERR("mismatch at element %s.%i\n"
          "  srcboffset= 0x%lx (%li)\n"
          "  srcvoffset= 0x%lx (%li)\n"
          "  areavoffset=0x%lx (%li)\n"
          "  expected val="VEC_FMT"\n"
          "    actual val="VEC_FMT"\n"
          "  dim_map=%s\n"
          "  at %s:%s:%i\n",
          idxstr, (int)v, 
          (long int)srcboffset, (long int)srcboffset,
          (long int)srcvoffset, (long int)srcvoffset,
          (long int)areavoffset, (long int)areavoffset,
          VEC_STR(srcval), VEC_STR(resval),
          dimstr,
        context, file, line);
      { size_t sz = gasnett_format_putsgets_bufsz(desc->stridelevels);
        char *buf = test_malloc(sz);
        gasnett_format_putsgets(buf, NULL, myteam, nodeid,
          desc->dstaddr, desc->dststrides,
          desc->srcaddr, desc->srcstrides,
          desc->elemsz, desc->count, desc->stridelevels);
        ERR("strided desc: %s\n", buf);
      }
      FATALERR("testvis failed.");
    }
    resultp++;
   } // v

   /* increment */
   for (curdim = 0; curdim < dim; curdim++) {
     if (idx[curdim] < count[curdim]-1) {
       idx[curdim]++;
       srcoffset += srcstrides[curdim];
       assert(idx[curdim] < count[curdim]);
       break;
     } else {
       assert(idx[curdim] == count[curdim] - 1);
       idx[curdim] = 0;
       srcoffset -= srcstrides[curdim]*(ptrdiff_t)(count[curdim]-1);
     }
   }

  } // while

  test_free(idx);
}

#define verify_xpose_desc_data(desc,context) do {                                             \
  if (verify) _verify_xpose_desc_data_both((desc),mynode,NULL,(context),__FILE__,__LINE__); \
} while (0)

#define verify_xpose_desc_data_remote(desc,node,areaptr,context) do {                              \
  if (verify) _verify_xpose_desc_data_both((desc),(node),(areaptr),(context),__FILE__,__LINE__); \
} while (0)

/* ------------------------------------------------------------------------------------ */
VEC_T *myseg = NULL;
VEC_T *partnerseg = NULL;
VEC_T *heapseg = NULL;
size_t areasz; /* in elem */
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

// restore a section of my_heap_read_area after an overwrite test
void restore_heap_read_area(VEC_T *addr, size_t cnt) {
  ptrdiff_t const offset = addr - my_heap_read_area;
  assert(offset >= 0 && offset + cnt <= areasz);
  for (size_t i = 0; i < cnt; i++) 
    *(addr++) = HEAP_VALUE(mynode, offset+i);
}

/* ------------------------------------------------------------------------------------ */
typedef struct {
  uint32_t packetsz; // entire packet
  gex_Rank_t srcjobrank;
} test_pcheader_t;

#define PC_VALUE(node, idx)   ((((uint8_t)(node)&0xF) << 4) | (((uint8_t)(idx))&0xF))

size_t pcsend_cnt = 0;
test_pcheader_t *rand_pcheader(size_t *packetsz) {
  size_t sz;
  if (TEST_RAND_ONEIN(8)) sz = 0;
  else if (TEST_RAND_ONEIN(8)) sz = GEX_VIS_MAX_PEERCOMPLETION;
  else sz = TEST_RAND(0,GEX_VIS_MAX_PEERCOMPLETION);
  assert_always(sz <= GEX_VIS_MAX_PEERCOMPLETION);
  *packetsz = sz;
  pcsend_cnt++;
  if (sz == 0) return NULL;
  test_pcheader_t *pcheader = test_malloc(sz);
  if (sz < sizeof(test_pcheader_t)) { // degenerate header
    memset(pcheader, (uint8_t)sz, sz);
  } else {
    pcheader->packetsz = sz;
    pcheader->srcjobrank = mynode;
    uint8_t *payload = (uint8_t*)(pcheader+1);
    size_t payloadsz = sz-sizeof(test_pcheader_t);
    for (size_t i=0; i < payloadsz; i++) {
      payload[i] = PC_VALUE(mynode, i);
    }
  }
  return pcheader;
}

void verify_pcheader(test_pcheader_t *pcheader, size_t sz, gex_Rank_t srcjobrank) {
  assert_always(sz <= GEX_VIS_MAX_PEERCOMPLETION);
  if (!verify) return;
  if (sz < sizeof(test_pcheader_t)) { // degenerate header
    uint8_t *payload = (uint8_t*)pcheader;
    for (size_t i=0; i < sz; i++) {
      uint8_t actual = payload[i];
      uint8_t expect = sz;
      if (actual != sz) 
        ERR("peer completion payload (sz=%i) mismatch at element %i: expect=0x%02x actual=0x%02x\n",
            (int)sz,(int)i,expect,actual);
    }
  } else {
    assert_always(pcheader->packetsz == sz);
    assert_always(pcheader->srcjobrank == srcjobrank);
    uint8_t *payload = (uint8_t*)(pcheader+1);
    size_t payloadsz = sz-sizeof(test_pcheader_t);
    for (size_t i=0; i < payloadsz; i++) {
      uint8_t expect = PC_VALUE(srcjobrank, i);
      uint8_t actual = payload[i];
      if (expect != actual)
        ERR("peer completion payload (sz=%i) mismatch at element %i: expect=0x%02x actual=0x%02x\n",
            (int)sz,(int)i,expect,actual);
    }
  }
}

#define hidx_pcverify 250
gasnett_atomic_t pcarrival_cnt = gasnett_atomic_init(0);
GASNETT_EXTERNC void pcverify_reph(gex_Token_t token, void *buf, size_t nbytes);
gex_AM_Entry_t pcverify_reph_entry = 
  { hidx_pcverify, (gex_AM_Fn_t)pcverify_reph, GEX_FLAG_AM_REPLY|GEX_FLAG_AM_MEDIUM, 
    0, (void *)&pcarrival_cnt, "testvis_pcverify_reph" };
void pcverify_reph(gex_Token_t token, void *buf, size_t nbytes) {
  gasnett_atomic_increment(&pcarrival_cnt,0);
  gex_Rank_t nranks = gex_TM_QuerySize(myteam);
  gex_Rank_t mysender = (mynode + nranks - 1) % nranks;

  // check all token properties are legit
  gex_Token_Info_t info;
  gex_TI_t rc = gex_Token_Info(token, &info, GEX_TI_ALL);
  assert_always(rc & GEX_TI_SRCRANK); 
  assert_always(info.gex_srcrank == mysender);
  assert_always(rc & GEX_TI_EP); 
  assert_always(info.gex_ep == myep);
  if (rc & GEX_TI_IS_REQ)  assert_always(info.gex_is_req == 0);
  if (rc & GEX_TI_IS_LONG) assert_always(info.gex_is_long == 0);
  if (rc & GEX_TI_ENTRY) {
    #define CHECK_FIELD(fname) assert_always(info.gex_entry->fname == pcverify_reph_entry.fname)
    CHECK_FIELD(gex_index);
    CHECK_FIELD(gex_fnptr);
    CHECK_FIELD(gex_flags);
    CHECK_FIELD(gex_nargs);
    CHECK_FIELD(gex_cdata);
    CHECK_FIELD(gex_name);
    #undef CHECK_FIELD
  }

  verify_pcheader(buf, nbytes, mysender);
}

#ifndef PC_FREQ // 1 = always, 0 = never
#define PC_FREQ 8
#endif

// schedule a peer completion handler for next VIS injection, if the Fates deem
#define PC_SCHEDULE(p_pcheader) do {                                     \
  if (PC_FREQ == 0 || !TEST_RAND_ONEIN(PC_FREQ)) *(p_pcheader) = 0;      \
  else {                                                                 \
    size_t _pcsz;                                                        \
    test_pcheader_t *_pcaddr = rand_pcheader(&_pcsz);                    \
    gex_VIS_SetPeerCompletionHandler(hidx_pcverify, _pcaddr, _pcsz, 0);  \
    *(p_pcheader) = _pcaddr;                                             \
  }                                                                      \
} while (0)

// release any temp storage used for peer completion injection
#define PC_FREE(p_pcheader) (test_free(*(p_pcheader)),*(p_pcheader) = 0)

// collective operation to check all peer completions have arrived globally
void verify_pc_arrivals(void) {
  BARRIER();
  if (!verify) return;
  gex_RMA_PutBlocking(myteam, partner, partner_seg_remotewrite_area, &pcsend_cnt, sizeof(pcsend_cnt), 0);
  BARRIER();
  size_t expect = *(size_t*)my_seg_remotewrite_area;
  size_t actual = gasnett_atomic_read(&pcarrival_cnt,0);
  for (int i=0; i < 5 && actual < expect; i++) { // bounded stall for global quiescence of PC
    sleep(1); 
    actual = gasnett_atomic_read(&pcarrival_cnt,0);
  }
  if (actual != expect) {
    ERR("peer completion arrival mismatch expect=%i actual=%i\n", (int)expect,(int)actual);
  }
  BARRIER();
}
/* ------------------------------------------------------------------------------------ */

typedef struct {
  test_memvec_list *vsrc;
  test_memvec_list *vdst;
  test_memvec_list *vtmp;
  test_addr_list *isrc;
  test_addr_list *idst;
  test_addr_list *itmp;
  test_strided_desc *sdesc;
  VEC_T *stmpbuf;
  test_xpose_desc *xdesc;
  test_pcheader_t *pcheader;
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
  test_pcheader_t *pcheader = 0;
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
    MSG("Vector... (max_veclen=%i)",(int)max_veclen);
    for (iter = 0; iter < iters; iter++) {
      /* put test */
      { test_memvec_list *src;
        test_memvec_list *dst;
        test_memvec_list *tmp;

        VEC_T *src_area = TEST_RAND_PICK(my_heap_read_area, my_seg_read_area);
        src = rand_memvec_list(src_area, areasz, 1);
        dst = rand_memvec_list(partner_seg_remotewrite_area, areasz, 0);
        trim_memvec_list(src, dst);
        tmp = buildcontig_memvec_list(TEST_RAND_PICK(my_heap_write2_area, my_seg_write2_area), dst->totalsz/VEC_SZ, areasz);

        #define CALL(fn) fn(myteam, partner, dst->count, dst->list, src->count, src->list, flags)
        gex_Flags_t flags = 0;
        PC_SCHEDULE(&pcheader);
        if (src_area == my_heap_read_area && TEST_RAND_ONEIN(2)) { // LC overwrite test
          gex_Event_t RC = GEX_EVENT_INVALID;
          gex_Event_t LC = GEX_EVENT_INVALID;
          flags = GEX_FLAG_ENABLE_LEAF_LC;
          int const flavor = TEST_RAND(1,3);
          switch (flavor) {
            case 1: // NB
              RC = CALL(gex_VIS_VectorPutNB);
              LC = gex_Event_QueryLeaf(RC, GEX_EC_LC);
              break;
            case 2: // NBI
              CALL(gex_VIS_VectorPutNBI);
              break;
            case 3: // NBI-AR
              gex_NBI_BeginAccessRegion(0);
              CALL(gex_VIS_VectorPutNBI);
              RC = gex_NBI_EndAccessRegion(0);
              LC = gex_Event_QueryLeaf(RC, GEX_EC_LC);
              break;
          }
          if (LC) gex_Event_Wait(LC);
          else gex_NBI_Wait(GEX_EC_LC, 0);

          for (ptrdiff_t i=src->count-1; i >= 0; i--) // clear
            if (src->list[i].gex_len)
              memset(src->list[i].gex_addr, 0, src->list[i].gex_len);

          if (RC) gex_Event_Wait(RC);
          else gex_NBI_Wait(GEX_EC_PUT, 0);

          for (size_t i=0; i < src->count; i++) // restore
            if (src->list[i].gex_len)
              restore_heap_read_area(src->list[i].gex_addr, src->list[i].gex_len/VEC_SZ);

        } else {
          TIMED_PUT(CALL(gex_VIS_VectorPutBlocking),dst->totalsz);
        }
        #undef CALL
        verify_memvec_list(src);
        verify_memvec_list(dst);
        TIMED_GET(gex_VIS_VectorGetBlocking(myteam, tmp->count, tmp->list, partner, dst->count, dst->list, 0),dst->totalsz);
        verify_memvec_list(tmp);
        verify_memvec_list(dst);
        verify_memvec_data(src, tmp->list[0].gex_addr, "gasnet_putv_bulk/gasnet_getv_bulk test");
        test_free(src);
        test_free(dst);
        test_free(tmp);
        PC_FREE(&pcheader);
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

        gex_VIS_VectorGetBlocking(myteam, dst->count, dst->list, partner, src->count, src->list, 0);
        verify_memvec_list(src);
        verify_memvec_list(dst);
        if ((segeverything || dstarea == my_seg_write1_area) && 
            TEST_RAND_PICK(0,1)) {
          gex_VIS_VectorGetBlocking(myteam, tmp->count, tmp->list, mynode, dst->count, dst->list, 0);
        } else {
          gex_VIS_VectorPutBlocking(myteam, mynode, tmp->count, tmp->list, dst->count, dst->list, 0);
        }
        verify_memvec_list(tmp);
        verify_memvec_list(dst);
        verify_memvec_data_remote(src, tmp->list[0].gex_addr, partner, partner_seg_read_area, "gasnet_getv_bulk test");
        test_free(src);
        test_free(dst);
        test_free(tmp);
      }
      TEST_PROGRESS_BAR(iter, iters);
    }
    BARRIER();
    checkmem();
    TIME_OUTPUT(v);
  } else BARRIER();
  verify_pc_arrivals();
  BARRIER();
  /*---------------------------------------------------------------------------------*/
  if (runtests & RUN_INDEXED) { 
    int iter;
    TIME_DECL();
    MSG("Indexed... (max_idxlen=%i, max_chunksz=%i)",(int)max_idxlen,(int)max_chunksz);
    for (iter = 0; iter < iters; iter++) {
      /* put test */
      { test_addr_list *src;
        test_addr_list *dst;
        test_addr_list *tmp;
        size_t srcchunkelem, dstchunkelem;

        rand_chunkelem(&srcchunkelem, &dstchunkelem);
        VEC_T *src_area = TEST_RAND_PICK(my_heap_read_area, my_seg_read_area);
        src = rand_addr_list(src_area, srcchunkelem, areasz, 1);
        dst = rand_addr_list(partner_seg_remotewrite_area, dstchunkelem, areasz, 0);
        trim_addr_list(src, dst);
        tmp = buildcontig_addr_list(TEST_RAND_PICK(my_heap_write2_area, my_seg_write2_area), dst->totalsz/VEC_SZ, areasz);

        #define CALL(fn) fn(myteam, partner, dst->count, dst->list, dst->chunklen, src->count, src->list, src->chunklen, flags)
        gex_Flags_t flags = 0;
        PC_SCHEDULE(&pcheader);
        if (src_area == my_heap_read_area && TEST_RAND_ONEIN(2)) { // LC overwrite test
          gex_Event_t RC = GEX_EVENT_INVALID;
          gex_Event_t LC = GEX_EVENT_INVALID;
          flags = GEX_FLAG_ENABLE_LEAF_LC;
          int const flavor = TEST_RAND(1,3);
          switch (flavor) {
            case 1: // NB
              RC = CALL(gex_VIS_IndexedPutNB);
              LC = gex_Event_QueryLeaf(RC, GEX_EC_LC);
              break;
            case 2: // NBI
              CALL(gex_VIS_IndexedPutNBI);
              break;
            case 3: // NBI-AR
              gex_NBI_BeginAccessRegion(0);
              CALL(gex_VIS_IndexedPutNBI);
              RC = gex_NBI_EndAccessRegion(0);
              LC = gex_Event_QueryLeaf(RC, GEX_EC_LC);
              break;
          }
          if (LC) gex_Event_Wait(LC);
          else gex_NBI_Wait(GEX_EC_LC, 0);

          if (src->chunklen)
            for (ptrdiff_t i=src->count-1; i >= 0; i--) // clear
              memset(src->list[i], 0, src->chunklen);

          if (RC) gex_Event_Wait(RC);
          else gex_NBI_Wait(GEX_EC_PUT, 0);

          if (src->chunklen)
            for (size_t i=0; i < src->count; i++) // restore
              restore_heap_read_area(src->list[i], src->chunklen/VEC_SZ);

        } else {
          TIMED_PUT(CALL(gex_VIS_IndexedPutBlocking),dst->totalsz);
        }
        #undef CALL
        verify_addr_list(src);
        verify_addr_list(dst);
        TIMED_GET(gex_VIS_IndexedGetBlocking(myteam, tmp->count, tmp->list, tmp->chunklen, partner, dst->count, dst->list, dst->chunklen, 0),dst->totalsz);
        verify_addr_list(tmp);
        verify_addr_list(dst);
        verify_addr_list_data(src, tmp->list[0], "gasnet_puti_bulk/gasnet_geti_bulk test");
        test_free(src);
        test_free(dst);
        test_free(tmp);
        PC_FREE(&pcheader);
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

        gex_VIS_IndexedGetBlocking(myteam, dst->count, dst->list, dst->chunklen, partner, src->count, src->list, src->chunklen, 0);
        verify_addr_list(src);
        verify_addr_list(dst);
        if ((segeverything || dstarea == my_seg_write1_area) && 
            TEST_RAND_PICK(0,1)) {
          gex_VIS_IndexedGetBlocking(myteam, tmp->count, tmp->list, tmp->chunklen, mynode, dst->count, dst->list, dst->chunklen, 0);
        } else {
          gex_VIS_IndexedPutBlocking(myteam, mynode, tmp->count, tmp->list, tmp->chunklen, dst->count, dst->list, dst->chunklen, 0);
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
    BARRIER();
    checkmem();
    TIME_OUTPUT(i);
  } else BARRIER();
  verify_pc_arrivals();
  BARRIER();
  /*---------------------------------------------------------------------------------*/
  if (runtests & RUN_STRIDED) { 
    int iter;
    TIME_DECL();
    MSG("Strided (linear)... (max_stridedim=%i)",(int)max_stridedim);
    for (iter = 0; iter < iters; iter++) {
      /* put test */
      { test_strided_desc *desc;
        void *srcarea = TEST_RAND_PICK(my_heap_read_area, my_seg_read_area);
        void *dstarea = partner_seg_remotewrite_area;
        void *tmparea = TEST_RAND_PICK(my_heap_write2_area, my_seg_write2_area);
        VEC_T *tmpbuf;

        desc = rand_strided_desc(srcarea, dstarea, tmparea, areasz);
        tmpbuf = ((VEC_T*)tmparea) + TEST_RAND(0,areasz - desc->totalsz/VEC_SZ);  // randomized tmpbuf position

        // push strided local data to strided peer segment
        #define CALL(fn) fn(myteam, partner, desc->dstaddr, (ptrdiff_t*)desc->dststrides, desc->srcaddr, (ptrdiff_t*)desc->srcstrides, desc->count[0], desc->count+1, desc->stridelevels, flags)
        gex_Flags_t flags = 0;
        PC_SCHEDULE(&pcheader);
        if (srcarea == my_heap_read_area && TEST_RAND_ONEIN(2) && desc->totalsz > 0) { // LC overwrite test
          gex_Event_t RC = GEX_EVENT_INVALID;
          gex_Event_t LC = GEX_EVENT_INVALID;
          flags = GEX_FLAG_ENABLE_LEAF_LC;
          int const flavor = TEST_RAND(1,3);
          switch (flavor) {
            case 1: // NB
              RC = CALL(gex_VIS_StridedPutNB);
              LC = gex_Event_QueryLeaf(RC, GEX_EC_LC);
              break;
            case 2: // NBI
              CALL(gex_VIS_StridedPutNBI);
              break;
            case 3: // NBI-AR
              gex_NBI_BeginAccessRegion(0);
              CALL(gex_VIS_StridedPutNBI);
              RC = gex_NBI_EndAccessRegion(0);
              LC = gex_Event_QueryLeaf(RC, GEX_EC_LC);
              break;
          }
          if (LC) gex_Event_Wait(LC);
          else gex_NBI_Wait(GEX_EC_LC, 0);

          // clear
          memset(desc->srcaddr, 0, desc->count[0]);

          if (RC) gex_Event_Wait(RC);
          else gex_NBI_Wait(GEX_EC_PUT, 0);

          // restore
          restore_heap_read_area(desc->srcaddr, desc->count[0]/VEC_SZ);

        } else {
          TIMED_PUT(CALL(gex_VIS_StridedPutBlocking),desc->totalsz);
        }
        #undef CALL
        verify_strided_desc(desc);
        // pull it back to contiguous local tmp
        TIMED_GET(gex_VIS_StridedGetBlocking(myteam, tmpbuf, (ptrdiff_t*)desc->contigstrides, partner, desc->dstaddr, (ptrdiff_t*)desc->dststrides, desc->count[0], desc->count+1, desc->stridelevels, 0),desc->totalsz);
        verify_strided_desc(desc);
        verify_strided_desc_data(desc, tmpbuf, "gex_VIS_Strided{Put,Get}Blocking linear test");
        test_free(desc);
        PC_FREE(&pcheader);
      }

      /* get test */
      { test_strided_desc *desc;
        void *srcarea = partner_seg_read_area;
        void *dstarea = TEST_RAND_PICK(my_heap_write1_area, my_seg_write1_area);
        void *tmparea = my_seg_write2_area;
        VEC_T *tmpbuf;

        desc = rand_strided_desc(srcarea, dstarea, tmparea, areasz);
        tmpbuf = ((VEC_T*)tmparea) + TEST_RAND(0,areasz - desc->totalsz/VEC_SZ); // randomized tmpbuf position

        // pull strided peer data to strided local area
        gex_VIS_StridedGetBlocking(myteam, desc->dstaddr, (ptrdiff_t*)desc->dststrides, partner, desc->srcaddr, (ptrdiff_t*)desc->srcstrides, desc->count[0], desc->count+1, desc->stridelevels, 0);
        verify_strided_desc(desc);
        if ((segeverything || dstarea == my_seg_write1_area) && 
            TEST_RAND_PICK(0,1)) { // loopback get: strided local data to contiguous local tmp
          gex_VIS_StridedGetBlocking(myteam, tmpbuf, (ptrdiff_t*)desc->contigstrides, mynode, desc->dstaddr, (ptrdiff_t*)desc->dststrides, desc->count[0], desc->count+1, desc->stridelevels, 0);
        } else { // loopback put: strided local data to contiguous local tmp
          gex_VIS_StridedPutBlocking(myteam, mynode, tmpbuf, (ptrdiff_t*)desc->contigstrides, desc->dstaddr, (ptrdiff_t*)desc->dststrides, desc->count[0], desc->count+1, desc->stridelevels, 0);
        }
        verify_strided_desc(desc);
        verify_strided_desc_data_remote(desc, tmpbuf, partner, partner_seg_read_area, "gex_VIS_StridedGetBlocking linear test");
        test_free(desc);
      }
      TEST_PROGRESS_BAR(iter, iters);
    }
    BARRIER();
    checkmem();
    TIME_OUTPUT(s);
  } else BARRIER();
  verify_pc_arrivals();
  BARRIER();
  /*---------------------------------------------------------------------------------*/
  if ((runtests & RUN_TRANSPOSE) && max_stridedim >= 2) { 
    TIME_DECL();
    MSG("Strided (transpositional)... (max_stridedim=%i)",(int)max_stridedim);
    for (int iter = 0; iter < iters; iter++) {

      /* get test */
      { void *srcarea = partner_seg_read_area;
        void *dstarea = NULL; // not used
        void *tmparea = TEST_RAND_PICK(my_heap_write1_area, my_seg_write1_area);

        test_xpose_desc *desc = rand_xpose_desc(srcarea, dstarea, tmparea, areasz);

        // pull strided peer data to contig local area
        gex_VIS_StridedGetBlocking(myteam, desc->contigaddr, desc->contigstrides, partner, desc->srcaddr, desc->srcstrides, desc->elemsz, desc->count, desc->stridelevels, 0);
        verify_xpose_desc(desc);
        verify_xpose_desc_data_remote(desc, partner, partner_seg_read_area, "gex_VIS_StridedGetBlocking transpose test");
        test_free(desc);
      }

      /* put/get test */
      { void *srcarea = TEST_RAND_PICK(my_heap_read_area, my_seg_read_area);
        void *dstarea = partner_seg_remotewrite_area;
        void *tmparea = TEST_RAND_PICK(my_heap_write2_area, my_seg_write2_area);

        test_xpose_desc *desc = rand_xpose_desc(srcarea, dstarea, tmparea, areasz);

        // push strided local data to strided peer segment
        PC_SCHEDULE(&pcheader);
        TIMED_PUT(gex_VIS_StridedPutBlocking(myteam, partner, desc->dstaddr, desc->dststrides, desc->srcaddr, desc->srcstrides, desc->elemsz, desc->count, desc->stridelevels, 0),desc->totalsz);
        verify_xpose_desc(desc);
        // pull it back to contiguous local tmp
        TIMED_GET(gex_VIS_StridedGetBlocking(myteam, desc->contigaddr, desc->contigstrides, partner, desc->dstaddr, desc->dststrides, desc->elemsz, desc->count, desc->stridelevels, 0),desc->totalsz);
        verify_xpose_desc(desc);
        verify_xpose_desc_data(desc, "gex_VIS_Strided{Put,Get}Blocking transpose test");
        test_free(desc);
        PC_FREE(&pcheader);
      }

      /* get/pack test */
      { void *srcarea = partner_seg_read_area;
        void *dstarea = TEST_RAND_PICK(my_heap_write1_area, my_seg_write1_area);
        void *tmparea = my_seg_write2_area;

        test_xpose_desc *desc = rand_xpose_desc(srcarea, dstarea, tmparea, areasz);

        // pull strided peer data to strided local area
        gex_VIS_StridedGetBlocking(myteam, desc->dstaddr, desc->dststrides, partner, desc->srcaddr, desc->srcstrides, desc->elemsz, desc->count, desc->stridelevels, 0);
        verify_xpose_desc(desc);
        if ((segeverything || dstarea == my_seg_write1_area) && 
            TEST_RAND_PICK(0,1)) { // loopback get: strided local data to contiguous local tmp
          gex_VIS_StridedGetBlocking(myteam, desc->contigaddr, desc->contigstrides, mynode, desc->dstaddr, desc->dststrides, desc->elemsz, desc->count, desc->stridelevels, 0);
        } else { // loopback put: strided local data to contiguous local tmp
          gex_VIS_StridedPutBlocking(myteam, mynode, desc->contigaddr, desc->contigstrides, desc->dstaddr, desc->dststrides, desc->elemsz, desc->count, desc->stridelevels, 0);
        }
        verify_xpose_desc(desc);
        verify_xpose_desc_data_remote(desc, partner, partner_seg_read_area, "gex_VIS_StridedGetBlocking/pack transpose test");
        test_free(desc);
      }
      TEST_PROGRESS_BAR(iter, iters);
    }
    BARRIER();
    checkmem();
    TIME_OUTPUT(x);
  } else BARRIER();
  verify_pc_arrivals();
  BARRIER();
  /*---------------------------------------------------------------------------------*/
  if (runtests & RUN_NB) { 
    int iter;
    MSG("Non-blocking tests... (max_inflight_ops=%i)",(int)max_inflight_ops);
    for (iter = 0; iter < iters; iter++) {
      size_t numops = TEST_RAND(1, max_inflight_ops);
      gex_Event_t *events = test_calloc(sizeof(gex_Event_t), numops);
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

        int last_case = 4;
        if (max_stridedim < 2) last_case = 3;
        gex_Flags_t flags = TEST_RAND_PICK(0,GEX_FLAG_IMMEDIATE); 
        if (!(flags & GEX_FLAG_IMMEDIATE)) PC_SCHEDULE(&(ops[i].pcheader)); // peer completion currently prohibited with IMMEDIATE
        int fail;
        switch (TEST_RAND(1,last_case)) {
          case 1: {
            ops[i].vsrc = rand_memvec_list(TEST_RAND_PICK(my_heap_read_area, my_seg_read_area), areasz, 1);
            ops[i].vdst = rand_memvec_list(op_partner_seg_remotewrite_area, opareasz, 0);
            trim_memvec_list(ops[i].vsrc, ops[i].vdst);
            ops[i].vtmp = buildcontig_memvec_list(TEST_RAND_PICK(op_my_heap_write2_area, op_my_seg_write2_area), ops[i].vdst->totalsz/VEC_SZ, opareasz);
            
            reputv: 
            if (TEST_RAND_ONEIN(2)) {
               events[i] = gex_VIS_VectorPutNB(myteam, partner, ops[i].vdst->count, ops[i].vdst->list, ops[i].vsrc->count, ops[i].vsrc->list, flags);
               fail = (events[i] == GEX_EVENT_NO_OP);
            } else fail = gex_VIS_VectorPutNBI(myteam, partner, ops[i].vdst->count, ops[i].vdst->list, ops[i].vsrc->count, ops[i].vsrc->list, flags);
            verify_memvec_list(ops[i].vsrc);
            verify_memvec_list(ops[i].vdst);
            if (fail) { assert_always(flags & GEX_FLAG_IMMEDIATE); events[i] = 0; flags &= ~GEX_FLAG_IMMEDIATE; goto reputv; }
            break;
          }
          case 2: {
            size_t srcchunkelem, dstchunkelem;
            rand_chunkelem(&srcchunkelem, &dstchunkelem);

            ops[i].isrc = rand_addr_list(TEST_RAND_PICK(my_heap_read_area, my_seg_read_area), srcchunkelem, areasz, 1);
            ops[i].idst = rand_addr_list(op_partner_seg_remotewrite_area, dstchunkelem, opareasz, 0);
            trim_addr_list(ops[i].isrc, ops[i].idst);
            ops[i].itmp = buildcontig_addr_list(TEST_RAND_PICK(op_my_heap_write2_area, op_my_seg_write2_area), ops[i].idst->totalsz/VEC_SZ, opareasz);

            reputi:
            if (TEST_RAND_ONEIN(2)) {
               events[i] = gex_VIS_IndexedPutNB(myteam, partner, ops[i].idst->count, ops[i].idst->list, ops[i].idst->chunklen, ops[i].isrc->count, ops[i].isrc->list, ops[i].isrc->chunklen, flags);
               fail = (events[i] == GEX_EVENT_NO_OP);
            } else fail = gex_VIS_IndexedPutNBI(myteam, partner, ops[i].idst->count, ops[i].idst->list, ops[i].idst->chunklen, ops[i].isrc->count, ops[i].isrc->list, ops[i].isrc->chunklen, flags);
            verify_addr_list(ops[i].isrc);
            verify_addr_list(ops[i].idst);
            if (fail) { assert_always(flags & GEX_FLAG_IMMEDIATE); events[i] = 0; flags &= ~GEX_FLAG_IMMEDIATE; goto reputi; }
            break;
          }
          case 3: {
            void *srcarea = TEST_RAND_PICK(my_heap_read_area, my_seg_read_area);
            void *dstarea = op_partner_seg_remotewrite_area;
            void *tmparea = TEST_RAND_PICK(op_my_heap_write2_area, op_my_seg_write2_area);

            ops[i].sdesc = rand_strided_desc(srcarea, dstarea, tmparea, opareasz);
            ops[i].stmpbuf = ((VEC_T*)tmparea) + TEST_RAND(0,opareasz - ops[i].sdesc->totalsz/VEC_SZ);

            reputs:
            if (TEST_RAND_ONEIN(2)) {
               events[i] = gex_VIS_StridedPutNB(myteam, partner, ops[i].sdesc->dstaddr, (ptrdiff_t*)ops[i].sdesc->dststrides, ops[i].sdesc->srcaddr, (ptrdiff_t*)ops[i].sdesc->srcstrides, ops[i].sdesc->count[0], ops[i].sdesc->count+1, ops[i].sdesc->stridelevels, flags);
               fail = (events[i] == GEX_EVENT_NO_OP);
            } else fail = gex_VIS_StridedPutNBI(myteam, partner, ops[i].sdesc->dstaddr, (ptrdiff_t*)ops[i].sdesc->dststrides, ops[i].sdesc->srcaddr, (ptrdiff_t*)ops[i].sdesc->srcstrides, ops[i].sdesc->count[0], ops[i].sdesc->count+1, ops[i].sdesc->stridelevels, flags);
            verify_strided_desc(ops[i].sdesc);
            if (fail) { assert_always(flags & GEX_FLAG_IMMEDIATE); events[i] = 0; flags &= ~GEX_FLAG_IMMEDIATE; goto reputs; }
            break;
          }
          case 4: {
            void *srcarea = TEST_RAND_PICK(my_heap_read_area, my_seg_read_area);
            void *dstarea = op_partner_seg_remotewrite_area;
            void *tmparea = TEST_RAND_PICK(op_my_heap_write2_area, op_my_seg_write2_area);

            ops[i].xdesc = rand_xpose_desc(srcarea, dstarea, tmparea, opareasz);

            reputx:
            if (TEST_RAND_ONEIN(2)) {
               events[i] = gex_VIS_StridedPutNB(myteam, partner, ops[i].xdesc->dstaddr, ops[i].xdesc->dststrides, ops[i].xdesc->srcaddr, ops[i].xdesc->srcstrides, ops[i].xdesc->elemsz, ops[i].xdesc->count, ops[i].xdesc->stridelevels, flags);
               fail = (events[i] == GEX_EVENT_NO_OP);
            } else fail = gex_VIS_StridedPutNBI(myteam, partner, ops[i].xdesc->dstaddr, ops[i].xdesc->dststrides, ops[i].xdesc->srcaddr, ops[i].xdesc->srcstrides, ops[i].xdesc->elemsz, ops[i].xdesc->count, ops[i].xdesc->stridelevels, flags);
            verify_xpose_desc(ops[i].xdesc);
            if (fail) { assert_always(flags & GEX_FLAG_IMMEDIATE); events[i] = 0; flags &= ~GEX_FLAG_IMMEDIATE; goto reputx; }
            break;
          }
        }
      }

      /* sync */
      gex_Event_WaitAll(events, numops, 0);
      gex_NBI_Wait(GEX_EC_ALL,0);

      for(i=0; i < numops; i++) PC_FREE(&(ops[i].pcheader)); 

      /* gets */
      for(i=0; i < numops; i++) {
        gex_Flags_t flags = TEST_RAND_PICK(0,GEX_FLAG_IMMEDIATE); 
        int fail;
        if (ops[i].vsrc != NULL) {
          assert(ops[i].vdst != NULL && ops[i].vtmp != NULL);

          regetv:
          if (TEST_RAND_ONEIN(2)) {
             events[i] = gex_VIS_VectorGetNB(myteam, ops[i].vtmp->count, ops[i].vtmp->list, partner, ops[i].vdst->count, ops[i].vdst->list, flags);
             fail = (events[i] == GEX_EVENT_NO_OP);
          } else fail = gex_VIS_VectorGetNBI(myteam, ops[i].vtmp->count, ops[i].vtmp->list, partner, ops[i].vdst->count, ops[i].vdst->list, flags);
          verify_memvec_list(ops[i].vtmp);
          verify_memvec_list(ops[i].vdst);
          if (fail) { assert_always(flags & GEX_FLAG_IMMEDIATE); events[i] = 0; flags &= ~GEX_FLAG_IMMEDIATE; goto regetv; }
        } else if (ops[i].isrc != NULL) {
          assert(ops[i].idst != NULL && ops[i].itmp != NULL);

          regeti:
          if (TEST_RAND_ONEIN(2)) {
             events[i] = gex_VIS_IndexedGetNB(myteam, ops[i].itmp->count, ops[i].itmp->list, ops[i].itmp->chunklen, partner, ops[i].idst->count, ops[i].idst->list, ops[i].idst->chunklen, flags);
             fail = (events[i] == GEX_EVENT_NO_OP);
          } else fail = gex_VIS_IndexedGetNBI(myteam, ops[i].itmp->count, ops[i].itmp->list, ops[i].itmp->chunklen, partner, ops[i].idst->count, ops[i].idst->list, ops[i].idst->chunklen, flags);
          verify_addr_list(ops[i].itmp);
          verify_addr_list(ops[i].idst);
          if (fail) { assert_always(flags & GEX_FLAG_IMMEDIATE); events[i] = 0; flags &= ~GEX_FLAG_IMMEDIATE; goto regeti; }
        } else if (ops[i].sdesc != NULL) {

          regets:
          if (TEST_RAND_ONEIN(2)) {
             events[i] = gex_VIS_StridedGetNB(myteam, ops[i].stmpbuf, (ptrdiff_t*)ops[i].sdesc->contigstrides, partner, ops[i].sdesc->dstaddr, (ptrdiff_t*)ops[i].sdesc->dststrides, ops[i].sdesc->count[0], ops[i].sdesc->count+1, ops[i].sdesc->stridelevels, flags);
             fail = (events[i] == GEX_EVENT_NO_OP);
          } else fail = gex_VIS_StridedGetNBI(myteam, ops[i].stmpbuf, (ptrdiff_t*)ops[i].sdesc->contigstrides, partner, ops[i].sdesc->dstaddr, (ptrdiff_t*)ops[i].sdesc->dststrides, ops[i].sdesc->count[0], ops[i].sdesc->count+1, ops[i].sdesc->stridelevels, flags);
          verify_strided_desc(ops[i].sdesc);
          if (fail) { assert_always(flags & GEX_FLAG_IMMEDIATE); events[i] = 0; flags &= ~GEX_FLAG_IMMEDIATE; goto regets; }
        } else {
          assert(ops[i].xdesc != NULL);

          regetx:
          if (TEST_RAND_ONEIN(2)) {
             events[i] = gex_VIS_StridedGetNB(myteam, ops[i].xdesc->contigaddr, ops[i].xdesc->contigstrides, partner, ops[i].xdesc->dstaddr, ops[i].xdesc->dststrides, ops[i].xdesc->elemsz, ops[i].xdesc->count, ops[i].xdesc->stridelevels, flags);
             fail = (events[i] == GEX_EVENT_NO_OP);
          } else fail = gex_VIS_StridedGetNBI(myteam, ops[i].xdesc->contigaddr, ops[i].xdesc->contigstrides, partner, ops[i].xdesc->dstaddr, ops[i].xdesc->dststrides, ops[i].xdesc->elemsz, ops[i].xdesc->count, ops[i].xdesc->stridelevels, flags);
          verify_xpose_desc(ops[i].xdesc);
          if (fail) { assert_always(flags & GEX_FLAG_IMMEDIATE); events[i] = 0; flags &= ~GEX_FLAG_IMMEDIATE; goto regetx; }
        }

      }

      /* sync */
      gex_Event_WaitAll(events, numops, 0);
      gex_NBI_Wait(GEX_EC_ALL,0);

      for(i=0; i < numops; i++) {
        if (ops[i].vsrc != NULL) {
          assert(ops[i].vdst != NULL && ops[i].vtmp != NULL);

          verify_memvec_list(ops[i].vsrc);
          verify_memvec_list(ops[i].vdst);
          verify_memvec_list(ops[i].vtmp);
          verify_memvec_data(ops[i].vsrc, ops[i].vtmp->list[0].gex_addr, "non-blocking gasnet_putv_bulk/gasnet_getv_bulk test");

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
        } else if (ops[i].sdesc) {
          assert(ops[i].sdesc != NULL && ops[i].stmpbuf != NULL);

          verify_strided_desc_data(ops[i].sdesc, ops[i].stmpbuf, "gasnet_puts_bulk/gasnet_gets_bulk test");

          test_free(ops[i].sdesc);
        } else {
          assert(ops[i].xdesc);
          verify_xpose_desc_data(ops[i].xdesc, "gasnet_puts_bulk/gasnet_gets_bulk test");

          test_free(ops[i].xdesc);
        }
      }
      test_free(events);
      test_free(ops);
      TEST_PROGRESS_BAR(iter, iters);
    }
    BARRIER();
    checkmem();
  } else BARRIER();
  verify_pc_arrivals();
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
  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testvis", &argc, &argv, 0));
  test_init_early("testvis",0, "[options] (iters) (seed)\n"
            " -v/-i/-s/-x/-n  run vector/indexed/strided/transpositional/non-blocking tests (defaults to all)\n"
            " -d        disable correctness verification checks\n"
            " -o        one-way (half duplex) mode\n"
            " -t        enable timing output\n"
            " -mm <n>   segment space to use (in MB)\n"
            " -mv <n>   max vector length (in elements)\n"
            " -mi <n>   max index addr list length (in elements)\n"
            " -ms <n>   max striding dimensions\n"
            " -mn <n>   max in-flight non-blocking operations\n"
            " -mc <n>   max chunk size (in bytes)\n"
            " iters     number of testing iterations\n"
            " seed      seed offset for PRNG \n"
            );

  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && (argv[i][1] == 'm' || argv[i][1] == 'M')) {
      char argtype = argv[i][2];
      char *valptr = &argv[i][3];
      if (!*valptr) {
        if (i+1 < argc) valptr = argv[++i];
        else test_usage_early();
      }
      int64_t argval = gasnett_parse_int(valptr, 0);
      if (argval == 0) test_usage_early();
      switch(argtype) { // -m*
          case 'm': case 'M': segsz = gasnett_parse_int(valptr, 1024*1024); break;
          case 'v': case 'V': max_veclen = argval; break;
          case 'i': case 'I': max_idxlen = argval; break;
          case 's': case 'S': max_stridedim = argval; break;
          case 'c': case 'C': max_chunksz = gasnett_parse_int(valptr, 1); break;
          case 'n': case 'N': max_inflight_ops = argval; break;
          default: test_usage_early();
      }
    } else if (argv[i][0] == '-') {
      int j;
      for (j = 1; argv[i][j]; j++) {
        switch(argv[i][j]) {
          case 'v': case 'V': runtests |= RUN_VECTOR; break;
          case 'i': case 'I': runtests |= RUN_INDEXED; break;
          case 's': case 'S': runtests |= RUN_STRIDED; break;
          case 'x': case 'X': runtests |= RUN_TRANSPOSE; break;
          case 'n': case 'N': runtests |= RUN_NB; break;
          case 'd': case 'D': verify = 0; break;
          case 'o': case 'O': halfduplex = 1; break;
          case 't': case 'T': showtiming = 1; break;
          default: test_usage_early();
        }
      }
    } else break;
  }
  int runall = RUN_VECTOR | RUN_INDEXED | RUN_STRIDED | RUN_TRANSPOSE | RUN_NB;
  if (runtests == 0) runtests = runall;
  if (i < argc) { iters = atoi(argv[i]); i++; }
  if (i < argc) { seedoffset = atoi(argv[i]); i++; }
  if (i < argc) test_usage_early();
  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));

  areasz = TEST_SEGSZ/NUM_AREAS/VEC_SZ; /* in elem */
  mynode = gex_TM_QueryRank(myteam);
  myseg = TEST_SEG(mynode);
  partner = (mynode + 1) % gex_TM_QuerySize(myteam);
  partnerseg = TEST_SEG(partner);
  heapseg = (VEC_T *)test_malloc(TEST_SEGSZ);

  assert_always(gex_EP_RegisterHandlers(myep, &pcverify_reph_entry, 1) == GASNET_OK);

  if (seedoffset == 0) {
    seedoffset = (((unsigned int)TIME()) & 0xFFFF);
    TEST_BCAST(&seedoffset, 0, &seedoffset, sizeof(seedoffset));
  }
  TEST_SRAND(mynode+seedoffset);
  char segstr[64];
  gasnett_format_number(segsz, segstr, sizeof(segstr), 1);
  MSG("running %i iterations of %s%s%s%s%s%s test (seed=%i, VEC_SZ=%i, PC_FREQ=%i, segsz=%s)%s...", 
    iters, 
    (halfduplex?"half-duplex ":""),
    (runtests&RUN_VECTOR?"V":""), 
    (runtests&RUN_INDEXED?"I":""), 
    (runtests&RUN_STRIDED?"S":""),
    (runtests&RUN_TRANSPOSE?"X":""),
    (runtests&RUN_NB?"N":""),
    mynode+seedoffset, VEC_SZ, PC_FREQ, segstr,
    (verify?"":" (verification disabled)")
    );

  if (halfduplex && mynode % 2 == 1) runtests = 0; /* odd nodes passive */

  doit(iters, runtests);
  test_free(heapseg);
  MSG("done.");

  gasnet_exit(0);
  return 0;
}
