/*   $Source: /var/local/cvs/gasnet/gasnet_vis.h,v $
 *     $Date: 2006/11/26 03:10:52 $
 * $Revision: 1.19 $
 * Description: GASNet Extended API Vector, Indexed & Strided declarations
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_VIS_H
#define _GASNET_VIS_H

#include <gasnet.h>

GASNETI_BEGIN_EXTERNC

/*---------------------------------------------------------------------------------*/
GASNETI_INLINE(gasnete_memveclist_totalsz)
uintptr_t gasnete_memveclist_totalsz(size_t count, gasnet_memvec_t const *list) {
  uintptr_t retval = 0;
  size_t i;
  for (i = 0; i < count; i++) {
    retval += list[i].len;
  }
  return retval;
}

GASNETI_INLINE(gasnete_memveclist_stats)
gasneti_memveclist_stats_t gasnete_memveclist_stats(size_t count, gasnet_memvec_t const *list) {
  gasneti_memveclist_stats_t retval;
  size_t minsz = (size_t)-1, maxsz = 0;
  uintptr_t totalsz = 0;
  char *minaddr = (char *)(intptr_t)(uintptr_t)-1;
  char *maxaddr = (char *)0;
  size_t i;
  for (i = 0; i < count; i++) {
    size_t const len = list[i].len;
    char * const addr = (char *)list[i].addr;
    if (len > 0) {
      if (len < minsz) minsz = len;
      if (len > maxsz) maxsz = len;
      if (addr < minaddr) minaddr = addr;
      if (addr + len - 1 > maxaddr) maxaddr = addr + len - 1;
      totalsz += len;
    }
  }
  retval.minsz = minsz;
  retval.maxsz = maxsz;
  retval.minaddr = minaddr;
  retval.maxaddr = maxaddr;
  retval.totalsz = totalsz;
  gasneti_assert(totalsz == gasnete_memveclist_totalsz(count, list));
  return retval;
}
/*---------------------------------------------------------------------------------*/

GASNETI_INLINE(gasnete_addrlist_stats)
gasneti_addrlist_stats_t gasnete_addrlist_stats(size_t count, void * const *list, size_t len) {
  gasneti_addrlist_stats_t retval;
  char *minaddr = (char *)(intptr_t)(uintptr_t)-1;
  char *maxaddr = (char *)0;
  size_t i;
  for (i = 0; i < count; i++) {
    char * const addr = (char *)list[i];
    if (addr < minaddr) minaddr = addr;
    if (addr + len - 1 > maxaddr) maxaddr = addr + len - 1;
  }
  retval.minaddr = minaddr;
  retval.maxaddr = maxaddr;
  return retval;
}

/*---------------------------------------------------------------------------------*/

/* returns non-zero iff the specified strided region is empty */
GASNETI_INLINE(gasnete_strided_empty)
int gasnete_strided_empty(size_t const *count, size_t stridelevels) {
  size_t i;
  for (i = 0; i <= stridelevels; i++) {
    if_pf (count[i] == 0) return 1;
  }
  return 0;
}

/* returns the number of top-level dimensions with a count of 1 */
GASNETI_INLINE(gasnete_strided_nulldims)
size_t gasnete_strided_nulldims(size_t const *count, size_t stridelevels) {
  ssize_t i;
  for (i = stridelevels; i >= 0; i--) {
    if_pt (count[i] != 1) return stridelevels-i;
  }
  return stridelevels+1;
}

/* returns the length of the bounding box containing all the data */
GASNETI_INLINE(gasnete_strided_extent)
size_t gasnete_strided_extent(size_t const *strides, size_t const *count, size_t stridelevels) {
  /* Calculating the bounding rectangle for a strided section is subtle.
     The obvious choice:
       count[stridelevels]*strides[stridelevels-1]
     is an upper bound which is too large in many cases
     The true exact length is:
       count[0] + SUM(i=[1..stridelevels] | (count[i]-1)*strides[i-1])
   */
  size_t i;
  size_t sz = count[0];
  if_pf (count[0] == 0) return 0;
  for (i = 1; i <= stridelevels; i++) {
    if_pf (count[i] == 0) return 0;
    sz += (count[i]-1)*strides[i-1];
  }
  return sz;
}

/* returns the total bytes of data in the transfer */
GASNETI_INLINE(gasnete_strided_datasize)
size_t gasnete_strided_datasize(size_t const *count, size_t stridelevels) {
  size_t i;
  size_t sz = count[0];
  for (i = 1; i <= stridelevels; i++) {
    size_t const cnt = count[i];
    if_pf (sz == 0) return 0;
    if_pf (cnt != 1) sz *= cnt;
  }
  return sz;
}

/* returns the size of the contiguous segments in the transfer
 */
