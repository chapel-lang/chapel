/*   $Source: /var/local/cvs/gasnet/gasnet_trace.c,v $
 *     $Date: 2010/04/16 23:19:40 $
 * $Revision: 1.139.2.1 $
 * Description: GASNet implementation of internal helpers
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_tools.h>
#include <gasnet_vis.h>
#include <gasnet_coll.h>

#include <time.h>
#include <sys/time.h>
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
static gasneti_tick_t starttime;

#if GASNET_STATS
  void (*gasnett_stats_callback)(
    GASNETI_FORMAT_PRINTF_FUNCPTR(format,1,2,void (*format)(const char *, ...))
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
  return 200+count*50;
}
extern gasneti_memveclist_stats_t gasneti_format_memveclist(char *buf, size_t count, gasnet_memvec_t const *list) {
  const int bufsz = gasneti_format_memveclist_bufsz(count);
  char * p = buf;
  int i, j=0;
  gasneti_memveclist_stats_t stats = gasnete_memveclist_stats((count), (list));
  sprintf(p, "%i entries, totalsz=%i, bounds=["GASNETI_LADDRFMT"..."GASNETI_LADDRFMT"]\n"
             "list=[",
              (int)(count), (int)(stats.totalsz),
              GASNETI_LADDRSTR(stats.minaddr), GASNETI_LADDRSTR(stats.maxaddr));
  p += strlen(p);
  for (i=0; i < count; i++) {
    j++;
    sprintf(p, "{"GASNETI_LADDRFMT",%5lu}", 
      GASNETI_LADDRSTR(list[i].addr), (unsigned long)list[i].len);
    if (i < count-1) { 
      strcat(p, ", ");
      if (j % 4 == 0) strcat(p,"\n      ");
    }
    p += strlen(p);
    gasneti_assert(p-buf < bufsz);
  }
  strcat(p,"]");
  p += strlen(p);
  gasneti_assert(p-buf < bufsz);
  return stats;
}
extern size_t gasneti_format_putvgetv_bufsz(size_t dstcount, size_t srccount) {
  return 200+dstcount*50+srccount*50;
}
extern size_t gasneti_format_putvgetv(char *buf, gasnet_node_t node, 
                                    size_t dstcount, gasnet_memvec_t const dstlist[], 
                                    size_t srccount, gasnet_memvec_t const srclist[]) {
  const int bufsz = gasneti_format_putvgetv_bufsz(dstcount, srccount);
  char * dstlist_str = (char *)gasneti_malloc(gasneti_format_memveclist_bufsz(dstcount));
  char * srclist_str = (char *)gasneti_malloc(gasneti_format_memveclist_bufsz(srccount));
  gasneti_memveclist_stats_t dststats = gasneti_format_memveclist(dstlist_str, dstcount, dstlist);
  (void) gasneti_format_memveclist(srclist_str, srccount, srclist);
  sprintf(buf,"(%i data bytes) node=%i\n"
              "dst: %s\nsrc: %s",
              (int)dststats.totalsz, (int)(node),
              dstlist_str, srclist_str);    
  gasneti_assert(strlen(buf) < bufsz);
  gasneti_free(dstlist_str);
  gasneti_free(srclist_str);
  return dststats.totalsz;
}

extern size_t gasneti_format_addrlist_bufsz(size_t count) {
  return 200+count*25;
}
extern gasneti_addrlist_stats_t gasneti_format_addrlist(char *buf, size_t count, void * const *list, size_t len) {
  const int bufsz = gasneti_format_addrlist_bufsz(count);
  char * p = buf;
  int i,j=0;
  gasneti_addrlist_stats_t stats = gasnete_addrlist_stats((count), (list), (len));
  sprintf(p, "%i entries, totalsz=%i, len=%i, bounds=["GASNETI_LADDRFMT"..."GASNETI_LADDRFMT"]\n"
             "list=[",
              (int)(count), (int)((count)*(len)), (int)(len),
              GASNETI_LADDRSTR(stats.minaddr), GASNETI_LADDRSTR(stats.maxaddr));
  p += strlen(p);
  for (i=0; i < count; i++) {
    j++;
    sprintf(p, GASNETI_LADDRFMT, GASNETI_LADDRSTR(list[i]));
    if (i < count-1) {
      strcat(p, ", ");
      if (j % 8 == 0) strcat(p,"\n      ");
    }
    p += strlen(p);
    gasneti_assert(p-buf < bufsz);
  }
  strcat(p,"]");
  p += strlen(p);
  gasneti_assert(p-buf < bufsz);
  return stats;
}
extern size_t gasneti_format_putigeti_bufsz(size_t dstcount, size_t srccount) {
  return 500+dstcount*25+srccount*25;
}
extern size_t gasneti_format_putigeti(char *buf, gasnet_node_t node, 
                                    size_t dstcount, void * const dstlist[], size_t dstlen,
                                    size_t srccount, void * const srclist[], size_t srclen) {
  const int bufsz = gasneti_format_putigeti_bufsz(dstcount, srccount);
  char * dstlist_str = (char *)gasneti_malloc(gasneti_format_addrlist_bufsz(dstcount));
  char * srclist_str = (char *)gasneti_malloc(gasneti_format_addrlist_bufsz(srccount));
  size_t totalsz = dstcount * dstlen;
  (void) gasneti_format_addrlist(dstlist_str, dstcount, (void * const *)dstlist, dstlen);
  (void) gasneti_format_addrlist(srclist_str, srccount, (void * const *)srclist, srclen);
  sprintf(buf,"(%i data bytes) node=%i\n"
              "dst: %s\nsrc: %s",
              (int)totalsz, (int)node,
              dstlist_str, srclist_str);    
  gasneti_assert(strlen(buf) < bufsz);
  gasneti_free(dstlist_str);
  gasneti_free(srclist_str);
  return totalsz;
}

extern size_t gasneti_format_strides_bufsz(size_t count) {
  return count*30+10;
}
extern void gasneti_format_strides(char *buf, size_t count, const size_t *list) {
  const int bufsz = gasneti_format_strides_bufsz(count);
  char * p = buf;
  int i;
  strcpy(p,"["); p++;
  for (i=0; i < count; i++) {
    sprintf(p, "%lu", (unsigned long)list[i]);
    if (i < count-1) strcat(p, ", ");
    p += strlen(p);
    gasneti_assert(p-buf < bufsz);
  }
  strcat(p,"]"); p++;
  gasneti_assert(p-buf < bufsz);
}

extern size_t gasneti_format_putsgets_bufsz(size_t stridelevels) {
  return 500+3*stridelevels*50;
}
extern size_t gasneti_format_putsgets(char *buf, void *_pstats, 
                                    gasnet_node_t node, 
                                    void *dstaddr, const size_t dststrides[],
                                    void *srcaddr, const size_t srcstrides[],
                                    const size_t count[], size_t stridelevels) {
  gasnete_strided_stats_t *pstats = _pstats;
  gasnete_strided_stats_t stats;
  const int bufsz = gasneti_format_putsgets_bufsz(stridelevels);
  char * srcstrides_str = (char *)gasneti_malloc(gasneti_format_strides_bufsz(stridelevels));
  char * dststrides_str = (char *)gasneti_malloc(gasneti_format_strides_bufsz(stridelevels));
  char * count_str = (char *)gasneti_malloc(gasneti_format_strides_bufsz(stridelevels+1));

  if (!pstats) pstats = &stats;
  gasnete_strided_stats(pstats, dststrides, srcstrides, count, stridelevels);
  gasneti_format_strides(srcstrides_str, stridelevels, srcstrides);
  gasneti_format_strides(dststrides_str, stridelevels, dststrides);
  gasneti_format_strides(count_str, stridelevels+1, count);
  sprintf(buf,"(%i data bytes) node=%i stridelevels=%i count=%s\n"
              "dualcontiguity=%i nulldims=%i\n"
              "dst: dstaddr="GASNETI_LADDRFMT" dststrides=%s\n"
              "     extent=%i bounds=["GASNETI_LADDRFMT"..."GASNETI_LADDRFMT"]\n"
              "     contiguity=%i contigsz=%i contigsegments=%i\n"
              "src: srcaddr="GASNETI_LADDRFMT" srcstrides=%s\n"
              "     extent=%i bounds=["GASNETI_LADDRFMT"..."GASNETI_LADDRFMT"]\n"
              "     contiguity=%i contigsz=%i contigsegments=%i",
              (int)pstats->totalsz, (int)(node), (int)(stridelevels), count_str,
              (int)pstats->dualcontiguity, (int)pstats->nulldims,
              GASNETI_LADDRSTR(dstaddr), dststrides_str, (int)pstats->dstextent,
              GASNETI_LADDRSTR(dstaddr), GASNETI_LADDRSTR((((char *)dstaddr)+pstats->dstextent)),
              (int)pstats->dstcontiguity, (int)pstats->dstcontigsz, (int)pstats->dstsegments,
              GASNETI_LADDRSTR(srcaddr), srcstrides_str, (int)pstats->srcextent,
              GASNETI_LADDRSTR(srcaddr), GASNETI_LADDRSTR((((char *)srcaddr)+pstats->srcextent)),
              (int)pstats->srccontiguity, (int)pstats->srccontigsz, (int)pstats->srcsegments
          );
  gasneti_assert(strlen(buf) < bufsz);

  gasneti_free(srcstrides_str);
  gasneti_free(dststrides_str);
  gasneti_free(count_str);
  return pstats->totalsz;
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
      gasneti_assert(sli->frozen > 0);
      sli->frozen--;
    }
  #else
    const char *gasneti_srcfilename = NULL;
    unsigned int gasneti_srclinenum = 0;
    unsigned int gasneti_srcfreeze = 0;
  #endif
#endif

#if GASNETI_STATS_OR_TRACE
  #define BUILD_STATS(type,name,desc) { #type, #name, #desc },
  gasneti_statinfo_t gasneti_stats[] = {
    GASNETI_ALL_STATS(BUILD_STATS, BUILD_STATS, BUILD_STATS)
    {NULL, NULL, NULL}
  };

  #define BUFSZ     8192
  #define NUMBUFS   4
  typedef struct {
    int bufidx;
    char bufs[NUMBUFS][BUFSZ];
  } gasneti_printbuf_t;
  GASNETI_THREADKEY_DEFINE(gasneti_printbuf_key);
  static void gasneti_printbuf_cleanup_threaddata(void *_td) {
      gasneti_free(_td);
  }

  /* give gcc enough information to type-check our format strings */
  GASNETI_FORMAT_PRINTF(gasneti_file_vprintf,2,0,
  static void gasneti_file_vprintf(FILE *fp, const char *format, va_list argptr));
  GASNETI_FORMAT_PRINTF(gasneti_trace_printf,1,2,
  static void gasneti_trace_printf(const char *format, ...));
  GASNETI_FORMAT_PRINTF(gasneti_stats_printf,1,2,
  static void gasneti_stats_printf(const char *format, ...));
  GASNETI_FORMAT_PRINTF(gasneti_tracestats_printf,1,2,
  static void gasneti_tracestats_printf(const char *format, ...));

  static char *gasneti_getbuf(void) {
    gasneti_printbuf_t * printbuf;
    if ((printbuf = gasneti_threadkey_get(gasneti_printbuf_key)) == NULL) {
      printbuf = gasneti_malloc(sizeof(gasneti_printbuf_t));
      gasneti_threadkey_set(gasneti_printbuf_key, printbuf);
      printbuf->bufidx = 0;
      gasnete_register_threadcleanup(gasneti_printbuf_cleanup_threaddata, printbuf);
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
          (int)gasnet_mynode(), (int)(uintptr_t)pthread_self(), time, srclinestr, *type,
          msg, (msg[strlen(msg)-1]=='\n'?"":"\n"));
      #else
        fprintf(fp, "%i %8.6fs>%s (%c) %s%s", (int)gasnet_mynode(), time, srclinestr, *type,
          msg, (msg[strlen(msg)-1]=='\n'?"":"\n"));
      #endif
    } else {
        fprintf(fp, "%i> (%c) %s%s", (int)gasnet_mynode(), *type, msg,
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
    fprintf(fp, "%i> ", (int)gasnet_mynode());
    vfprintf(fp, format, argptr);
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

static FILE *gasneti_open_outputfile(const char *filename, const char *desc) {
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
      sprintf(temp,"%s%i%s",pathtemp,(int)gasnet_mynode(),p+1);
      strcpy(pathtemp,temp);
    }
    filename = pathtemp;
    #ifdef HAVE_FOPEN64
      fp = fopen64(filename, "wt");
    #else
      fp = fopen(filename, "wt");
    #endif
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

char gasneti_exename[PATH_MAX];
static const char *gasneti_mallocreport_filename = NULL;

extern void gasneti_trace_init(int *pargc, char ***pargv) {
  gasneti_free(gasneti_malloc(1)); /* touch the malloc system to ensure it's intialized */

  /* ensure the arguments have been decoded */
  gasneti_decode_args(pargc, pargv); 

  gasneti_qualify_path(gasneti_exename, (*pargv)[0]);

 #if GASNETI_STATS_OR_TRACE
  starttime = gasneti_ticks_now();

  { /* setup tracefile */
    FILE *gasneti_tracefile_tmp = NULL, *gasneti_statsfile_tmp = NULL;
    char *tracefilename = gasneti_getenv_withdefault("GASNET_TRACEFILE","");
    char *statsfilename = gasneti_getenv_withdefault("GASNET_STATSFILE","");
    if (tracefilename && !strcmp(tracefilename, "")) tracefilename = NULL;
    if (statsfilename && !strcmp(statsfilename, "")) statsfilename = NULL;
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
    int i;
    char temp[1024];
    char *p;
    time(&ltime); 
    strcpy(temp, ctime(&ltime));
    if (temp[strlen(temp)-1] == '\n') temp[strlen(temp)-1] = '\0';
    gasneti_tracestats_printf("Program %s (pid=%i) starting on %s at: %s", 
      gasneti_exename, (int)getpid(), gasnett_gethostname(), temp);
    p = temp;
    for (i=0; i < *pargc; i++) { 
      char *q = (*pargv)[i];
      int hasspace = 0;
      for (;*q;q++) if (isspace((int)*q)) hasspace = 1;
      if (hasspace) sprintf(p, "'%s'", (*pargv)[i]);
      else sprintf(p, "%s", (*pargv)[i]);
      if (i < *pargc-1) strcat(p, " ");
      p += strlen(p);
    }
    gasneti_tracestats_printf("Command-line: %s", temp);
  }

  gasneti_tracestats_printf("GASNET_CONFIG_STRING: %s", GASNET_CONFIG_STRING);
  gasneti_tracestats_printf("GASNet build timestamp:   " __DATE__ " " __TIME__);
  gasneti_tracestats_printf("GASNet configure args:    %s", GASNETI_CONFIGURE_ARGS);
  gasneti_tracestats_printf("GASNet configure buildid: " GASNETI_BUILD_ID);
  gasneti_tracestats_printf("GASNet system tuple:      " GASNETI_SYSTEM_TUPLE);
  gasneti_tracestats_printf("GASNet configure system:  " GASNETI_SYSTEM_NAME);
  gasneti_tracestats_printf("gasnet_mynode(): %i", (int)gasnet_mynode());
  gasneti_tracestats_printf("gasnet_nodes(): %i", (int)gasnet_nodes());
  gasneti_tracestats_printf("gasneti_cpu_count(): %i", (int)gasneti_cpu_count());
  #if GASNET_STATS
    gasneti_stats_printf("GASNET_STATSMASK: %s", GASNETI_STATS_GETMASK());
  #endif
  #if GASNET_TRACE
    gasneti_trace_printf("GASNET_TRACEMASK: %s", GASNETI_TRACE_GETMASK());
    gasneti_trace_printf("GASNET_TRACEFLUSH: %i", gasneti_autoflush);
    gasneti_trace_printf("GASNET_TRACELOCAL: %i", !gasneti_trace_suppresslocal);
  #endif

  gasneti_mallocreport_filename = gasneti_getenv_withdefault("GASNET_MALLOCFILE","");
  if (gasneti_mallocreport_filename && !strcmp(gasneti_mallocreport_filename, "")) gasneti_mallocreport_filename = NULL;

  #if GASNET_NDEBUG
  { char *NDEBUG_warning =
     "WARNING: tracing/statistical collection may adversely affect application performance.";
    gasneti_tracestats_printf(NDEBUG_warning);
    if (gasneti_tracefile != stdout && gasneti_tracefile != stderr &&
        gasneti_statsfile != stdout && gasneti_statsfile != stderr) {
      fprintf(stderr,NDEBUG_warning);
      fprintf(stderr,"\n");
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
 #endif
}

#if GASNETI_STATS_OR_TRACE
#define AGGRNAME(cat,type) gasneti_aggregate_##cat##_##type
#define AGGR(type)                                       \
  GASNETI_UNUSED                                         \
  static gasneti_statctr_t AGGRNAME(ctr,type) = 0;       \
  GASNETI_UNUSED                                         \
  static gasneti_stat_intval_t AGGRNAME(intval,type) =   \
    { 0, GASNETI_STATCTR_MAX, GASNETI_STATCTR_MIN, 0 };  \
  GASNETI_UNUSED                                         \
  static gasneti_stat_timeval_t AGGRNAME(timeval,type) = \
    { 0, GASNETI_TICK_MAX, GASNETI_TICK_MIN, 0 }
AGGR(G);
AGGR(P);
AGGR(S);
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
    
      #define ACCUM(pacc, pintval) do {                                       \
          pacc->count += pintval->count;                                      \
          if (pintval->minval < pacc->minval) pacc->minval = pintval->minval; \
          if (pintval->maxval > pacc->maxval) pacc->maxval = pintval->maxval; \
          pacc->sumval += pintval->sumval;                                    \
      } while (0)
      #define CALC_AVG(sum,count) ((count) == 0 ? (gasneti_statctr_t)-1 : (sum) / (count))
      #define DUMP_CTR(type,name,desc)                     \
        if (GASNETI_STATS_ENABLED(type)) {                 \
          gasneti_statctr_t *p = &gasneti_stat_ctr_##name; \
          gasneti_stats_printf(" %-25s %6llu",             \
                #name" "#desc":", (unsigned long long)*p); \
          AGGRNAME(ctr,type) += *p;                        \
        }
      #define DUMP_INTVAL(type,name,desc)                           \
        if (GASNETI_STATS_ENABLED(type)) {                          \
          gasneti_stat_intval_t *p = &gasneti_stat_intval_##name;   \
          const char *pdesc = #desc;                                \
          if (!p->count)                                            \
            gasneti_stats_printf(" %-25s %6i", #name":", 0);        \
          else                                                      \
            gasneti_stats_printf(" %-25s %6llu  avg/min/max/total"  \
                                 " %s = %llu/%llu/%llu/%llu",       \
                  #name":", (unsigned long long)p->count,           \
                  pdesc,                                            \
                  (unsigned long long)CALC_AVG(p->sumval,p->count), \
                  (unsigned long long)p->minval,                    \
                  (unsigned long long)p->maxval,                    \
                  (unsigned long long)p->sumval);                   \
          ACCUM((&AGGRNAME(intval,type)), p);                       \
        }
      #define DUMP_TIMEVAL(type,name,desc)                                   \
        if (GASNETI_STATS_ENABLED(type)) {                                   \
          gasneti_stat_timeval_t *p = &gasneti_stat_timeval_##name;          \
          const char *pdesc = #desc;                                         \
          if (!p->count)                                                     \
            gasneti_stats_printf(" %-25s %6i", #name":", 0);                 \
          else                                                               \
            gasneti_stats_printf(" %-25s %6llu  avg/min/max/total"           \
                                 " %s (us) = %.3f/%.3f/%.3f/%.3f",           \
                  #name":", (unsigned long long)p->count,                    \
                  pdesc,                                                     \
                  gasneti_ticks_to_ns(CALC_AVG(p->sumval, p->count))/1000.0, \
                  gasneti_ticks_to_ns(p->minval)/1000.0,                     \
                  gasneti_ticks_to_ns(p->maxval)/1000.0,                     \
                  gasneti_ticks_to_ns(p->sumval)/1000.0);                    \
          ACCUM((&AGGRNAME(timeval,type)), p);                               \
        }

      GASNETI_ALL_STATS(DUMP_CTR, DUMP_INTVAL, DUMP_TIMEVAL);

      gasneti_stats_printf(" ");
      gasneti_stats_printf(" ");
      #define DUMP_AGGR_SZ(type,name) do {                                      \
        if (GASNETI_STATS_ENABLED(type)) {                                      \
          gasneti_stat_intval_t *p = &AGGRNAME(intval,type);                    \
          if (!p->count)                                                        \
            gasneti_stats_printf("%-25s  %6i","Total "#name":",0);              \
          else                                                                  \
            gasneti_stats_printf("%-25s  %6llu  avg/min/max/total"              \
                                 " sz = %llu/%llu/%llu/%llu", "Total "#name":", \
              (unsigned long long)p->count,                                     \
              (unsigned long long)CALC_AVG(p->sumval, p->count),                \
              (unsigned long long)p->minval,                                    \
              (unsigned long long)p->maxval,                                    \
              (unsigned long long)p->sumval);                                   \
        }                                                                       \
      } while (0)
      DUMP_AGGR_SZ(G,gets);
      DUMP_AGGR_SZ(P,puts);
      DUMP_AGGR_SZ(W,collectives);
      if (GASNETI_STATS_ENABLED(S)) {
        gasneti_stat_intval_t *try_succ = &AGGRNAME(intval,S);
        gasneti_stat_timeval_t *wait_time = &AGGRNAME(timeval,S);
        if (!try_succ->count)
          gasneti_stats_printf("%-25s  %6i","Total try sync. calls:",0);
        else
          gasneti_stats_printf("%-25s  %6llu  try success rate = %f%%  \n",
            "Total try sync. calls:",  (unsigned long long)try_succ->count,
            (float)(CALC_AVG((float)try_succ->sumval, try_succ->count) * 100.0));
        if (!wait_time->count)
          gasneti_stats_printf("%-25s  %6i","Total wait sync. calls:",0);
        else
          gasneti_stats_printf("%-25s  %6llu  avg/min/max/total waittime (us) = %.3f/%.3f/%.3f/%.3f", 
            "Total wait sync. calls:", (unsigned long long)wait_time->count,
            gasneti_ticks_to_ns(CALC_AVG(wait_time->sumval, wait_time->count))/1000.0,
            gasneti_ticks_to_ns(wait_time->minval)/1000.0,
            gasneti_ticks_to_ns(wait_time->maxval)/1000.0,
            gasneti_ticks_to_ns(wait_time->sumval)/1000.0);
      }
      if (GASNETI_STATS_ENABLED(X)) {
        gasneti_stat_intval_t *try_succ = &AGGRNAME(intval,X);
        gasneti_stat_timeval_t *wait_time = &AGGRNAME(timeval,X);
        if (!try_succ->count)
          gasneti_stats_printf("%-25s  %6i","Total coll. try syncs:",0);
        else
          gasneti_stats_printf("%-25s  %6llu  collective try success rate = %f%%  \n",
            "Total coll. try syncs:",  (unsigned long long)try_succ->count,
            (float)(CALC_AVG((float)try_succ->sumval, try_succ->count) * 100.0));
        if (!wait_time->count)
          gasneti_stats_printf("%-25s  %6i","Total coll. wait syncs:",0);
        else
          gasneti_stats_printf("%-25s  %6llu  avg/min/max/total waittime (us) = %.3f/%.3f/%.3f/%.3f", 
            "Total coll. wait syncs:", (unsigned long long)wait_time->count,
            gasneti_ticks_to_ns(CALC_AVG(wait_time->sumval, wait_time->count))/1000.0,
            gasneti_ticks_to_ns(wait_time->minval)/1000.0,
            gasneti_ticks_to_ns(wait_time->maxval)/1000.0,
            gasneti_ticks_to_ns(wait_time->sumval)/1000.0);
      }
      if (GASNETI_STATS_ENABLED(A)) 
        gasneti_stats_printf("%-25s  %6llu", "Total AM's:", (unsigned long long)AGGRNAME(ctr,A));

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
#if GASNET_DEBUG
  if (gasneti_mallocreport_filename) {
    static gasneti_mutex_t gasneti_debugmalloclock = GASNETI_MUTEX_INITIALIZER;
    FILE *fp;
    gasneti_mutex_lock(&gasneti_debugmalloclock);
    fp = gasneti_open_outputfile(gasneti_mallocreport_filename, "malloc report");
    if (fp) {
      gasnett_heapstats_t stats;
      gasnett_getheapstats(&stats);
      fprintf(fp, "# GASNet Debug Mallocator Report\n");
      fprintf(fp, "#\n");
      fprintf(fp, "# program: %s\n",gasneti_exename);
      fprintf(fp, "# host:    %s\n",gasnett_gethostname());
      fprintf(fp, "# pid:     %i\n",(int)getpid());
      fprintf(fp, "# node:    %i / %i\n", (int)gasnet_mynode(), (int)gasnet_nodes());
      fprintf(fp, "#\n");
      fprintf(fp, "# Private memory utilization:\n");
      fprintf(fp, "# ---------------------------\n");
      fprintf(fp, "#\n");
      fprintf(fp, "# malloc() space total:        %10lu bytes, in %10lu objects\n",
        (long unsigned)stats.allocated_bytes, (long unsigned)stats.allocated_objects);
      fprintf(fp, "# malloc() space in-use:       %10lu bytes, in %10lu objects\n",
        (unsigned long)stats.live_bytes, (unsigned long)stats.live_objects);
      fprintf(fp, "# malloc() space freed:        %10lu bytes, in %10lu objects\n",
        (unsigned long)stats.freed_bytes, (unsigned long)stats.freed_objects);
      fprintf(fp, "# malloc() space peak usage:   %10lu bytes,    %10lu objects\n",
        (unsigned long)stats.live_bytes_max, (unsigned long)stats.live_objects_max);
      fprintf(fp, "# malloc() system overhead: >= %10lu bytes\n",
        (unsigned long)stats.overhead_bytes);
      fprintf(fp, "#\n");

      gasneti_memcheck_all(); /* check ring sanity */

      fprintf(fp, "# Live objects at job exit\n");
      fprintf(fp, "# ------------------------\n");
      fprintf(fp, "#\n");
      fprintf(fp, "# Table below shows objects allocated, but not freed by job exit.\n");
      fprintf(fp, "# Note that GASNet does not free most of its internal permanent data structures,\n");
      fprintf(fp, "# in order to streamline job shutdown.\n");
      fprintf(fp, "#\n");
      fprintf(fp, "# Object size     Location Allocated\n");
      fprintf(fp, "# ==================================\n");

      gasneti_malloc_dump_liveobjects(fp);
      fclose(fp);
    }
    gasneti_mutex_unlock(&gasneti_debugmalloclock);
  }
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
    pintval->count++;
    pintval->sumval += val;
    if_pf (val > pintval->maxval) pintval->maxval = val;
    if_pf (val < pintval->minval) pintval->minval = val;
  GASNETI_STAT_UNLOCK();
}
extern void gasneti_stat_timeval_accumulate(gasneti_stat_timeval_t *pintval, gasneti_tick_t val) {
  GASNETI_STAT_LOCK();
    pintval->count++;
    pintval->sumval += val;
    if_pf (val > pintval->maxval) pintval->maxval = val;
    if_pf (val < pintval->minval) pintval->minval = val;
  GASNETI_STAT_UNLOCK();
}
#endif

