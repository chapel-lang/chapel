/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_trace.c $
 * Description: GASNet implementation of internal helpers
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#define GASNETI_NEED_GASNET_VIS_H 1
#define GASNETI_NEED_GASNET_COLL_H 1
#include <gasnet_internal.h>
#include <gasnet_tools.h>

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

/* ------------------------------------------------------------------------------------ */
/* GASNet Tracing and Statistics */

#if GASNET_TRACE
  GASNETI_IDENT(gasneti_IdentString_trace, "$GASNetTracingEnabled: 1 $");
#endif
#if GASNET_STATS
  GASNETI_IDENT(gasneti_IdentString_stats, "$GASNetStatisticsEnabled: 1 $");
#endif

gasneti_mutex_t gasneti_tracelock = GASNETI_MUTEX_INITIALIZER;
#define GASNETI_MAX_MASKBITS 256
char gasneti_tracetypes[GASNETI_MAX_MASKBITS];
char gasneti_tracetypes_all[GASNETI_MAX_MASKBITS];
char gasneti_statstypes[GASNETI_MAX_MASKBITS];
char gasneti_statstypes_all[GASNETI_MAX_MASKBITS];
char gasneti_trace_maskstr[GASNETI_MAX_MASKBITS+1];
char gasneti_stats_maskstr[GASNETI_MAX_MASKBITS+1];
int gasneti_trace_suppresslocal;
FILE *gasneti_tracefile = NULL;
FILE *gasneti_statsfile = NULL;
#if GASNETI_STATS_OR_TRACE
static gasneti_tick_t starttime;
#endif

#if GASNET_STATS
  void (*gasnett_stats_callback)(
    GASNETI_FORMAT_PRINTF_FUNCPTR_ARG(format,1,2,void (*format)(const char *, ...))
  ) = NULL;
#endif


static int _gasnett_trace_enabled_body(char tracecat) {
  return GASNETI_TRACE_ENABLED(tracecat);
}
int (*_gasnett_trace_enabled)(char tracecat) = &_gasnett_trace_enabled_body;

#if GASNET_TRACE
  #define TMPBUFSZ 1024
  #define _GASNETT_TRACE_PRINTF_DOIT(cat) do {                                 \
    char output[TMPBUFSZ];                                                     \
    if (GASNETI_TRACE_ENABLED(cat)) { /* skip some varargs overhead */         \
      va_list argptr;                                                          \
      va_start(argptr, format); /*  pass in last argument */                   \
        { int sz = vsnprintf(output, TMPBUFSZ, format, argptr);                \
          if (sz >= (TMPBUFSZ-5) || sz < 0) strcpy(output+(TMPBUFSZ-5),"..."); \
        }                                                                      \
      va_end(argptr);                                                          \
      GASNETI_TRACE_MSG(cat, output);                                            \
    }                                                                          \
  } while (0)

  GASNETI_FORMAT_PRINTF(_gasnett_trace_printf_body,1,2,
  static void _gasnett_trace_printf_body(const char *format, ...)) {
    _GASNETT_TRACE_PRINTF_DOIT(H);
  }
  GASNETI_FORMAT_PRINTF(_gasnett_trace_printf_force_body,1,2,
  static void _gasnett_trace_printf_force_body(const char *format, ...)) {
    _GASNETT_TRACE_PRINTF_DOIT(U);
  }
  GASNETT_FORMAT_PRINTF_FUNCPTR(_gasnett_trace_printf,1,2,
  void (*_gasnett_trace_printf)(const char *format, ...)) = _gasnett_trace_printf_body;
  GASNETT_FORMAT_PRINTF_FUNCPTR(_gasnett_trace_printf_force,1,2,
  void (*_gasnett_trace_printf_force)(const char *format, ...)) = _gasnett_trace_printf_force_body;

  #undef _GASNETT_TRACE_PRINTF_DOIT
  #undef TMPBUFSZ
#endif

/* ------------------------------------------------------------------------------------ */
/* VIS trace formatting - these are legal even without STATS/TRACE */
extern size_t gasneti_format_memveclist_bufsz(size_t count) {
  size_t res = 200+count*50;
  gasneti_assume(res > 0); // silence a warning observed on gcc 7.2.1 (Advance-Toolchain-at11.0)
  return res;
}
extern gasneti_memveclist_stats_t gasneti_format_memveclist(char *buf, size_t count, gex_Memvec_t const *list) {
  const size_t bufsz = gasneti_format_memveclist_bufsz(count);
  char * p = buf;
  size_t j=0;
  gasneti_memveclist_stats_t stats = gasnete_memveclist_stats(count, list);
  sprintf(p, "%"PRIuSZ" entries, totalsz=%"PRIuSZ", bounds=["GASNETI_LADDRFMT"..."GASNETI_LADDRFMT"]\n"
             "list=[",
              count, stats._totalsz,
              GASNETI_LADDRSTR(stats._minaddr), GASNETI_LADDRSTR(stats._maxaddr));
  p += strlen(p);
  for (size_t i=0; i < count; i++) {
    j++;
    sprintf(p, "{"GASNETI_LADDRFMT",%5"PRIuSZ"}", 
      GASNETI_LADDRSTR(list[i].gex_addr), list[i].gex_len);
    if (i < count-1) { 
      strcat(p, ", ");
      if (j % 4 == 0) strcat(p,"\n      ");
    }
    p += strlen(p);
    gasneti_assert_uint(p-buf ,<, bufsz);
  }
  strcat(p,"]");
  p += strlen(p);
  gasneti_assert_uint(p-buf ,<, bufsz);
  return stats;
}
extern size_t gasneti_format_putvgetv_bufsz(size_t dstcount, size_t srccount) {
  return 200+dstcount*50+srccount*50;
}
extern size_t gasneti_format_putvgetv(char *buf, gex_TM_t tm, gex_Rank_t rank,
                                    size_t dstcount, gex_Memvec_t const dstlist[], 
                                    size_t srccount, gex_Memvec_t const srclist[]) {
  const int bufsz = gasneti_format_putvgetv_bufsz(dstcount, srccount);
  char * dstlist_str = (char *)gasneti_malloc(gasneti_format_memveclist_bufsz(dstcount));
  char * srclist_str = (char *)gasneti_malloc(gasneti_format_memveclist_bufsz(srccount));
  gasneti_memveclist_stats_t dststats = gasneti_format_memveclist(dstlist_str, dstcount, dstlist);
  (void) gasneti_format_memveclist(srclist_str, srccount, srclist);
  sprintf(buf,"(%"PRIuSZ" data bytes) peer="GASNETI_TMRANKFMT"\n"
              "dst: %s\nsrc: %s",
              dststats._totalsz, GASNETI_TMRANKSTR(tm,rank),
              dstlist_str, srclist_str);    
  gasneti_assert_uint(strlen(buf) ,<, bufsz);
  gasneti_free(dstlist_str);
  gasneti_free(srclist_str);
  return dststats._totalsz;
}