GASNETI_INLINE(gasnete_strided_contigsz)
size_t gasnete_strided_contigsz(size_t const *strides, size_t const *count, size_t stridelevels) {
  size_t limit = stridelevels;
  size_t sz = count[0];
  size_t i;

  /* querying the contiguity of an empty region probably signifies a bug */
  gasneti_assert(!gasnete_strided_empty(count,stridelevels)); 

  while (limit && count[limit] == 1) limit--; /* ignore null dimensions */
  if (strides[0] > sz || limit == 0) return sz;
  for (i = 1; i < limit; i++) {
    sz *= count[i];
    if (strides[i] > sz) return sz;
    gasneti_assert(strides[i] == (count[i]*strides[i-1]));
  }
  sz *= count[limit];
  return sz;
}

/* returns the highest stridelevel with data contiguity 
   eg. returns zero if only the bottom level is contiguous,
   and stridelevels if the entire region is contiguous
 */
GASNETI_INLINE(gasnete_strided_contiguity)
size_t gasnete_strided_contiguity(size_t const *strides, size_t const *count, size_t stridelevels) {
  size_t i;
  size_t limit = stridelevels;

  /* querying the contiguity of an empty region probably signifies a bug */
  gasneti_assert(!gasnete_strided_empty(count,stridelevels)); 

  while (limit && count[limit] == 1) limit--; /* ignore null dimensions */
  if_pf (limit == 0) return stridelevels; /* trivially fully contiguous */

  if (strides[0] > count[0]) return 0;
  gasneti_assert(strides[0] == count[0]);
  for (i = 1; i < limit; i++) {
    if (strides[i] > (count[i]*strides[i-1])) return i;
    gasneti_assert(strides[i] == (count[i]*strides[i-1]));
  }
  return stridelevels;
}

/* returns the highest stridelevel with data contiguity in both regions
   eg. returns zero if only the bottom level is contiguous in one or both regions,
   and stridelevels if the both regions are entirely contiguous
   this can computed more efficiently than checking contiguity of each separately
 */
GASNETI_INLINE(gasnete_strided_dualcontiguity)
size_t gasnete_strided_dualcontiguity(size_t const *strides1, size_t const *strides2, size_t const *count, size_t stridelevels) {
  size_t i;
  size_t temp;
  size_t limit = stridelevels;

  /* querying the contiguity of an empty region probably signifies a bug */
  gasneti_assert(!gasnete_strided_empty(count,stridelevels)); 

  while (limit && count[limit] == 1) limit--; /* ignore null dimensions */
  if_pf (limit == 0) return stridelevels; /* trivially fully contiguous */

  temp = (strides1[0]+strides2[0]);
  if (temp > (count[0]<<1)) {
    gasneti_assert(strides1[0] > count[0] || strides2[0] > count[0]);
    return 0;
  }
  gasneti_assert(strides1[0] == count[0] && strides1[0] == count[0]);
  /* loop invariant: temp == strides1[i-1]*2 == strides2[i-1]*2 */
  for (i = 1; i < limit; i++) {
    size_t const newtemp = (strides1[i]+strides2[i]);
    if (newtemp > (count[i]*temp)) {
      gasneti_assert(strides1[i] > (count[i]*strides1[i-1]) || strides2[i] > (count[i]*strides2[i-1]));
      return i;
    }
    gasneti_assert(strides1[i] == (count[i]*strides1[i-1]) || strides2[i] == (count[i]*strides2[i-1]));
    temp = newtemp;
  }
  return stridelevels;
}

/* returns the size of the contiguous region at the dualcontiguity level */
GASNETI_INLINE(gasnete_strided_dualcontigsz)
size_t gasnete_strided_dualcontigsz(size_t const *strides1, size_t const *strides2, size_t const *count, size_t stridelevels) {
  size_t i;
  size_t temp;
  size_t limit = stridelevels;

  /* querying the contiguity of an empty region probably signifies a bug */
  gasneti_assert(!gasnete_strided_empty(count,stridelevels)); 

  while (limit && count[limit] == 1) limit--; /* ignore null dimensions */
  if_pf (limit == 0) return count[0]; /* trivially fully contiguous */

  temp = (strides1[0]+strides2[0]);
  if (temp > (count[0]<<1)) {
    gasneti_assert(strides1[0] > count[0] || strides2[0] > count[0]);
    return count[0];
  }
  gasneti_assert(strides1[0] == count[0] && strides1[0] == count[0]);
  /* loop invariant: temp == strides1[i-1]*2 == strides2[i-1]*2 */
  for (i = 1; i < limit; i++) {
    size_t const newtemp = (strides1[i]+strides2[i]);
    temp *= count[i];
    if (newtemp > temp) {
      gasneti_assert(strides1[i] > (count[i]*strides1[i-1]) || strides2[i] > (count[i]*strides2[i-1]));
      return temp>>1;
    }
    gasneti_assert(strides1[i] == (count[i]*strides1[i-1]) || strides2[i] == (count[i]*strides2[i-1]));
    temp = newtemp;
  }
  temp *= count[limit];
  return temp>>1;
}