extern size_t gasneti_format_addrlist_bufsz(size_t count) {
  size_t res = 200+count*25;
  gasneti_assume(res > 0); // silence a warning observed on gcc 7.2.1 (Advance-Toolchain-at11.0)
  return res;
}
extern gasneti_addrlist_stats_t gasneti_format_addrlist(char *buf, size_t count, void * const *list, size_t len) {
  const size_t bufsz = gasneti_format_addrlist_bufsz(count);
  char * p = buf;
  size_t j=0;
  gasneti_addrlist_stats_t stats = gasnete_addrlist_stats((count), (list), (len));
  sprintf(p, "%"PRIuSZ" entries, totalsz=%"PRIuSZ", len=%"PRIuSZ", bounds=["GASNETI_LADDRFMT"..."GASNETI_LADDRFMT"]\n"
             "list=[",
              count, count*len, len,
              GASNETI_LADDRSTR(stats._minaddr), GASNETI_LADDRSTR(stats._maxaddr));
  p += strlen(p);
  for (size_t i=0; i < count; i++) {
    j++;
    sprintf(p, GASNETI_LADDRFMT, GASNETI_LADDRSTR(list[i]));
    if (i < count-1) {
      strcat(p, ", ");
      if (j % 8 == 0) strcat(p,"\n      ");
    }
    p += strlen(p);
    gasneti_assert_uint(p-buf ,<, bufsz);
  }
  strcat(p,"]");
  p += strlen(p);
  gasneti_assert_uint(p-buf ,<, bufsz);
  return stats;
}
extern size_t gasneti_format_putigeti_bufsz(size_t dstcount, size_t srccount) {
  return 500+dstcount*25+srccount*25;
}
extern size_t gasneti_format_putigeti(char *buf, gex_TM_t tm, gex_Rank_t rank,
                                    size_t dstcount, void * const dstlist[], size_t dstlen,
                                    size_t srccount, void * const srclist[], size_t srclen) {
  const int bufsz = gasneti_format_putigeti_bufsz(dstcount, srccount);
  char * dstlist_str = (char *)gasneti_malloc(gasneti_format_addrlist_bufsz(dstcount));
  char * srclist_str = (char *)gasneti_malloc(gasneti_format_addrlist_bufsz(srccount));
  size_t totalsz = dstcount * dstlen;
  (void) gasneti_format_addrlist(dstlist_str, dstcount, (void * const *)dstlist, dstlen);
  (void) gasneti_format_addrlist(srclist_str, srccount, (void * const *)srclist, srclen);
  size_t len = snprintf(buf,bufsz,"(%"PRIuSZ" data bytes) peer="GASNETI_TMRANKFMT"\n"
              "dst: %s\nsrc: %s",
              totalsz, GASNETI_TMRANKSTR(tm,rank),
              dstlist_str, srclist_str);    
  gasneti_assert_uint(len ,<, bufsz);
  gasneti_free(dstlist_str);
  gasneti_free(srclist_str);
  return totalsz;
}

extern size_t gasneti_format_strides_bufsz(size_t count) {
  return count*30+10;
}
extern void gasneti_format_strides(char *buf, size_t count, const ptrdiff_t *list) {
  const int bufsz = gasneti_format_strides_bufsz(count);
  char * p = buf;
  strcpy(p,"["); p++;
  for (size_t i=0; i < count; i++) {
    sprintf(p, "%"PRIdPD, list[i]);
    if (i < count-1) strcat(p, ", ");
    p += strlen(p);
    gasneti_assert_uint(p-buf ,<, bufsz);
  }
  strcat(p,"]"); p++;
  gasneti_assert_uint(p-buf ,<, bufsz);
}

extern size_t gasneti_format_putsgets_bufsz(size_t stridelevels) {
  return 500+3*stridelevels*50;
}
extern size_t gasneti_format_putsgets(char *buf, void *_pstats, 
                                    gex_TM_t tm, gex_Rank_t rank,
                                    void *dstaddr, const ptrdiff_t dststrides[],
                                    void *srcaddr, const ptrdiff_t srcstrides[],
                                    size_t elemsz, const size_t count[], size_t stridelevels) {
  // TODO-EX: trace tm and flags
  gasneti_assert(buf);

  // initial degenerate cases
  if_pf (elemsz == 0) {
    sprintf(buf,"(0 data bytes) elemsz=0");
    return 0;
  }
  if_pf (stridelevels == 0) {
    sprintf(buf,"(%"PRIuSZ" data bytes) peer="GASNETI_TMRANKFMT" stridelevels=0 elemsz=%"PRIuSZ"\n"
              "dst: dstaddr="GASNETI_LADDRFMT"\n"
              "src: srcaddr="GASNETI_LADDRFMT,
       elemsz, GASNETI_TMRANKSTR(tm,rank), elemsz,
       GASNETI_LADDRSTR(dstaddr),
       GASNETI_LADDRSTR(srcaddr)
    );
    return elemsz;
  }

  // general case
  const int bufsz = gasneti_format_putsgets_bufsz(stridelevels);
  char * srcstrides_str = (char *)gasneti_malloc(gasneti_format_strides_bufsz(stridelevels));
  char * dststrides_str = (char *)gasneti_malloc(gasneti_format_strides_bufsz(stridelevels));
  char * count_str = (char *)gasneti_malloc(gasneti_format_strides_bufsz(stridelevels));

  size_t const totalsz = gasnete_strided_datasize(elemsz, count, stridelevels);

  void const *dstbase = dstaddr;
  size_t const dstextent = gasnete_strided_bounds(dststrides, elemsz, count, stridelevels, &dstbase);
  void const *srcbase = srcaddr;
  size_t const srcextent = gasnete_strided_bounds(srcstrides, elemsz, count, stridelevels, &srcbase);

  gasneti_format_strides(srcstrides_str, stridelevels, srcstrides);
  gasneti_format_strides(dststrides_str, stridelevels, dststrides);
  gasneti_assert_uint(sizeof(ptrdiff_t) ,==, sizeof(size_t));
  gasneti_format_strides(count_str, stridelevels, (const ptrdiff_t *)count);
  sprintf(buf,"(%"PRIuSZ" data bytes) peer="GASNETI_TMRANKFMT" stridelevels=%"PRIuSZ" elemsz=%"PRIuSZ" count=%s\n"
              "dst: dstaddr="GASNETI_LADDRFMT" dststrides=%s\n"
              "     extent=%"PRIuSZ" bounds=["GASNETI_LADDRFMT"..."GASNETI_LADDRFMT"]\n"
              "src: srcaddr="GASNETI_LADDRFMT" srcstrides=%s\n"
              "     extent=%"PRIuSZ" bounds=["GASNETI_LADDRFMT"..."GASNETI_LADDRFMT"]\n"
           ,
              totalsz, GASNETI_TMRANKSTR(tm,rank), stridelevels, elemsz, count_str,
              GASNETI_LADDRSTR(dstaddr), dststrides_str, 
              dstextent, GASNETI_LADDRSTR(dstbase), GASNETI_LADDRSTR((uint8_t *)dstbase+dstextent-1),
              GASNETI_LADDRSTR(srcaddr), srcstrides_str,
              srcextent, GASNETI_LADDRSTR(srcbase), GASNETI_LADDRSTR((uint8_t *)srcbase+srcextent-1)
          );
  gasneti_assert_uint(strlen(buf) ,<, bufsz);

  gasneti_free(srcstrides_str);
  gasneti_free(dststrides_str);
  gasneti_free(count_str);
  return totalsz;
}

/* ------------------------------------------------------------------------------------ */
/* Enum/mask trace formatting - these are legal even without STATS/TRACE */

// Returns number of bytes written (or "would have been" for buf == NULL), including the '\0'.
// For val==0 the output is "(empty)"
// For val containing bits not named in the input, the output is "(invalid)"
static size_t
gasneti_format_mask(char *buf, uint64_t val, size_t count, const char **names, const char *prefix) {
  gasneti_assert_uint(count ,<, 64);
  if (!val) {
    const char *answer = "(empty)";
    if (buf) strcpy(buf, answer);
    return (1 + strlen(answer));
  } 
  if (val & ~(((uint64_t)1 << count) - 1)) {
    const char *answer = "(invalid)";
    if (buf) strcpy(buf, answer);
    return (1 + strlen(answer));
  }
  
  size_t prefixlen = strlen(prefix);
  size_t rc = 1;
  if (buf) buf[0] = '\0';
  for (int i = 0; i < count; ++i) {
    uint64_t mask = (uint64_t)1 << i;
    if (! (val & mask)) continue;
    const char *name = names[i];
    if (buf) {
      if (rc != 1) strcat(buf, "|");
      strcat(buf, prefix);
      strcat(buf, name);
    }
    rc += (rc==1?0:1) + prefixlen + strlen(name);
  }
  if (buf) gasneti_assert_uint(rc ,==, 1+strlen(buf));
  return rc;
}

size_t gasneti_format_dt(char *buf, gex_DT_t dt) {
  static const char* names[] = {"I32", "U32", "I64", "U64", "FLT", "DBL", "USER"};
  return gasneti_format_mask(buf,dt,sizeof(names)/sizeof(char *),names,"GEX_DT_");
}