/* returns the number of contiguous segments in the transfer */
GASNETI_INLINE(gasnete_strided_segments)
size_t gasnete_strided_segments(size_t const *strides, size_t const *count, size_t stridelevels) {
  size_t contiglevel = gasnete_strided_contiguity(strides, count, stridelevels);
  if (contiglevel == stridelevels) return 1;
  else {
    size_t i;
    size_t sz = count[contiglevel+1];
    for (i = contiglevel+2; i <= stridelevels; i++) {
      sz *= count[i];
    }
    return sz;
  }
}

typedef struct {
  size_t srcextent; /* the length of the bounding box containing all the src data */
  size_t dstextent; /* the length of the bounding box containing all the dst data */

  uintptr_t totalsz;   /* the total bytes of data in the transfer */

  size_t nulldims;  /* number of top-level dimensions with a count of 1 -
                       these dimensions can be ignored for most purposes 
                       (stridelevels+1 means all counts are one)
                     */

  size_t srccontiguity; /* the highest stridelevel with data contiguity in the src region
                           eg. zero if only the bottom level is contiguous,
                           and stridelevels if the entire region is contiguous
                         */
  size_t dstcontiguity; /* the highest stridelevel with data contiguity in the dst region
                           eg. zero if only the bottom level is contiguous,
                           and stridelevels if the entire region is contiguous
                         */
  size_t dualcontiguity; /* MIN(srccontiguity, dstcontiguity) */

  size_t srcsegments;   /* number of contiguous segments in the src region */
  size_t dstsegments;   /* number of contiguous segments in the dst region */

  size_t srccontigsz;   /* size of the contiguous segments in the src region */
  size_t dstcontigsz;   /* size of the contiguous segments in the dst region */
  size_t dualcontigsz;   /* MIN(srccontigsz,dstcontigsz) */

} gasnete_strided_stats_t;

/* calculate a number of useful shape properties over the given regions */
GASNETI_INLINE(gasnete_strided_stats)
void gasnete_strided_stats(gasnete_strided_stats_t *result, 
                           size_t const *dststrides, size_t const *srcstrides, 
                           size_t const *count, size_t stridelevels) {
  if_pf (stridelevels == 0 && count[0] != 0) {
    size_t const sz = count[0];
    gasneti_assert(!gasnete_strided_empty(count, stridelevels));
    result->srcextent = sz;
    result->dstextent = sz;
    result->totalsz = sz;
    result->nulldims = 0;
    result->srccontiguity = 0;
    result->dstcontiguity = 0;
    result->dualcontiguity = 0;
    result->srcsegments = 1;
    result->dstsegments = 1;
    result->srccontigsz = sz;
    result->dstcontigsz = sz;
    result->dualcontigsz = sz;
    return;
  } else {
    ssize_t limit;
    ssize_t i;
    int srcbreak = 0;
    int dstbreak = 0;
    size_t const sz = count[0];
    size_t srcextent = sz;
    size_t dstextent = sz;
    size_t srcsegments = 1;
    size_t dstsegments = 1;
    size_t srccontigsz = sz;
    size_t dstcontigsz = sz;
    for (limit = stridelevels; limit >= 0; limit--) if (count[limit] != 1) break;
    result->nulldims = stridelevels - limit;
    result->srccontiguity = stridelevels;
    result->dstcontiguity = stridelevels;

    for (i=0; i < limit; i++) {
      size_t const nextcount = count[i+1];
      size_t const srcstride = srcstrides[i];
      size_t const dststride = dststrides[i];
      srcextent += (nextcount-1)*srcstride;
      dstextent += (nextcount-1)*dststride;

      if (srcbreak) srcsegments *= nextcount;
      else if (srcstride > srccontigsz) {
          srcbreak = 1;
          result->srccontiguity = i;
          srcsegments *= nextcount;
      } else srccontigsz *= nextcount;

      if (dstbreak) dstsegments *= nextcount;
      else if (dststride > dstcontigsz) {
          dstbreak = 1;
          result->dstcontiguity = i;
          dstsegments *= nextcount;
      } else dstcontigsz *= nextcount;
    }

    result->totalsz = ((uintptr_t)srcsegments)*srccontigsz;
    if (result->totalsz == 0) { /* empty xfer */
      gasneti_assert(gasnete_strided_empty(count, stridelevels));
      gasneti_assert(gasnete_strided_datasize(count, stridelevels) == 0);
      result->srcextent = 0;
      result->dstextent = 0;
      result->nulldims = 0;
      result->srccontiguity = 0;
      result->dstcontiguity = 0;
      result->dualcontiguity = 0;
      result->srcsegments = 0;
      result->dstsegments = 0;
      result->srccontigsz = 0;
      result->dstcontigsz = 0;
      return;
    }
    result->srccontigsz = srccontigsz;
    result->dstcontigsz = dstcontigsz;
    result->srcsegments = srcsegments;
    result->dstsegments = dstsegments;
    result->srcextent = srcextent;
    result->dstextent = dstextent;
    result->dualcontiguity = MIN(result->srccontiguity, result->dstcontiguity);
    result->dualcontigsz = MIN(result->srccontigsz, result->dstcontigsz);
    /* sanity check */
    gasneti_assert(!gasnete_strided_empty(count, stridelevels));
    gasneti_assert(result->srcextent == gasnete_strided_extent(srcstrides, count, stridelevels));
    gasneti_assert(result->dstextent == gasnete_strided_extent(dststrides, count, stridelevels));
    gasneti_assert(result->totalsz == gasnete_strided_datasize(count, stridelevels));
    gasneti_assert(result->nulldims == gasnete_strided_nulldims(count, stridelevels));
    gasneti_assert(result->srccontiguity == gasnete_strided_contiguity(srcstrides, count, stridelevels));
    gasneti_assert(result->dstcontiguity == gasnete_strided_contiguity(dststrides, count, stridelevels));
    gasneti_assert(result->dualcontiguity == gasnete_strided_dualcontiguity(srcstrides, dststrides, count, stridelevels));
    gasneti_assert(result->srcsegments == gasnete_strided_segments(srcstrides, count, stridelevels));
    gasneti_assert(result->dstsegments == gasnete_strided_segments(dststrides, count, stridelevels));
    gasneti_assert(result->srccontigsz == gasnete_strided_contigsz(srcstrides, count, stridelevels));
    gasneti_assert(result->dstcontigsz == gasnete_strided_contigsz(dststrides, count, stridelevels));
    gasneti_assert(result->dualcontigsz == gasnete_strided_dualcontigsz(srcstrides, dststrides, count, stridelevels));
  }
  return;
}