size_t gasneti_format_op(char *buf, gex_OP_t op) {
  static const char* names[] = {
    "AND",   "OR",    "XOR",
    "ADD",   "SUB",   "MULT",
    "MIN",   "MAX",
    "INC",   "DEC",
    "SET",   "CAS",
    "UNK12", "UNK13", "UNK14", // 12 - 14 reserved
    "FAND",  "FOR",   "FXOR",
    "FADD",  "FSUB",  "FMULT",
    "FMIN",  "FMAX",
    "FINC",  "FDEC",
    "SWAP",  "FCAS",  "GET",
    "UNK28", "UNK29",          // 28 - 29 reserved
    "USER",  "USER_NC"
  };
  return gasneti_format_mask(buf,op,sizeof(names)/sizeof(char *),names,"GEX_OP_");
}

size_t gasneti_format_ti(char *buf, gex_TI_t ti) {
  static const char* names[] = { "SRCRANK", "ENTRY", "IS_REQ", "IS_LONG", "EP" };
  return gasneti_format_mask(buf,ti,sizeof(names)/sizeof(char *),names,"GEX_TI_");
}

/* ------------------------------------------------------------------------------------ */

/* line number control */
#if GASNET_SRCLINES
  #if GASNETI_CLIENT_THREADS
    GASNETI_THREADKEY_DEFINE(gasneti_srclineinfo_key);
    typedef struct {
      const char *filename;
      unsigned int linenum;
      unsigned int frozen;
    } gasneti_srclineinfo_t;
    static void gasneti_srclineinfo_cleanup_threaddata(void *_td) {
      gasneti_threadkey_set(gasneti_srclineinfo_key, NULL);
      gasneti_free(_td);
    }
    GASNETI_INLINE(gasneti_mysrclineinfo)
    gasneti_srclineinfo_t *gasneti_mysrclineinfo(void) {
      gasneti_srclineinfo_t *srclineinfo = gasneti_threadkey_get(gasneti_srclineinfo_key);
      if_pt (srclineinfo) {
        gasneti_memcheck(srclineinfo);
        return srclineinfo;
      } else {
        /*  first time we've seen this thread - need to set it up */
        gasneti_srclineinfo_t *srclineinfo = gasneti_calloc(1,sizeof(gasneti_srclineinfo_t));
        gasneti_threadkey_set(gasneti_srclineinfo_key, srclineinfo);
        gasnete_register_threadcleanup(gasneti_srclineinfo_cleanup_threaddata, srclineinfo);
        gasneti_leak(srclineinfo);
        return srclineinfo;
      }
    }
    void gasneti_trace_setsourceline(const char *filename, unsigned int linenum) {
      gasneti_srclineinfo_t *sli = gasneti_mysrclineinfo();
      if_pf (sli->frozen > 0) return;
      if_pt (filename) sli->filename = filename;
      sli->linenum = linenum;
    }
    extern void gasneti_trace_getsourceline(const char **pfilename, unsigned int *plinenum) {
      gasneti_srclineinfo_t *sli = gasneti_mysrclineinfo();
      *pfilename = sli->filename;
      *plinenum = sli->linenum;
    }
    extern void gasneti_trace_freezesourceline(void) {
      gasneti_srclineinfo_t *sli = gasneti_mysrclineinfo();
      sli->frozen++;
    }
    extern void gasneti_trace_unfreezesourceline(void) {
      gasneti_srclineinfo_t *sli = gasneti_mysrclineinfo();
      gasneti_assert_int(sli->frozen ,>, 0);
      sli->frozen--;
    }
  #else
    const char *gasneti_srcfilename = NULL;
    unsigned int gasneti_srclinenum = 0;
    unsigned int gasneti_srcfreeze = 0;
  #endif
#endif

// gasneti_dynsprintf() available even w/o TRACE/STATS
// For instance, for generating portions of error messages
#if 1
  #define BUFSZ     8192
  #define NUMBUFS   4
  typedef struct {
    int bufidx;
    char bufs[NUMBUFS][BUFSZ];
  } gasneti_printbuf_t;
  GASNETI_THREADKEY_DEFINE(gasneti_printbuf_key);
  static void gasneti_printbuf_cleanup_threaddata(void *_td) {
      gasneti_threadkey_set(gasneti_printbuf_key, NULL);
      gasneti_free(_td);
  }

  static char *gasneti_getbuf(void) {
    gasneti_printbuf_t * printbuf;
    if ((printbuf = gasneti_threadkey_get(gasneti_printbuf_key)) == NULL) {
      printbuf = gasneti_malloc(sizeof(gasneti_printbuf_t));
      gasneti_threadkey_set(gasneti_printbuf_key, printbuf);
      printbuf->bufidx = 0;
      gasnete_register_threadcleanup(gasneti_printbuf_cleanup_threaddata, printbuf);
      gasneti_leak(printbuf);
    }
    gasneti_memcheck(printbuf);
    printbuf->bufidx = (printbuf->bufidx+1) % NUMBUFS;
    return printbuf->bufs[printbuf->bufidx];
  }

  /* format and return a string result
     caller should not deallocate string, they are recycled automatically
  */
  extern char *gasneti_dynsprintf(const char *format, ...) {
    va_list argptr;
    char *output = gasneti_getbuf();

    va_start(argptr, format); /*  pass in last argument */
      { int sz = vsnprintf(output, BUFSZ, format, argptr);
        if (sz >= (BUFSZ-5) || sz < 0) strcpy(output+(BUFSZ-5),"...");
      }
    va_end(argptr);
    return output;
  }
#endif