#if GASNET_NDEBUG
  #define gasnete_boundscheck_memveclist(node, count, list)
  #define gasnete_memveclist_checksizematch(dstcount, dstlist, srccount, srclist)
  #define gasnete_boundscheck_addrlist(node, count, list, len) 
  #define gasnete_addrlist_checksizematch(dstcount, dstlen, srccount, srclen)
  #define gasnete_boundscheck_strided(node, addr, strides, count, stridelevels)
  #define gasnete_check_strides(dststrides, srcstrides, count, stridelevels)
#else
  #define gasnete_boundscheck_memveclist(node, count, list) do {    \
    gasnet_node_t __node = (node);                                  \
    size_t _count = (count);                                        \
    gasnet_memvec_t const * const _list = (list);                   \
    size_t _i;                                                      \
    for (_i=0; _i < _count; _i++) {                                 \
      if (_list[_i].len > 0)                                        \
        gasneti_boundscheck(__node, _list[_i].addr, _list[_i].len); \
    }                                                               \
  } while (0)

  #define gasnete_memveclist_checksizematch(dstcount, dstlist, srccount, srclist) do {         \
    gasneti_memveclist_stats_t dststats = gasnete_memveclist_stats((dstcount), (dstlist));     \
    gasneti_memveclist_stats_t srcstats = gasnete_memveclist_stats((srccount), (srclist));     \
    if_pf (dststats.totalsz != srcstats.totalsz) {                                             \
      char * dstlist_str =                                                                     \
             (char *)gasneti_extern_malloc(gasneti_format_memveclist_bufsz(dstcount));         \
      char * srclist_str =                                                                     \
             (char *)gasneti_extern_malloc(gasneti_format_memveclist_bufsz(srccount));         \
      gasneti_format_memveclist(dstlist_str, (dstcount), (dstlist));                           \
      gasneti_format_memveclist(srclist_str, (srccount), (srclist));                           \
      gasneti_fatalerror("Source and destination memvec lists disagree on total size at %s:\n" \
                         "  srclist: %s\n"                                                     \
                         "  dstlist: %s\n",                                                    \
                         gasneti_current_loc, dstlist_str, srclist_str);                       \
      /* gasneti_extern_free(dstlist_str); -- dead code */                                     \
      /* gasneti_extern_free(srclist_str); -- dead code */                                     \
    }                                                                                          \
    if_pf (dststats.totalsz != 0 &&                                                            \
      ((uintptr_t)dststats.minaddr) + dststats.totalsz - 1 > ((uintptr_t)dststats.maxaddr)) {  \
      char * dstlist_str =                                                                     \
             (char *)gasneti_extern_malloc(gasneti_format_memveclist_bufsz(dstcount));         \
      gasneti_format_memveclist(dstlist_str, (dstcount), (dstlist));                           \
      gasneti_fatalerror("Destination memvec list has overlapping elements at %s:\n"           \
                         "  dstlist: %s\n"                                                     \
                         "(note this test is currently conservative "                          \
                         "and may fail to detect some illegal cases)",                         \
                         gasneti_current_loc, dstlist_str);                                    \
      /* gasneti_extern_free(dstlist_str); -- dead code */                                     \
    }                                                                                          \
  } while (0)

  #define gasnete_boundscheck_addrlist(node, count, list, len) do { \
    gasnet_node_t __node = (node);                                  \
    size_t _count = (count);                                        \
    void * const * const _list = (list);                            \
    size_t _len = (len);                                            \
    size_t _i;                                                      \
    if_pt (_len > 0) {                                              \
      for (_i=0; _i < _count; _i++) {                               \
        gasneti_boundscheck(__node, _list[_i], _len);               \
      }                                                             \
    }                                                               \
  } while (0)

  #define gasnete_addrlist_checksizematch(dstcount, dstlen, srccount, srclen) do {        \
    if_pf ((dstlen) == 0) gasneti_fatalerror("dstlen == 0 at: %s\n",gasneti_current_loc); \
    if_pf ((srclen) == 0) gasneti_fatalerror("srclen == 0 at: %s\n",gasneti_current_loc); \
    if_pf ((dstcount)*(dstlen) != (srccount)*(srclen)) {                                  \
      gasneti_fatalerror("Total data size mismatch at: %s\n"                              \
                         "dstcount(%i)*dstlen(%i) != srccount(%i)*srclen(%i)",            \
                         gasneti_current_loc,                                             \
                         (int)dstcount, (int)dstlen, (int)srccount, (int)srclen);         \
    }                                                                                     \
  } while (0)

  #define gasnete_check_strides(dststrides, srcstrides, count, stridelevels) do {                \
    const size_t * const _dststrides = (dststrides);                                             \
    const size_t * const _srcstrides = (srcstrides);                                             \
    const size_t * const _count = (count);                                                       \
    const size_t _stridelevels = (stridelevels);                                                 \
    if_pt (!gasnete_strided_empty(_count, _stridelevels)) {                                      \
      size_t _i;                                                                                 \
      if_pf (_stridelevels > 0 && _dststrides[0] < _count[0])                                    \
          gasneti_fatalerror("dststrides[0](%i) < count[0](%i) at: %s",                          \
                        (int)_dststrides[0],(int)_count[0], gasneti_current_loc);                \
      if_pf (_stridelevels > 0 && _srcstrides[0] < _count[0])                                    \
          gasneti_fatalerror("srcstrides[0](%i) < count[0](%i) at: %s",                          \
                        (int)_srcstrides[0],(int)_count[0], gasneti_current_loc);                \
      for (_i = 1; _i < _stridelevels; _i++) {                                                   \
        if_pf (_dststrides[_i] < (_count[_i] * _dststrides[_i-1]))                               \
          gasneti_fatalerror("dststrides[%i](%i) < (count[%i](%i) * dststrides[%i](%i)) at: %s", \
                     (int)_i,(int)_dststrides[_i],                                               \
                     (int)_i,(int)_count[_i], (int)_i-1,(int)_dststrides[_i-1], gasneti_current_loc); \
        if_pf (_srcstrides[_i] < (_count[_i] * _srcstrides[_i-1]))                               \
          gasneti_fatalerror("srcstrides[%i](%i) < (count[%i](%i) * srcstrides[%i](%i)) at: %s", \
                     (int)_i,(int)_srcstrides[_i],                                               \
                     (int)_i,(int)_count[_i], (int)_i-1,(int)_srcstrides[_i-1], gasneti_current_loc); \
      }                                                                                          \
    }                                                                                            \
  } while (0)

  #define gasnete_boundscheck_strided(node, addr, strides, count, stridelevels) do { \
    size_t _stridelevels = (stridelevels);                                           \
    if_pt (!gasnete_strided_empty((count), _stridelevels)) {                          \
      gasneti_boundscheck((node), (addr),                                            \
        gasnete_strided_extent((strides),(count),_stridelevels));                    \
    }                                                                                \
  } while (0)