#if GASNETI_STATS_OR_TRACE
  #define BUILD_STATS(type,name,desc) { #type, #name, #desc },
  gasneti_statinfo_t gasneti_stats[] = {
    GASNETI_ALL_STATS(BUILD_STATS, BUILD_STATS, BUILD_STATS)
    {NULL, NULL, NULL}
  };

  /* give gcc enough information to type-check our format strings */
  GASNETI_FORMAT_PRINTF(gasneti_file_vprintf,2,0,
  static void gasneti_file_vprintf(FILE *fp, const char *format, va_list argptr));
  GASNETI_FORMAT_PRINTF(gasneti_trace_printf,1,2,
  static void gasneti_trace_printf(const char *format, ...));
  GASNETI_FORMAT_PRINTF(gasneti_stats_printf,1,2,
  static void gasneti_stats_printf(const char *format, ...));
  GASNETI_FORMAT_PRINTF(gasneti_tracestats_printf,1,2,
  static void gasneti_tracestats_printf(const char *format, ...));

  #define BYTES_PER_LINE 16
  #define MAX_LINES 10
  /* format a block of data into a string and return it - 
     caller should not deallocate string, they are recycled automatically
   */
  extern char *gasneti_formatdata(void *p, size_t nbytes) { 
    uint8_t *data = (uint8_t *)p;
    char *output = gasneti_getbuf();
    *output = '\0';
    if (nbytes <= 8) { /* fits on one line */
      size_t i;
      for (i=0; i < nbytes; i++) {
        char temp[5];
        sprintf(temp,"%02x ",(int)data[i]);
        strcat(output, temp);
      }
    } else {
      size_t line;
      size_t col;
      size_t byteidx = 0;
      strcat(output,"\n");
      for (line=0;line<MAX_LINES && byteidx<nbytes;line++) {
        char nicefmt[BYTES_PER_LINE+1];
        char lineheader[10];
        nicefmt[0] = '\0';
        sprintf(lineheader, "  0x%-2x:  ", (int)byteidx);
        strcat(output, lineheader);
        for (col=0;col<BYTES_PER_LINE && byteidx<nbytes;col++) {
          char temp[5];
          sprintf(temp,"%02x ",(int)data[byteidx]);
          strcat(output, temp);
          if (isprint(data[byteidx])) nicefmt[col] = data[byteidx];
          else nicefmt[col] = '.';
          byteidx++;
        }
        nicefmt[col] = '\0';
        for(;col < BYTES_PER_LINE; col++) strcat(output, "   ");
        strcat(output, "  ");
        strcat(output, nicefmt);
        strcat(output, "\n");
      }
      if (byteidx < nbytes) strcat(output, "         (output truncated)\n");
    }
    return output;
  }

  #define MAX_FORMAT_EP_LOC 128
  /* format an array of EP_Location_t
     caller should not deallocate string, they are recycled automatically
   */
  extern char *gasneti_format_eploc(const gex_EP_Location_t *members, size_t nmembers) {
    // (rank is 10 chars max) + "." + (ep_idx is 4 char max at 12 bits) + ", " =  17 char each
    gasneti_static_assert(17 * MAX_FORMAT_EP_LOC + 5 < BUFSZ);

    int count = MIN(MAX_FORMAT_EP_LOC, nmembers);
    char *buf = gasneti_getbuf();
    char *p = buf;
    for (int i = 0; i < count; ++i) {
      sprintf(p, "%d.%d", members[i].gex_rank, members[i].gex_ep_index);
      if (i != count-1) {
        strcat(p, ", ");  // non-final element
      } else if (count != nmembers) {
        strcat(p, ", ..."); // final element of a truncated list
      }
      p += strlen(p);
      gasneti_assert_uint(p-buf ,<, BUFSZ);
    }
    return buf;
  }

  static int gasneti_autoflush = 0;
  #define GASNETI_TRACEFILE_FLUSH(fp) do {  \
    if (gasneti_autoflush) fflush(fp);      \
  } while (0)

  /* private helper for gasneti_trace/stats_output */
  static void gasneti_file_output(FILE *fp, double time, const char *type, const char *msg, int traceheader) {
    gasneti_mutex_assertlocked(&gasneti_tracelock);
    gasneti_assert(fp);
    if (traceheader) {
      char srclinestr[255];
      srclinestr[0] ='\0';
      #if GASNET_TRACE
        if (GASNETI_TRACE_ENABLED(N)) {
          const char *filename; 
          unsigned int linenum;
          gasneti_trace_getsourceline(&filename, &linenum);
          if (filename) sprintf(srclinestr," [%s:%i]", filename, linenum);
        }
      #endif
      #if GASNETI_THREADS
        fprintf(fp, "%i(%x) %8.6fs>%s (%c) %s%s", 
          (int)gasneti_mynode, (int)(uintptr_t)pthread_self(), time, srclinestr, *type,
          msg, (msg[strlen(msg)-1]=='\n'?"":"\n"));
      #else
        fprintf(fp, "%i %8.6fs>%s (%c) %s%s", (int)gasneti_mynode, time, srclinestr, *type,
          msg, (msg[strlen(msg)-1]=='\n'?"":"\n"));
      #endif
    } else {
        fprintf(fp, "%i> (%c) %s%s", (int)gasneti_mynode, *type, msg,
                (msg[strlen(msg)-1]=='\n'?"":"\n"));
    }
    GASNETI_TRACEFILE_FLUSH(fp);
  }

  /* dump message to tracefile */
  extern void gasneti_trace_output(const char *type, const char *msg, int traceheader) {
    if (gasneti_tracefile) {
      double time = gasneti_ticks_to_ns(gasneti_ticks_now() - starttime) / 1.0E9;
      gasneti_mutex_lock(&gasneti_tracelock);
        if (gasneti_tracefile) 
          gasneti_file_output(gasneti_tracefile, time, type, msg, traceheader);
      gasneti_mutex_unlock(&gasneti_tracelock);
    }
  }
  extern void gasneti_stats_output(const char *type, const char *msg, int traceheader) {
    if (gasneti_tracefile || gasneti_statsfile) {
      double time = gasneti_ticks_to_ns(gasneti_ticks_now() - starttime) / 1.0E9;
      gasneti_mutex_lock(&gasneti_tracelock);
        if (gasneti_statsfile) 
          gasneti_file_output(gasneti_statsfile, time, type, msg, traceheader);
        #if GASNETI_STATS_ECHOED_TO_TRACEFILE
        if (gasneti_tracefile) /* stat output also goes to trace */
          gasneti_file_output(gasneti_tracefile, time, type, msg, traceheader);
        #endif
      gasneti_mutex_unlock(&gasneti_tracelock);
    }
  }
  extern void gasneti_tracestats_output(const char *type, const char *msg, int traceheader) {
    if (gasneti_tracefile || gasneti_statsfile) {
      double time = gasneti_ticks_to_ns(gasneti_ticks_now() - starttime) / 1.0E9;
      gasneti_mutex_lock(&gasneti_tracelock);
        if (gasneti_statsfile) 
          gasneti_file_output(gasneti_statsfile, time, type, msg, traceheader);
        if (gasneti_tracefile) 
          gasneti_file_output(gasneti_tracefile, time, type, msg, traceheader);
      gasneti_mutex_unlock(&gasneti_tracelock);
    }
  }

  /* private helper for gasneti_trace/stats_printf */
  static void gasneti_file_vprintf(FILE *fp, const char *format, va_list argptr) {
    gasneti_mutex_assertlocked(&gasneti_tracelock);
    gasneti_assert(fp);
    int rc;
    rc = fprintf(fp, "%i> ", (int)gasneti_mynode);
    gasneti_assert_always_int(rc ,>=, 3);
    rc = vfprintf(fp, format, argptr);
    gasneti_assert_always_int(rc ,>=, 0);
    if (format[strlen(format)-1]!='\n') fprintf(fp, "\n");
    GASNETI_TRACEFILE_FLUSH(fp);
  }

  /* dump message to tracefile with simple header */
  static void gasneti_trace_printf(const char *format, ...) {
    va_list argptr;
    if (gasneti_tracefile) {
      gasneti_mutex_lock(&gasneti_tracelock);
      if (gasneti_tracefile) {
        va_start(argptr, format); /*  pass in last argument */
        gasneti_file_vprintf(gasneti_tracefile, format, argptr);
        va_end(argptr);
      }
      gasneti_mutex_unlock(&gasneti_tracelock);
    }
  }
  static void gasneti_stats_printf(const char *format, ...) {
    va_list argptr;
    if (gasneti_tracefile || gasneti_statsfile) {
      gasneti_mutex_lock(&gasneti_tracelock);
      if (gasneti_statsfile) {
        va_start(argptr, format); /*  pass in last argument */
        gasneti_file_vprintf(gasneti_statsfile, format, argptr);
        va_end(argptr);
      }
      #if GASNETI_STATS_ECHOED_TO_TRACEFILE
      if (gasneti_tracefile) { /* stat output also goes to trace */
        va_start(argptr, format); /*  pass in last argument */
        gasneti_file_vprintf(gasneti_tracefile, format, argptr);
        va_end(argptr);
      }
      #endif
      gasneti_mutex_unlock(&gasneti_tracelock);
    }
  }
  static void gasneti_tracestats_printf(const char *format, ...) {
    va_list argptr;
    if (gasneti_tracefile || gasneti_statsfile) {
      gasneti_mutex_lock(&gasneti_tracelock);
      if (gasneti_statsfile) {
        va_start(argptr, format); /*  pass in last argument */
        gasneti_file_vprintf(gasneti_statsfile, format, argptr);
        va_end(argptr);
      }
      if (gasneti_tracefile) {
        va_start(argptr, format); /*  pass in last argument */
        gasneti_file_vprintf(gasneti_tracefile, format, argptr);
        va_end(argptr);
      }
      gasneti_mutex_unlock(&gasneti_tracelock);
    }
  }
#endif

extern FILE *gasneti_open_outputfile(const char *filename, const char *desc) {
  FILE *fp = NULL;
  char pathtemp[255];
  if (!strcmp(filename, "stderr") ||
      !strcmp(filename, "-")) {
    filename = "stderr";
    fp = stderr;
  } else if (!strcmp(filename, "stdout")) {
    filename = "stdout";
    fp = stdout;
  } else {
    strcpy(pathtemp,filename);
    while (strchr(pathtemp,'%')) { /* replace any '%' with node num */
      char temp[255];
      char *p = strchr(pathtemp,'%');
      *p = '\0';
      sprintf(temp,"%s%i%s",pathtemp,(int)gasneti_mynode,p+1);
      strcpy(pathtemp,temp);
    }
    filename = pathtemp;
    {
    #if PLATFORM_OS_CYGWIN
      const char mode[] = "wt";
    #else
      const char mode[] = "w";
    #endif
    #ifdef HAVE_FOPEN64
      fp = fopen64(filename, mode);
    #else
      fp = fopen(filename, mode);
    #endif
    }
    if (!fp) {
      fprintf(stderr, "ERROR: Failed to open '%s' for %s output (%s). Redirecting output to stderr.\n",
              filename, desc, strerror(errno));
      filename = "stderr";
      fp = stderr;
    }
  }
  fprintf(stderr, "GASNet reporting enabled - %s output directed to %s\n", 
          desc, filename);
  return fp;
}