#endif

typedef enum _gasnete_synctype_t {
  gasnete_synctype_b,
  gasnete_synctype_nb,
  gasnete_synctype_nbi
} gasnete_synctype_t;

/*---------------------------------------------------------------------------------*/
/* Vector */
#ifndef gasnete_putv
  extern gasnet_handle_t gasnete_putv(gasnete_synctype_t synctype,
                                     gasnet_node_t dstnode,
                                     size_t dstcount, gasnet_memvec_t const dstlist[], 
                                     size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG);
#endif
#ifndef gasnete_getv
  extern gasnet_handle_t gasnete_getv(gasnete_synctype_t synctype,
                                     size_t dstcount, gasnet_memvec_t const dstlist[], 
                                     gasnet_node_t srcnode,
                                     size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG);
#endif

GASNETI_INLINE(_gasnet_putv_bulk)
void _gasnet_putv_bulk(gasnet_node_t dstnode,
                       size_t dstcount, gasnet_memvec_t const dstlist[], 
                       size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasnete_boundscheck_memveclist(dstnode, dstcount, dstlist);
  gasnete_memveclist_checksizematch(dstcount, dstlist, srccount, srclist);
  GASNETI_TRACE_PUTV(PUTV_BULK,dstnode,dstcount,dstlist,srccount,srclist);
  gasnete_putv(gasnete_synctype_b,dstnode,dstcount,dstlist,srccount,srclist GASNETE_THREAD_PASS);
}
#define gasnet_putv_bulk(dstnode,dstcount,dstlist,srccount,srclist) \
       _gasnet_putv_bulk(dstnode,dstcount,dstlist,srccount,srclist GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_getv_bulk)