/* check node restriction */
extern int gasneti_check_node_list(const char *listvar) {
  unsigned long node = (unsigned long)gasneti_mynode;
  char *p = gasneti_getenv_withdefault(listvar,NULL);
  if (!p || !*p) return 1;

  while (p && *p) {
    unsigned long val1, val2;
    int match;

    if (*p == '*') return 1;

    match = sscanf(p, "%lu-%lu", &val1, &val2);
    if (match == 1) {
      if (node == val1) return 1;
    } else if (match == 2) {
      if ((node >= val1) && (node <= val2)) return 1;
    }

    p = strchr(p, ',');
    if (p) ++p;
  }

  return 0;
}

/* overwrite the current stats/trace mask (types) with the provided human-readable newmask,
   updating the human-readable maskstr. Unrecognized human-readable types are ignored.
 */
extern void gasneti_trace_updatemask(const char *newmask, char *maskstr, char *types) {
  char *typesall = NULL;
  const char *desc; 
  const char *p;
  char *newmaskstr = maskstr;
  
  if (types == gasneti_tracetypes) { 
    typesall = gasneti_tracetypes_all; 
    desc = "GASNET_TRACEMASK"; 
    #if !GASNET_TRACE
      return;
    #endif
  } else if (types == gasneti_statstypes) { 
    typesall = gasneti_statstypes_all; 
    desc = "GASNET_STATSMASK"; 
    #if !GASNET_STATS
      return;
    #endif
  } else gasneti_fatalerror("Bad call to gasneti_trace_updatemask");

  { static gasneti_mutex_t maskupdate_mutex = GASNETI_MUTEX_INITIALIZER;
    /* ensure mutual exclusion for concurrent mask updates - 
       we do not attempt to prevent races with concurrent tracing, any such desired
       synchronization must be provided by the client
     */
    gasneti_mutex_lock(&maskupdate_mutex);

    for (p = GASNETI_ALLTYPES; *p; p++) { 
      gasneti_assert(!types[(int)*p] || typesall[(int)*p]);
      types[(int)*p] = !!strchr(newmask, *p);
      typesall[(int)*p] |= types[(int)*p];
      if (types[(int)*p]) *(newmaskstr++) = *p;
    }
    *newmaskstr = '\0';
    types['U'] = 1; /* category U is not in GASNETI_ALLTYPES, but is always enabled */
    typesall['U'] = 1;

    { /* ensure tracemask change messages always makes it into the trace */
      char tmpi = gasneti_tracetypes[(int)'I'];
      gasneti_tracetypes[(int)'I'] = 1;
      GASNETI_TRACE_PRINTF(I,("Setting %s to: %s", desc, maskstr));
      gasneti_tracetypes[(int)'I'] = tmpi;
    }

    gasneti_mutex_unlock(&maskupdate_mutex);
  }
}

char gasneti_exename[PATH_MAX] = "[unknown]";
#if GASNET_DEBUGMALLOC
static const char *gasneti_mallocreport_filename = NULL;
#endif

#if PLATFORM_OS_LINUX || PLATFORM_OS_CNL || PLATFORM_OS_WSL || PLATFORM_OS_CYGWIN || \
    PLATFORM_OS_FREEBSD || PLATFORM_OS_NETBSD || PLATFORM_OS_OPENBSD
#define GASNETI_HAVE_ARGV_FROM_PROC 1
/* Try to get substitute argv from /proc, if available.
 * Note that /proc is not mounted by default on many/most BSD systems.
 */
static void gasneti_argv_from_proc(int **ppargc, char ****ppargv) {
  static int argc = 0;
  static char **argv = NULL;

#if PLATFORM_OS_LINUX || PLATFORM_OS_CNL || PLATFORM_OS_WSL || PLATFORM_OS_CYGWIN
  const char *filename = "/proc/self/cmdline";
#elif PLATFORM_OS_FREEBSD || PLATFORM_OS_NETBSD || PLATFORM_OS_OPENBSD
  const char *filename = "/proc/curproc/cmdline";
#endif
  int fd;
  size_t len = 0;
  char *cmdline;

  if_pf (argc) { /* duplicate call */
    *ppargc = &argc;
    *ppargv = &argv;
    return;
  }

  if ((fd = open(filename, O_RDONLY)) < 0) return;

  /* Read the whole file, made harder because stat() yields st_size=0 */
  {
    ssize_t rc;
    size_t asize = 32;
    cmdline = gasneti_malloc(asize);
    while (1) {
      rc = read(fd, cmdline+len, asize-len);
      if (rc == 0) {
        break; /* Normal termination */
      } else if (rc < 0) {
        if (errno == EINTR) continue;
        gasneti_free(cmdline);
        (void) close(fd);
        return; /* Fail silently (non-fatal) */
      }
      len += rc;
      if (len == asize) {
        asize += MIN(asize,1024); /* double up to 1k and linear after */
        cmdline = gasneti_realloc(cmdline, asize);
      }
    }
    (void) close(fd);
  }
  cmdline = gasneti_realloc(cmdline, len);
  gasneti_leak(cmdline);
  if (len > 0) cmdline[len-1] = 0; // bug 4076: defensively ensure null-termination

  /* Parse the cmdline on '\0' separators */
  {
    char *p;
    int i;

    for (p = cmdline, argc = 0; p < (cmdline+len); ++argc) {
      p += strlen(p) + 1;
    }

    argv = gasneti_malloc((argc+1) * sizeof(char*));
    for (p = cmdline, i = 0; i < argc; ++i) {
      argv[i] = p;
      p += strlen(p) + 1;
    }
    argv[argc] = NULL;
  }
  gasneti_leak(argv);

  *ppargc = &argc;
  *ppargv = &argv;
}
#endif

#if PLATFORM_OS_SOLARIS
#include <procfs.h>
#define GASNETI_HAVE_ARGV_FROM_PROC 1
/* Try to get address of true original argv from /proc, if available. */
static void gasneti_argv_from_proc(int **ppargc, char ****ppargv) {
  static int argc = 0;
  static char **argv = NULL;

  int fd;
  size_t len = 0;
  psinfo_t psi;

  if_pf (argc) { /* duplicate call */
    *ppargc = &argc;
    *ppargv = &argv;
    return;
  }

  if ((fd = open("/proc/self/psinfo", O_RDONLY)) < 0) return;

  /* Read the whole file */
  {
    ssize_t rc;
    char *p = (char *)&psi;
    do {
      rc = read(fd, p + len, sizeof(psi) - len);
      if (rc < 0) {
        if (errno == EINTR) continue;
        (void) close(fd);
        return; /* Fail silently (non-fatal) */
      }
      len += rc;
    } while (len != sizeof(psi));
    (void) close(fd);
  }

  /* Extract argc and (shallow) copy argv */
  argc = psi.pr_argc;
  len = (argc+1) * sizeof(char*);
  argv = memcpy(gasneti_malloc(len), (void*)(psi.pr_argv), len);
  gasneti_leak(argv);

  *ppargc = &argc;
  *ppargv = &argv;
}
#endif

#if PLATFORM_OS_DARWIN
#include <sys/sysctl.h>
#if defined(CTL_KERN) && defined(KERN_PROCARGS2)
#define GASNETI_HAVE_ARGV_FROM_SYSCTL 1
/* Try to get substitute argv from the memory above our stack.  */
static void gasneti_argv_from_sysctl(int **ppargc, char ****ppargv) {
  static int argc = 0;
  static char **argv = NULL;

  int i, mib[3];
  char *argv0, *buf;
  size_t len;

  if_pf (argc) { /* duplicate call */
    *ppargc = &argc;
    *ppargv = &argv;
    return;
  }

  mib[0] = CTL_KERN;
  mib[1] = KERN_PROCARGS2;
  mib[2] = getpid();

retry:
  /* Query for length and allocate space */
  len = 0;
  if (sysctl(mib, 3, NULL, &len, NULL, 0) < 0) return;
  len += 8; /* Empirically determined that we must add at least 1 */
  buf = gasneti_malloc(len);

  /* Actual sysctl() query */
  if (sysctl(mib, 3, buf, &len, NULL, 0) < 0) {
    gasneti_free(buf);
    return;
  }

  /* Extract argc and the argv array from buf */
  { char *start, *end;

    if (mib[1] == KERN_PROCARGS2) {
      /* argc is first int */
      argc = *(int*)buf;
      start = buf + sizeof(int);
    } else {
      start = buf;
    }

    /* Skip over execpath and any trailing '\0' to find argv[0] */
    start += strlen(start) + 1;
    while ((start - buf < len) && ! *start) start++;

    #if defined(KERN_PROCARGS)
      if ((start - buf == len) && (mib[1] == KERN_PROCARGS2)) {
        // Did not find anything after exepath.
        // So, try again using KERN_PROCARGS (which excludes argc).
        gasneti_free(buf);
        len = 0;
        mib[1] = KERN_PROCARGS;
        goto retry;
      }
    #endif
    gasneti_assert_uint(start - buf ,<, len);

    /* Skip over the args to find end */
    for (end = start, i = 0; i < argc; ++i) {
      end += strlen(end) + 1;
      gasneti_assert_uint(end - buf ,<, len);
    }

    /* Keep a copy of only what we need */
    len = end - start;
    argv0 = memcpy(gasneti_malloc(len), start, len);
    gasneti_leak(argv0);
    gasneti_free(buf);
  }

  /* Build the argv array */
  argv = gasneti_malloc((argc+1) * sizeof(char*));
  gasneti_leak(argv);
  { char *p = argv0;
    for (i = 0; i < argc; ++i) {
      argv[i] = p;
      p += strlen(p) + 1;
    }
    argv[argc] = NULL;
  }

  *ppargc = &argc;
  *ppargv = &argv;
}
#endif
#endif

#if PLATFORM_OS_FREEBSD || PLATFORM_OS_NETBSD || PLATFORM_OS_OPENBSD
#include <sys/sysctl.h>
#if (PLATFORM_OS_FREEBSD && defined(CTL_KERN) && defined(KERN_PROC) && defined(KERN_PROC_ARGS)) || \
    (PLATFORM_OS_NETBSD && defined(CTL_KERN) && defined(KERN_PROC_ARGS) && defined(KERN_PROC_ARGV)) || \
    (PLATFORM_OS_OPENBSD && defined(CTL_KERN) && defined(KERN_PROC_ARGS) && defined(KERN_PROC_ARGV))
#define GASNETI_HAVE_ARGV_FROM_SYSCTL 1
static void gasneti_argv_from_sysctl(int **ppargc, char ****ppargv) {
  static int argc = 0;
  static char **argv = NULL;

  int mib[4];
  char *buf;
  size_t len;

  if_pf (argc) { /* duplicate call */
    *ppargc = &argc;
    *ppargv = &argv;
    return;
  }

  mib[0] = CTL_KERN;
#if PLATFORM_OS_FREEBSD
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_ARGS;
  mib[3] = getpid();
#elif PLATFORM_OS_NETBSD || PLATFORM_OS_OPENBSD
  mib[1] = KERN_PROC_ARGS;
  mib[2] = getpid();
  mib[3] = KERN_PROC_ARGV;
#else
  #error
#endif

  /* Query for length and allocate space */
  len = 0;
  if (sysctl(mib, 4, NULL, &len, NULL, 0) < 0) return;
  buf = gasneti_malloc(len);

  /* Actual sysctl() query */
  if (sysctl(mib, 4, buf, &len, NULL, 0) < 0) {
    gasneti_free(buf);
    return;
  }

#if PLATFORM_OS_FREEBSD || PLATFORM_OS_NETBSD
  /* Extract argc and the argv array from buf */
  { char *p;
    int i;
    gasneti_leak(buf = gasneti_realloc(buf, len));
    for (argc = 0, p = buf ; p - buf < len; ++argc) {
      p += strlen(p) + 1;
    }
    argv = gasneti_malloc((argc+1) * sizeof(char*));
    gasneti_leak(argv);
    for (i = 0, p = buf; i < argc; ++i) {
      argv[i] = p;
      p += strlen(p) + 1;
    }
    argv[argc] = NULL;
  }
#elif PLATFORM_OS_OPENBSD
  /* Count and relocate (due to realloc) argv[] array already in buf */
  gasneti_leak(argv = gasneti_realloc(buf, len));
  for (argc = 0; argv[argc]; ++argc) {
    argv[argc] += ((uintptr_t)argv - (uintptr_t)buf);
  }
#else
  #error
#endif

  *ppargc = &argc;
  *ppargv = &argv;
}
#endif
#endif