void _gasnet_getv_bulk(size_t dstcount, gasnet_memvec_t const dstlist[], 
                       gasnet_node_t srcnode,
                       size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasnete_boundscheck_memveclist(srcnode, srccount, srclist);
  gasnete_memveclist_checksizematch(dstcount, dstlist, srccount, srclist);
  GASNETI_TRACE_GETV(GETV_BULK,srcnode,dstcount,dstlist,srccount,srclist);
  gasnete_getv(gasnete_synctype_b,dstcount,dstlist,srcnode,srccount,srclist GASNETE_THREAD_PASS);
}
#define gasnet_getv_bulk(dstcount,dstlist,srcnode,srccount,srclist) \
       _gasnet_getv_bulk(dstcount,dstlist,srcnode,srccount,srclist GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_putv_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t _gasnet_putv_nb_bulk(gasnet_node_t dstnode,
                                     size_t dstcount, gasnet_memvec_t const dstlist[], 
                                     size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasnete_boundscheck_memveclist(dstnode, dstcount, dstlist);
  gasnete_memveclist_checksizematch(dstcount, dstlist, srccount, srclist);
  GASNETI_TRACE_PUTV(PUTV_NB_BULK,dstnode,dstcount,dstlist,srccount,srclist);
  return gasnete_putv(gasnete_synctype_nb,dstnode,dstcount,dstlist,srccount,srclist GASNETE_THREAD_PASS);
}
#define gasnet_putv_nb_bulk(dstnode,dstcount,dstlist,srccount,srclist) \
       _gasnet_putv_nb_bulk(dstnode,dstcount,dstlist,srccount,srclist GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_getv_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t _gasnet_getv_nb_bulk(size_t dstcount, gasnet_memvec_t const dstlist[], 
                                     gasnet_node_t srcnode,
                                     size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasnete_boundscheck_memveclist(srcnode, srccount, srclist);
  gasnete_memveclist_checksizematch(dstcount, dstlist, srccount, srclist);
  GASNETI_TRACE_GETV(GETV_NB_BULK,srcnode,dstcount,dstlist,srccount,srclist);
  return gasnete_getv(gasnete_synctype_nb,dstcount,dstlist,srcnode,srccount,srclist GASNETE_THREAD_PASS);
}
#define gasnet_getv_nb_bulk(dstcount,dstlist,srcnode,srccount,srclist) \
       _gasnet_getv_nb_bulk(dstcount,dstlist,srcnode,srccount,srclist GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_putv_nbi_bulk)
void _gasnet_putv_nbi_bulk(gasnet_node_t dstnode,
                           size_t dstcount, gasnet_memvec_t const dstlist[], 
                           size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasnete_boundscheck_memveclist(dstnode, dstcount, dstlist);
  gasnete_memveclist_checksizematch(dstcount, dstlist, srccount, srclist);
  GASNETI_TRACE_PUTV(PUTV_NBI_BULK,dstnode,dstcount,dstlist,srccount,srclist);
  gasnete_putv(gasnete_synctype_nbi,dstnode,dstcount,dstlist,srccount,srclist GASNETE_THREAD_PASS);
}
#define gasnet_putv_nbi_bulk(dstnode,dstcount,dstlist,srccount,srclist) \
       _gasnet_putv_nbi_bulk(dstnode,dstcount,dstlist,srccount,srclist GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_getv_nbi_bulk)
void _gasnet_getv_nbi_bulk(size_t dstcount, gasnet_memvec_t const dstlist[], 
                           gasnet_node_t srcnode,
                           size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasnete_boundscheck_memveclist(srcnode, srccount, srclist);
  gasnete_memveclist_checksizematch(dstcount, dstlist, srccount, srclist);
  GASNETI_TRACE_GETV(GETV_NBI_BULK,srcnode,dstcount,dstlist,srccount,srclist);
  gasnete_getv(gasnete_synctype_nbi,dstcount,dstlist,srcnode,srccount,srclist GASNETE_THREAD_PASS);
}
#define gasnet_getv_nbi_bulk(dstcount,dstlist,srcnode,srccount,srclist) \
       _gasnet_getv_nbi_bulk(dstcount,dstlist,srcnode,srccount,srclist GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/
/* Indexed */
#ifndef gasnete_puti
  extern gasnet_handle_t gasnete_puti(gasnete_synctype_t synctype,
                                     gasnet_node_t dstnode, 
                                     size_t dstcount, void * const dstlist[], size_t dstlen,
                                     size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG);
#endif
#ifndef gasnete_geti
  extern gasnet_handle_t gasnete_geti(gasnete_synctype_t synctype,
                                     size_t dstcount, void * const dstlist[], size_t dstlen,
                                     gasnet_node_t srcnode,
                                     size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG);
#endif

GASNETI_INLINE(_gasnet_puti_bulk)
void _gasnet_puti_bulk(gasnet_node_t dstnode, 
                       size_t dstcount, void * const dstlist[], size_t dstlen,
                       size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasnete_boundscheck_addrlist(dstnode, dstcount, dstlist, dstlen);
  gasnete_addrlist_checksizematch(dstcount, dstlen, srccount, srclen);
  GASNETI_TRACE_PUTI(PUTI_BULK,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen);
  gasnete_puti(gasnete_synctype_b,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_PASS);
}
#define gasnet_puti_bulk(dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen) \
       _gasnet_puti_bulk(dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_geti_bulk)
void _gasnet_geti_bulk(size_t dstcount, void * const dstlist[], size_t dstlen,
                       gasnet_node_t srcnode,
                       size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasnete_boundscheck_addrlist(srcnode, srccount, srclist, srclen);
  gasnete_addrlist_checksizematch(dstcount, dstlen, srccount, srclen);
  GASNETI_TRACE_GETI(GETI_BULK,srcnode,dstcount,dstlist,dstlen,srccount,srclist,srclen);
  gasnete_geti(gasnete_synctype_b,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_PASS);
}
#define gasnet_geti_bulk(dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen) \
       _gasnet_geti_bulk(dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_puti_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t _gasnet_puti_nb_bulk(gasnet_node_t dstnode, 
                                     size_t dstcount, void * const dstlist[], size_t dstlen,
                                     size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasnete_boundscheck_addrlist(dstnode, dstcount, dstlist, dstlen);
  gasnete_addrlist_checksizematch(dstcount, dstlen, srccount, srclen);
  GASNETI_TRACE_PUTI(PUTI_NB_BULK,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen);
  return gasnete_puti(gasnete_synctype_nb,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_PASS);
}
#define gasnet_puti_nb_bulk(dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen) \
       _gasnet_puti_nb_bulk(dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_geti_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t _gasnet_geti_nb_bulk(size_t dstcount, void * const dstlist[], size_t dstlen,
                                     gasnet_node_t srcnode,
                                     size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasnete_boundscheck_addrlist(srcnode, srccount, srclist, srclen);
  gasnete_addrlist_checksizematch(dstcount, dstlen, srccount, srclen);
  GASNETI_TRACE_GETI(GETI_NB_BULK,srcnode,dstcount,dstlist,dstlen,srccount,srclist,srclen);
  return gasnete_geti(gasnete_synctype_nb,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_PASS);
}
#define gasnet_geti_nb_bulk(dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen) \
       _gasnet_geti_nb_bulk(dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_puti_nbi_bulk)
void _gasnet_puti_nbi_bulk(gasnet_node_t dstnode, 
                           size_t dstcount, void * const dstlist[], size_t dstlen,
                           size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasnete_boundscheck_addrlist(dstnode, dstcount, dstlist, dstlen);
  gasnete_addrlist_checksizematch(dstcount, dstlen, srccount, srclen);
  GASNETI_TRACE_PUTI(PUTI_NBI_BULK,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen);
  gasnete_puti(gasnete_synctype_nbi,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_PASS);
}
#define gasnet_puti_nbi_bulk(dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen) \
       _gasnet_puti_nbi_bulk(dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_geti_nbi_bulk)
void _gasnet_geti_nbi_bulk(size_t dstcount, void * const dstlist[], size_t dstlen,
                           gasnet_node_t srcnode,
                           size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasnete_boundscheck_addrlist(srcnode, srccount, srclist, srclen);
  gasnete_addrlist_checksizematch(dstcount, dstlen, srccount, srclen);
  GASNETI_TRACE_GETI(GETI_NBI_BULK,srcnode,dstcount,dstlist,dstlen,srccount,srclist,srclen);
  gasnete_geti(gasnete_synctype_nbi,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_PASS);
}
#define gasnet_geti_nbi_bulk(dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen) \
       _gasnet_geti_nbi_bulk(dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/
/* Strided */
#ifndef gasnete_puts
  extern gasnet_handle_t gasnete_puts(gasnete_synctype_t synctype,
                                    gasnet_node_t dstnode,
                                     void *dstaddr, const size_t dststrides[],
                                     void *srcaddr, const size_t srcstrides[],
                                     const size_t count[], size_t stridelevels GASNETE_THREAD_FARG);

#endif
#ifndef gasnete_gets
  extern gasnet_handle_t gasnete_gets(gasnete_synctype_t synctype,
                                     void *dstaddr, const size_t dststrides[],
                                     gasnet_node_t srcnode, 
                                     void *srcaddr, const size_t srcstrides[],
                                     const size_t count[], size_t stridelevels GASNETE_THREAD_FARG);
#endif

GASNETI_INLINE(_gasnet_puts_bulk)
void _gasnet_puts_bulk(gasnet_node_t dstnode,
                       void *dstaddr, const size_t dststrides[],
                       void *srcaddr, const size_t srcstrides[],
                       const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasnete_check_strides(dststrides, srcstrides, count, stridelevels);
  gasnete_boundscheck_strided(dstnode, dstaddr, dststrides, count, stridelevels);
  GASNETI_TRACE_PUTS(PUTS_BULK,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels);
  gasnete_puts(gasnete_synctype_b,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);
}
#define gasnet_puts_bulk(dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_puts_bulk(dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_gets_bulk)
void _gasnet_gets_bulk(void *dstaddr, const size_t dststrides[],
                       gasnet_node_t srcnode, 
                       void *srcaddr, const size_t srcstrides[],
                       const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasnete_check_strides(dststrides, srcstrides, count, stridelevels);
  gasnete_boundscheck_strided(srcnode, srcaddr, srcstrides, count, stridelevels);
  GASNETI_TRACE_GETS(GETS_BULK,srcnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels);
  gasnete_gets(gasnete_synctype_b,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);
}
#define gasnet_gets_bulk(dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_gets_bulk(dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_puts_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t _gasnet_puts_nb_bulk(gasnet_node_t dstnode,
                                     void *dstaddr, const size_t dststrides[],
                                     void *srcaddr, const size_t srcstrides[],
                                     const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasnete_check_strides(dststrides, srcstrides, count, stridelevels);
  gasnete_boundscheck_strided(dstnode, dstaddr, dststrides, count, stridelevels);
  GASNETI_TRACE_PUTS(PUTS_NB_BULK,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels);
  return gasnete_puts(gasnete_synctype_nb,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);
}
#define gasnet_puts_nb_bulk(dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_puts_nb_bulk(dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_gets_nb_bulk) GASNETI_WARN_UNUSED_RESULT
gasnet_handle_t _gasnet_gets_nb_bulk(void *dstaddr, const size_t dststrides[],
                                     gasnet_node_t srcnode, 
                                     void *srcaddr, const size_t srcstrides[],
                                     const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasnete_check_strides(dststrides, srcstrides, count, stridelevels);
  gasnete_boundscheck_strided(srcnode, srcaddr, srcstrides, count, stridelevels);
  GASNETI_TRACE_GETS(GETS_NB_BULK,srcnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels);
  return gasnete_gets(gasnete_synctype_nb,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);
}
#define gasnet_gets_nb_bulk(dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_gets_nb_bulk(dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_puts_nbi_bulk)
void _gasnet_puts_nbi_bulk(gasnet_node_t dstnode,
                           void *dstaddr, const size_t dststrides[],
                           void *srcaddr, const size_t srcstrides[],
                           const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasnete_check_strides(dststrides, srcstrides, count, stridelevels);
  gasnete_boundscheck_strided(dstnode, dstaddr, dststrides, count, stridelevels);
  GASNETI_TRACE_PUTS(PUTS_NBI_BULK,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels);
  gasnete_puts(gasnete_synctype_nbi,dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);
}
#define gasnet_puts_nbi_bulk(dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_puts_nbi_bulk(dstnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_GET)

GASNETI_INLINE(_gasnet_gets_nbi_bulk)
void _gasnet_gets_nbi_bulk(void *dstaddr, const size_t dststrides[],
                           gasnet_node_t srcnode, 
                           void *srcaddr, const size_t srcstrides[],
                           const size_t count[], size_t stridelevels GASNETE_THREAD_FARG) {
  gasnete_check_strides(dststrides, srcstrides, count, stridelevels);
  gasnete_boundscheck_strided(srcnode, srcaddr, srcstrides, count, stridelevels);
  GASNETI_TRACE_GETS(GETS_NBI_BULK,srcnode,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels);
  gasnete_gets(gasnete_synctype_nbi,dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_PASS);
}
#define gasnet_gets_nbi_bulk(dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_gets_nbi_bulk(dstaddr,dststrides,srcnode,srcaddr,srcstrides,count,stridelevels GASNETE_THREAD_GET)

/*---------------------------------------------------------------------------------*/

GASNETI_END_EXTERNC

#endif