extern void gasneti_trace_init(int *pargc, char ***pargv) {
  char *exename = NULL;

  gasneti_free(gasneti_malloc(1)); /* touch the malloc system to ensure it's intialized */

  static char procid_str[32]; // init the process identifier used in error reporting
  sprintf(procid_str, "proc %i", (int)gasneti_mynode);
  gasneti_procid_str = procid_str;

  /* If we didn't receive argc and argv from caller, try to get the full
   * command line from the system.  Some systems may support multiple
   * mechanisms and we try them all until we get something.
   */
 #ifdef GASNETI_HAVE_ARGV_FROM_SYSCTL
  if (!pargc || !pargv) gasneti_argv_from_sysctl(&pargc,&pargv);
 #endif
 #ifdef GASNETI_HAVE_ARGV_FROM_PROC
  if (!pargc || !pargv) gasneti_argv_from_proc(&pargc,&pargv);
 #endif

  if (pargc && pargv) {
    /* ensure the arguments have been decoded */
    gasneti_decode_args(pargc, pargv);
    exename = (*pargv)[0];
  }

#if 0 /* TODO: none of the following is/are implemented yet */
  /* If we don't yet have a FULL cmdline, perhaps we can find an argv[0] */
 #ifdef GASNETI_HAVE_EXENAME_FROM_SYSCTL
  if (!exename) exename = gasneti_exename_from_proc();
 #endif
 #ifdef GASNETI_HAVE_EXENAME_FROM_PROC
  if (!exename) exename = gasneti_exename_from_proc();
 #endif
#endif

  if (exename) {
    gasneti_qualify_path(gasneti_exename, exename);
    gasneti_backtrace_init(gasneti_exename);
  }

 #if GASNETI_STATS_OR_TRACE
  starttime = gasneti_ticks_now();

  { /* setup tracefile */
    FILE *gasneti_tracefile_tmp = NULL, *gasneti_statsfile_tmp = NULL;
    char *tracefilename = gasneti_getenv_withdefault("GASNET_TRACEFILE","");
    char *statsfilename = gasneti_getenv_withdefault("GASNET_STATSFILE","");
    if (tracefilename && !strcmp(tracefilename, "")) tracefilename = NULL;
    if (tracefilename && !gasneti_check_node_list("GASNET_TRACENODES")) tracefilename = NULL;
    if (statsfilename && !strcmp(statsfilename, "")) statsfilename = NULL;
    if (statsfilename && !gasneti_check_node_list("GASNET_STATSNODES")) statsfilename = NULL;
    #if GASNET_TRACE || (GASNET_STATS && GASNETI_STATS_ECHOED_TO_TRACEFILE)
      if (tracefilename) {
        gasneti_tracefile_tmp = gasneti_open_outputfile(tracefilename, 
        #if GASNET_TRACE
          "tracing"
          #if GASNET_STATS && GASNETI_STATS_ECHOED_TO_TRACEFILE
            " and "
          #endif
        #endif
        #if GASNET_STATS && GASNETI_STATS_ECHOED_TO_TRACEFILE
          "statistical"
        #endif
        );
      } else 
    #endif
      gasneti_tracefile_tmp = NULL;
    #if GASNET_STATS
      if (statsfilename) gasneti_statsfile_tmp = gasneti_open_outputfile(statsfilename, "statistical");
      else 
    #endif
        gasneti_statsfile_tmp = NULL;

    /* query tracing environment variables with tracing still disabled */
  #if GASNET_TRACE
    if (gasneti_tracefile_tmp) { 
      GASNETI_TRACE_SETMASK(gasneti_getenv_withdefault("GASNET_TRACEMASK", GASNETI_ALLTYPES));
    } else GASNETI_TRACE_SETMASK("");
  #endif

  #if GASNET_STATS
    if (gasneti_statsfile_tmp || gasneti_tracefile_tmp) { 
      GASNETI_STATS_SETMASK(gasneti_getenv_withdefault("GASNET_STATSMASK", GASNETI_ALLTYPES));
    } else GASNETI_STATS_SETMASK("");
  #endif

    gasneti_autoflush = gasneti_getenv_yesno_withdefault("GASNET_TRACEFLUSH",0);
    gasneti_trace_suppresslocal = !gasneti_getenv_yesno_withdefault("GASNET_TRACELOCAL",1);

    /* begin tracing */
    gasneti_tracefile = gasneti_tracefile_tmp;
    gasneti_statsfile = gasneti_statsfile_tmp;
  }

  { time_t ltime;
    char temp[1024];
    time(&ltime); 
    strcpy(temp, ctime(&ltime));
    if (temp[strlen(temp)-1] == '\n') temp[strlen(temp)-1] = '\0';
    gasneti_tracestats_printf("Program %s (pid=%i) starting on %s at: %s", 
      gasneti_exename, (int)getpid(), gasnett_gethostname(), temp);
   }
   if (pargc && pargv && (gasneti_tracefile || gasneti_statsfile)) {
    size_t sz = 80;
    for (int i=0; i < *pargc; i++) { 
      const char *arg = (*pargv)[i];
      sz += (arg?strlen(arg):0) + 8;
    }
    char *temp = gasneti_malloc(sz);
    char *p = temp; *p = 0;
    for (int i=0; i < *pargc; i++) { 
      const char *arg = (*pargv)[i];
      if (!arg) strcpy(p,"<null>");
      else {
        int hasspace = 0;
        for (const char *q = arg; *q && !hasspace; q++) if (isspace((int)*q)) hasspace = 1;
        if (hasspace) sprintf(p, "'%s'", arg);
        else sprintf(p, "%s", arg);
      } 
      if (i < *pargc-1) strcat(p, " ");
      p += strlen(p);
    }
    gasneti_tracestats_printf("Command-line: %s", temp);
    gasneti_free(temp);
  }

  gasneti_tracestats_printf("GASNET_CONFIG_STRING: %s", GASNET_CONFIG_STRING);
  gasneti_tracestats_printf("GASNet build timestamp:   " __DATE__ " " __TIME__);
  gasneti_tracestats_printf("GASNet configure args:    %s", GASNETI_CONFIGURE_ARGS);
  gasneti_tracestats_printf("GASNet configure buildid: " GASNETI_BUILD_ID);
  gasneti_tracestats_printf("GASNet system tuple:      " GASNETI_SYSTEM_TUPLE);
  gasneti_tracestats_printf("GASNet configure system:  " GASNETI_SYSTEM_NAME);
  gasneti_tracestats_printf("gex_System_QueryJobRank(): %i", (int)gex_System_QueryJobRank());
  gasneti_tracestats_printf("gex_System_QueryJobSize(): %i", (int)gex_System_QueryJobSize());
  gasneti_tracestats_printf("gasneti_cpu_count(): %i", (int)gasneti_cpu_count());
  gasneti_tracestats_printf("gasneti_getPhysMemSz(): %"PRIu64, gasneti_getPhysMemSz(0));
  #if GASNET_STATS
    gasneti_stats_printf("GASNET_STATSMASK: %s", GASNETI_STATS_GETMASK());
  #endif
  #if GASNET_TRACE
    gasneti_trace_printf("GASNET_TRACEMASK: %s", GASNETI_TRACE_GETMASK());
    gasneti_trace_printf("GASNET_TRACEFLUSH: %i", gasneti_autoflush);
    gasneti_trace_printf("GASNET_TRACELOCAL: %i", !gasneti_trace_suppresslocal);
  #endif

  #if GASNET_NDEBUG
  { const char *NDEBUG_warning =
     "WARNING: tracing/statistical collection may adversely affect application performance.";
    gasneti_tracestats_printf("%s",NDEBUG_warning);
    if (gasneti_tracefile != stdout && gasneti_tracefile != stderr &&
        gasneti_statsfile != stdout && gasneti_statsfile != stderr) {
      fputs(NDEBUG_warning,stderr);
      fputs("\n",stderr);
    }
  }
  #endif

  #ifdef GASNETI_USING_GETTIMEOFDAY
    gasneti_tracestats_printf("WARNING: using gettimeofday() for timing measurement - "
      "all short-term time measurements will be very rough and include significant timer overheads");
  #endif
  gasneti_tracestats_printf("Timer granularity: ~ %.3f us, overhead: ~ %.3f us",
   gasneti_tick_granularity(), gasneti_tick_overhead());

  fflush(NULL);
 #endif /* GASNETI_STATS_OR_TRACE */

 #if GASNET_DEBUGMALLOC
  #if GASNET_NDEBUG
  { const char *NDEBUG_warning =
     "WARNING: debugging malloc may adversely affect application performance.";
   #if GASNETI_STATS_OR_TRACE
    gasneti_tracestats_printf(NDEBUG_warning);
    if (gasneti_tracefile != stdout && gasneti_tracefile != stderr &&
        gasneti_statsfile != stdout && gasneti_statsfile != stderr)
   #endif
    {
      fputs(NDEBUG_warning,stderr);
      fputs("\n",stderr);
    }
  }
  #endif
  gasneti_mallocreport_filename = gasneti_getenv_withdefault("GASNET_MALLOCFILE","");
  if (gasneti_mallocreport_filename && !strcmp(gasneti_mallocreport_filename, "")) gasneti_mallocreport_filename = NULL;
  if (gasneti_mallocreport_filename && !gasneti_check_node_list("GASNET_MALLOCNODES")) gasneti_mallocreport_filename = NULL;
 #endif
}

#if GASNETI_STATS_OR_TRACE
#define AGGRNAME(cat,type) gasneti_aggregate_##cat##_##type
#define AGGR(type)                                       \
  static gasneti_statctr_t AGGRNAME(ctr,type) = 0;       \
  static gasneti_stat_intval_t AGGRNAME(intval,type) =   \
    { 0, GASNETI_STATCTR_MAX, GASNETI_STATCTR_MIN, 0 };  \
  static gasneti_stat_timeval_t AGGRNAME(timeval,type) = \
    { 0, GASNETI_TICK_MAX, GASNETI_TICK_MIN, 0 }
AGGR(G);
AGGR(P);
AGGR(S);
AGGR(R);
AGGR(W);
AGGR(X);
AGGR(B);
AGGR(L);
AGGR(A);
AGGR(I);
AGGR(C);
AGGR(D);
#endif

extern void gasneti_trace_finish(void) {
#if GASNETI_STATS_OR_TRACE
  static gasneti_mutex_t gasneti_tracefinishlock = GASNETI_MUTEX_INITIALIZER;
  gasneti_mutex_lock(&gasneti_tracefinishlock);
  if (gasneti_tracefile || gasneti_statsfile) {

    double time = gasneti_ticks_to_ns(gasneti_ticks_now() - starttime) / 1.0E9;
    gasneti_tracestats_printf("Total application run time: %10.6fs", time);

    fflush(NULL);
    #if GASNET_STATS
    { /* output statistical summary */

      /* reenable all statistics that have ever been enabled, for the final aggregation dump */
      memcpy(gasneti_statstypes, gasneti_statstypes_all, GASNETI_MAX_MASKBITS);

      if (gasnett_stats_callback && GASNETI_STATS_ENABLED(H)) {
        gasneti_stats_printf("--------------------------------------------------------------------------------");
        (*gasnett_stats_callback)(gasneti_stats_printf);
      }

      gasneti_stats_printf("--------------------------------------------------------------------------------");
      gasneti_stats_printf("GASNet Statistical Summary:");
    
      #define ACCUM(pacc, pintval) do {                                           \
          pacc->_count += pintval->_count;                                        \
          if (pintval->_minval < pacc->_minval) pacc->_minval = pintval->_minval; \
          if (pintval->_maxval > pacc->_maxval) pacc->_maxval = pintval->_maxval; \
          pacc->_sumval += pintval->_sumval;                                      \
      } while (0)
      #define CALC_AVG(sum,count) ((count) == 0 ? (double)-1 : (double)(sum) / (double)(count))
      #define DUMP_CTR(type,name,desc)                     \
        if (GASNETI_STATS_ENABLED(type)) {                 \
          gasneti_statctr_t *p = &gasneti_stat_ctr_##name; \
          gasneti_stats_printf(" %-25s %6"PRIu64,          \
                #name" "#desc":", *p);                     \
          AGGRNAME(ctr,type) += *p;                        \
        }
      #define DUMP_INTVAL(type,name,desc)                           \
        if (GASNETI_STATS_ENABLED(type)) {                          \
          gasneti_stat_intval_t *p = &gasneti_stat_intval_##name;   \
          const char *pdesc = #desc;                                \
          if (!p->_count)                                           \
            gasneti_stats_printf(" %-25s %6i", #name":", 0);        \
          else                                                      \
            gasneti_stats_printf(" %-25s %6"PRIu64"  avg/min/max/total"  \
                                 " %s = %.3f/%"PRIu64"/%"PRIu64"/%"PRIu64, \
                  #name":", p->_count, pdesc,                       \
                  CALC_AVG(p->_sumval,p->_count),                   \
                  p->_minval, p->_maxval, p->_sumval);              \
          ACCUM((&AGGRNAME(intval,type)), p);                       \
        }
      #define DUMP_TIMEVAL(type,name,desc)                                   \
        if (GASNETI_STATS_ENABLED(type)) {                                   \
          gasneti_stat_timeval_t *p = &gasneti_stat_timeval_##name;          \
          const char *pdesc = #desc;                                         \
          if (!p->_count)                                                    \
            gasneti_stats_printf(" %-25s %6i", #name":", 0);                 \
          else                                                               \
            gasneti_stats_printf(" %-25s %6"PRIu64"  avg/min/max/total"      \
                                 " %s (us) = %.3f/%.3f/%.3f/%.3f",           \
                  #name":", p->_count, pdesc,                                \
                  gasneti_ticks_to_ns(CALC_AVG(p->_sumval, p->_count))/1000.0, \
                  gasneti_ticks_to_ns(p->_minval)/1000.0,                    \
                  gasneti_ticks_to_ns(p->_maxval)/1000.0,                    \
                  gasneti_ticks_to_ns(p->_sumval)/1000.0);                   \
          ACCUM((&AGGRNAME(timeval,type)), p);                               \
        }

      GASNETI_ALL_STATS(DUMP_CTR, DUMP_INTVAL, DUMP_TIMEVAL);

      gasneti_stats_printf(" ");
      gasneti_stats_printf(" ");
      #define DUMP_AGGR_SZ(type,name) do {                                      \
        if (GASNETI_STATS_ENABLED(type)) {                                      \
          gasneti_stat_intval_t *p = &AGGRNAME(intval,type);                    \
          if (!p->_count)                                                       \
            gasneti_stats_printf("%-25s  %6i","Total "#name":",0);              \
          else                                                                  \
            gasneti_stats_printf("%-25s  %6"PRIu64"  avg/min/max/total"         \
                                 " sz = %.3f/%"PRIu64"/%"PRIu64"/%"PRIu64, \
                                 "Total "#name":",                              \
                                 p->_count, CALC_AVG(p->_sumval,p->_count),     \
                                 p->_minval, p->_maxval, p->_sumval);           \
        }                                                                       \
      } while (0)
      DUMP_AGGR_SZ(G,gets);
      DUMP_AGGR_SZ(P,puts);
      DUMP_AGGR_SZ(W,collectives);
      if (GASNETI_STATS_ENABLED(S)) {
        gasneti_stat_intval_t *try_succ = &AGGRNAME(intval,S);
        gasneti_stat_timeval_t *wait_time = &AGGRNAME(timeval,S);
        if (!try_succ->_count)
          gasneti_stats_printf("%-25s  %6i","Total try sync. calls:",0);
        else
          gasneti_stats_printf("%-25s  %6"PRIu64"  try success rate = %.3f%%  \n",
            "Total try sync. calls:",  try_succ->_count,
            CALC_AVG(try_succ->_sumval, try_succ->_count) * 100.0);
        if (!wait_time->_count)
          gasneti_stats_printf("%-25s  %6i","Total wait sync. calls:",0);
        else
          gasneti_stats_printf("%-25s  %6"PRIu64"  avg/min/max/total waittime (us) = %.3f/%.3f/%.3f/%.3f", 
            "Total wait sync. calls:", wait_time->_count,
            gasneti_ticks_to_ns(CALC_AVG(wait_time->_sumval, wait_time->_count))/1000.0,
            gasneti_ticks_to_ns(wait_time->_minval)/1000.0,
            gasneti_ticks_to_ns(wait_time->_maxval)/1000.0,
            gasneti_ticks_to_ns(wait_time->_sumval)/1000.0);
      }
      if (GASNETI_STATS_ENABLED(A)) 
        gasneti_stats_printf("%-25s  %6"PRIu64, "Total AM's:", AGGRNAME(ctr,A));

      gasneti_stats_printf("--------------------------------------------------------------------------------");
    }
    #endif

    GASNETC_TRACE_FINISH(); /* allow for final output of conduit-core specific statistics */
    GASNETE_TRACE_FINISH(); /* allow for final output of conduit-extended specific statistics */
    fflush(NULL);

    gasneti_mutex_lock(&gasneti_tracelock);
    if (gasneti_tracefile && gasneti_tracefile != stdout && gasneti_tracefile != stderr) 
      fclose(gasneti_tracefile);
    if (gasneti_statsfile && gasneti_statsfile != stdout && gasneti_statsfile != stderr) 
      fclose(gasneti_statsfile);
    gasneti_tracefile = NULL;
    gasneti_statsfile = NULL;
    gasneti_mutex_unlock(&gasneti_tracelock);
    gasneti_mutex_unlock(&gasneti_tracefinishlock);
    sleep(1); /* pause to ensure everyone has written trace if this is a collective exit */
  }
#endif
#if GASNET_DEBUGMALLOC
  if (gasneti_mallocreport_filename) gasneti_heapinfo_dump(gasneti_mallocreport_filename, 1);
#endif
}


/* ------------------------------------------------------------------------------------ */
#if GASNET_STATS
  #define DEF_CTR(type,name,desc) \
    gasneti_statctr_t gasneti_stat_ctr_##name = 0;
  #define DEF_INTVAL(type,name,desc)                   \
    gasneti_stat_intval_t gasneti_stat_intval_##name = \
      { 0, GASNETI_STATCTR_MAX, GASNETI_STATCTR_MIN, 0 };
  #define DEF_TIMEVAL(type,name,desc)                    \
    gasneti_stat_timeval_t gasneti_stat_timeval_##name = \
      { 0, GASNETI_TICK_MAX, GASNETI_TICK_MIN, 0 };
  GASNETI_ALL_STATS(DEF_CTR, DEF_INTVAL, DEF_TIMEVAL)

/* TODO: these routines are probably a bottleneck for stats performance, 
         especially with pthreads. We could reduce the performance impact
         of statistical collection by using inlined functions that 
         increment weak atomics or thread-private counters that are combined at shutdown.
 */
static gasneti_mutex_t gasneti_statlock = GASNETI_MUTEX_INITIALIZER;
#define GASNETI_STAT_LOCK()   gasneti_mutex_lock(&gasneti_statlock);
#define GASNETI_STAT_UNLOCK() gasneti_mutex_unlock(&gasneti_statlock);

extern void gasneti_stat_count_accumulate(gasneti_statctr_t *pctr) {
  GASNETI_STAT_LOCK();
    (*pctr)++;
  GASNETI_STAT_UNLOCK();
}
extern void gasneti_stat_intval_accumulate(gasneti_stat_intval_t *pintval, gasneti_statctr_t val) {
  GASNETI_STAT_LOCK();
    pintval->_count++;
    pintval->_sumval += val;
    if_pf (val > pintval->_maxval) pintval->_maxval = val;
    if_pf (val < pintval->_minval) pintval->_minval = val;
  GASNETI_STAT_UNLOCK();
}
extern void gasneti_stat_timeval_accumulate(gasneti_stat_timeval_t *pintval, gasneti_tick_t val) {
  GASNETI_STAT_LOCK();
    pintval->_count++;
    pintval->_sumval += val;
    if_pf (val > pintval->_maxval) pintval->_maxval = val;
    if_pf (val < pintval->_minval) pintval->_minval = val;
  GASNETI_STAT_UNLOCK();
}
#endif

