/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_trace.h $
 * Description: GASNet Tracing Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_TRACE_H
#define _GASNET_TRACE_H

#include <stdio.h>

/* ------------------------------------------------------------------------------------ */
/* Statistical collection & tracing 
   See README for user-interface usage information
*/

#if GASNETI_STATS_OR_TRACE
  /* emit trace info and increment a stat ctr */
  #define GASNETI_TRACE_EVENT(type, name) do { \
       _GASNETI_STAT_EVENT (type, name);       \
       _GASNETI_TRACE_EVENT(type, name);       \
      } while (0)

  /* emit trace info and accumulate an integer stat value */
  #define GASNETI_TRACE_EVENT_VAL(type, name, val) do { \
       gasneti_statctr_t _te_val = (val);               \
       _GASNETI_STAT_EVENT_VAL (type, name, _te_val);   \
       _GASNETI_TRACE_EVENT_VAL(type, name, _te_val);   \
      } while (0)

  /* emit trace info and accumulate a time stat value */
  #define GASNETI_TRACE_EVENT_TIME(type, name, time) do { \
       gasneti_tick_t _te_time = (time);                  \
       _GASNETI_STAT_EVENT_TIME(type, name, _te_time);    \
       _GASNETI_TRACE_EVENT_TIME(type, name, _te_time);   \
      } while (0)
#else
  #define GASNETI_TRACE_EVENT(type, name)            ((void)0)
  #define GASNETI_TRACE_EVENT_VAL(type, name, val)   ((void)0)
  #define GASNETI_TRACE_EVENT_TIME(type, name, time) ((void)0)
#endif

#if GASNET_TRACE
  /* print a string on the trace 
     Ex: GASNETI_TRACE_MSG(C, "init complete") */
  #define GASNETI_TRACE_MSG(type, string) \
      GASNETI_TRACE_PRINTF(type, ("%s",(string)))

  /* print a formatted string on output
     Ex: GASNETI_TRACE_PRINTF(C, ("%i buffers free", numbufs))
      (note the extra parentheses around arg)
  */
  #define GASNETI_TRACE_PRINTF(type, args) do { \
    if (GASNETI_TRACE_ENABLED(type)) {          \
      char *_tpf_msg = gasneti_dynsprintf args; \
      gasneti_trace_output(#type, _tpf_msg, 1); \
    }                                           \
  } while(0)
#else
  #define GASNETI_TRACE_MSG(type, string)   ((void)0)
  #define GASNETI_TRACE_PRINTF(type, args)  ((void)0)
#endif

#if GASNET_STATS
  /* print an arbitrary string of statistical output on the trace 
     Ex: GASNETI_STATS_MSG(C, "init complete") */
  #define GASNETI_STATS_MSG(type, string) \
      GASNETI_STATS_PRINTF(type, ("%s",(string)))

  /* print a formatted string of statistical output on the trace 
     Ex: GASNETI_STATS_PRINTF(C, ("%i buffers free", numbufs))
      (note the extra parentheses around arg)
  */
  #define GASNETI_STATS_PRINTF(type, args) do { \
    if (GASNETI_STATS_ENABLED(type)) {          \
      char *_spf_msg = gasneti_dynsprintf args; \
      gasneti_stats_output(#type, _spf_msg, 0); \
    }                                           \
  } while(0)
#else
  #define GASNETI_STATS_MSG(type, string)   ((void)0)
  #define GASNETI_STATS_PRINTF(type, args)  ((void)0)
#endif

/* allow for final output of conduit-core specific statistics */
#ifndef GASNETC_TRACE_FINISH
#define GASNETC_TRACE_FINISH()  ((void)0)
#endif
/* allow for final output of conduit-extended specific statistics */
#ifndef GASNETE_TRACE_FINISH
#define GASNETE_TRACE_FINISH()  ((void)0)
#endif

#ifndef GASNETI_STATS_ECHOED_TO_TRACEFILE
#define GASNETI_STATS_ECHOED_TO_TRACEFILE 1
#endif

#if GASNET_SRCLINES
  /* GASNETI_TRACE_SETSOURCELINE(filename, linenum): 
     set the current "high-level" source file and line for the current thread
     to the given value. This information is used to dump out current line information
     into the tracefile along with each trace message.
     Passing a NULL filename implies no change to the current filename
     GASNETI_TRACE_GETSOURCELINE(pfilename, plinenum): 
     fetch the current "high-level" source file and line for the current thread
     into *pfilename and *plinenum. no-op when tracing is off.
   */
  #if GASNETI_CLIENT_THREADS
    extern void gasneti_trace_setsourceline(const char *_filename, unsigned int _linenum);
    extern void gasneti_trace_getsourceline(const char **_pfilename, unsigned int *_plinenum);
    extern void gasneti_trace_freezesourceline(void);
    extern void gasneti_trace_unfreezesourceline(void);
  #else
    extern const char *gasneti_srcfilename;
    extern unsigned int gasneti_srclinenum;
    extern unsigned int gasneti_srcfreeze;
    GASNETI_INLINE(gasneti_trace_setsourceline)
    void gasneti_trace_setsourceline(const char *_filename, unsigned int _linenum) {
      if_pt (gasneti_srcfreeze == 0) {
        if_pt (_filename != NULL) gasneti_srcfilename = _filename;
        gasneti_srclinenum = _linenum;
      }
    }
    GASNETI_INLINE(gasneti_trace_getsourceline)
    void gasneti_trace_getsourceline(const char **_pfilename, unsigned int *_plinenum) {
      *_pfilename = gasneti_srcfilename;
      *_plinenum = gasneti_srclinenum;
    }
    GASNETI_INLINE(gasneti_trace_freezesourceline)
    void gasneti_trace_freezesourceline(void) {
      gasneti_srcfreeze++;
    }
    GASNETI_INLINE(gasneti_trace_unfreezesourceline)
    void gasneti_trace_unfreezesourceline(void) {
      gasneti_assert_int(gasneti_srcfreeze ,>, 0);
      gasneti_srcfreeze--;
    }
  #endif

  /* GASNETI_SRCLINES_FORCE ensures we always track srclines
     otherwise we only track them as needed for tracing purposes */
  #ifdef GASNETI_SRCLINES_FORCE
    #if 1 /* All currently supported platforms */
      #define GASNETI_SRCLINE_TRACKING() (1)
    #endif
  #else
    #define GASNETI_SRCLINE_TRACKING() GASNETI_TRACE_ENABLED(N)
  #endif
  #if 1 /* All currently supported platforms */
    #define GASNETI_TRACE_SETSOURCELINE(filename, linenum) \
      (GASNETI_SRCLINE_TRACKING() ? gasneti_trace_setsourceline((const char *)filename, (unsigned int)linenum) : ((void)0))
  #endif
  #define GASNETI_TRACE_GETSOURCELINE(pfilename, plinenum) \
      (GASNETI_SRCLINE_TRACKING() ? gasneti_trace_getsourceline(pfilename, plinenum) : ((void)0))
  #define GASNETI_TRACE_FREEZESOURCELINE() \
      (GASNETI_SRCLINE_TRACKING() ? gasneti_trace_freezesourceline() : ((void)0))
  #define GASNETI_TRACE_UNFREEZESOURCELINE() \
      (GASNETI_SRCLINE_TRACKING() ? gasneti_trace_unfreezesourceline() : ((void)0))
#else
  #define GASNETI_TRACE_SETSOURCELINE(filename, linenum)   ((void)0)
  #define GASNETI_TRACE_GETSOURCELINE(pfilename, plinenum) ((void)0)
  #define GASNETI_TRACE_FREEZESOURCELINE()                 ((void)0)
  #define GASNETI_TRACE_UNFREEZESOURCELINE()               ((void)0)
#endif

/* ------------------------------------------------------------------------------------ */
/* misc helpers for specific tracing scenarios */

#define GASNETI_TMFMT "%s"
#define GASNETI_TMSTR(tm) gasneti_formattm(tm)
#define GASNETI_TMRANKFMT GASNETI_TMFMT ":%i"
#define GASNETI_TMRANKSTR(tm,rank) GASNETI_TMSTR(tm),(rank)
// Team and *self* rank (note multiple evaluation of tm):
#define GASNETI_TMSELFFMT GASNETI_TMRANKFMT
#define GASNETI_TMSELFSTR(tm) GASNETI_TMRANKSTR(tm,gex_TM_QueryRank(tm))

#if PLATFORM_ARCH_32 
  #define GASNETI_LADDRFMT "0x%08" PRIxPTR
  #define GASNETI_LADDRSTR(ptr) ((uintptr_t)(ptr))
  #define GASNETI_RADDRFMT "(" GASNETI_TMRANKFMT ",0x%08" PRIxPTR ")"
  #define GASNETI_RADDRSTR(tm,rank,ptr) GASNETI_TMRANKSTR((tm),(rank)),GASNETI_LADDRSTR(ptr)
#else
  #define GASNETI_LADDRFMT "0x%08x %08x"
  #define GASNETI_LADDRSTR(ptr) GASNETI_HIWORD(ptr), GASNETI_LOWORD(ptr)
  #define GASNETI_RADDRFMT "(" GASNETI_TMRANKFMT ",0x%08x %08x)"
  #define GASNETI_RADDRSTR(tm,rank,ptr) GASNETI_TMRANKSTR((tm),(rank)),GASNETI_LADDRSTR(ptr)
#endif


#if GASNET_TRACE
  #define GASNETI_TRACE_GETPUT(type, name, nbytes, tm, rank)                  \
    GASNETI_TRACE_PRINTF(type, ("%s: %s = %6" PRIuPTR ",  peer = "            \
                                GASNETI_TMRANKFMT, #name,                     \
                                gasneti_stats[(int)GASNETI_STAT_##name]._desc,\
                                (uintptr_t)(nbytes), GASNETI_TMRANKSTR((tm),(rank))));
  #define GASNETI_TRACE_GETPUT_NONLOCAL GASNETI_TRACE_GETPUT
  #define GASNETI_TRACE_GETPUT_LOCAL(type, name, nbytes, tm, rank) do {  \
    if (GASNETI_TRACE_ENABLED(type) && !gasneti_trace_suppresslocal)     \
      GASNETI_TRACE_GETPUT(type, name, nbytes, tm, rank);                \
    } while (0)
#else
  #define GASNETI_TRACE_GETPUT_NONLOCAL(type, name, nbytes, tm, rank) ((void)0)
  #define GASNETI_TRACE_GETPUT_LOCAL(type, name, nbytes, tm, rank)    ((void)0)
#endif

#define GASNETI_TRACE_GET_NAMED(name,locality,tm,dest,rank,src,nbytes) do {                \
  _GASNETI_STAT_EVENT_VAL (G, name, (nbytes));                                             \
  GASNETI_TRACE_GETPUT_##locality(G, name, (nbytes), (tm), (rank));                        \
  GASNETI_TRACE_PRINTF(D,(#name ": " GASNETI_LADDRFMT" <- " GASNETI_RADDRFMT" (%" PRIuPTR " bytes)", \
                          GASNETI_LADDRSTR(dest), GASNETI_RADDRSTR((tm),(rank),(src)),     \
                          (uintptr_t)(nbytes)));                                           \
} while (0)

#if GASNETI_STATS_OR_TRACE
#define GASNETI_TRACE_PUT_NAMED(name,locality,tm,rank,dest,src,nbytes) do {                    \
  void *_tpn_src = (src);  /* workaround for CrayC warning */                                  \
  _GASNETI_STAT_EVENT_VAL (P, name, (nbytes));                                                 \
  GASNETI_TRACE_GETPUT_##locality(P, name, (nbytes), tm, rank);                                \
  GASNETI_TRACE_PRINTF(D,(#name ": " GASNETI_RADDRFMT" <- " GASNETI_LADDRFMT" (%" PRIuPTR " bytes): %s", \
                          GASNETI_RADDRSTR((tm),(rank),(dest)), GASNETI_LADDRSTR(_tpn_src),    \
                          (uintptr_t)(nbytes), gasneti_formatdata(_tpn_src,(nbytes))));        \
} while (0)
#else
#define GASNETI_TRACE_PUT_NAMED(name,locality,tm,rank,dest,src,nbytes) ((void)0)
#endif

/* tracing for remote gets/puts */
#define GASNETI_TRACE_GET(variety,tm,dest,rank,src,nbytes) \
  GASNETI_TRACE_GET_NAMED(GET_##variety,NONLOCAL,tm,dest,rank,src,nbytes)

#define GASNETI_TRACE_PUT(variety,tm,rank,dest,src,nbytes) \
  GASNETI_TRACE_PUT_NAMED(PUT_##variety,NONLOCAL,tm,rank,dest,src,nbytes)   

/* tracing for local gets/puts (separation allows suppression of trace output) */
#define GASNETI_TRACE_GET_LOCAL(variety,tm,dest,rank,src,nbytes) \
  GASNETI_TRACE_GET_NAMED(GET_##variety##_LOCAL,LOCAL,tm,dest,rank,src,nbytes)

#define GASNETI_TRACE_PUT_LOCAL(variety,tm,rank,dest,src,nbytes) \
  GASNETI_TRACE_PUT_NAMED(PUT_##variety##_LOCAL,LOCAL,tm,rank,dest,src,nbytes)   

/*------------------------------------------------------------------------------------*/

#if GASNET_TRACE
  #define GASNETI_TRACE_BARRIER1(tm, flags) do { \
    /* TODO-EX: _GASNETI_STAT_EVENT for TM barrier */                   \
    GASNETI_TRACE_PRINTF(B,("Blocking Barrier: TM=" GASNETI_TMFMT " flags=%d",   \
                            GASNETI_TMSTR(tm), flags));                 \
  } while (0)
  #define GASNETI_TRACE_BARRIER2(tm, flags) do { \
    /* TODO-EX: _GASNETI_STAT_EVENT for TM barrier */                   \
    GASNETI_TRACE_PRINTF(B,("Non-blocking Barrier: TM=" GASNETI_TMFMT " flags=%d",   \
                            GASNETI_TMSTR(tm), flags));                 \
  } while (0)
#elif GASNET_STATS
  // TODO-EX: _GASNETI_STAT_EVENT for TM barrier
  #define GASNETI_TRACE_BARRIER1(tm, flags) ((void)0)
  #define GASNETI_TRACE_BARRIER2(tm, flags) ((void)0)
#else
  #define GASNETI_TRACE_BARRIER1(tm, flags) ((void)0)
  #define GASNETI_TRACE_BARRIER2(tm, flags) ((void)0)
#endif

/*------------------------------------------------------------------------------------*/
#define GASNETI_TRACE_TRYSYNC(name,success) \
  GASNETI_TRACE_EVENT_VAL(S,name,((success) == GASNET_OK?1:0))

#if GASNETI_STATS_OR_TRACE
  #define GASNETI_TRACE_WAITSYNC_BEGIN() \
    gasneti_tick_t _waitstart = GASNETI_TICKS_NOW_IFENABLED(S)
#else 
  #define GASNETI_TRACE_WAITSYNC_BEGIN()  ((void)0)
#endif

#if GASNET_STATS
  typedef void (*gasnett_stats_callback_t)(void (*)(const char *, ...));
  extern void (*gasnett_stats_callback)(
    GASNETI_FORMAT_PRINTF_FUNCPTR_ARG(_format,1,2,void (*_format)(const char *, ...))
  );
#endif

#define GASNETI_TRACE_WAITSYNC_END(name) \
  GASNETI_TRACE_EVENT_TIME(S,name,GASNETI_TICKS_NOW_IFENABLED(S) - _waitstart)
/*------------------------------------------------------------------------------------*/
/* AM Request/Reply tracing helpers */
#define _GASNETI_TRACE_GATHERARGS(numargs,lastarg)                  \
  char _tga_argstr[256];                                            \
  do {                                                              \
    char *_tga_aptr = _tga_argstr;                                  \
    size_t _tga_aspace = sizeof(_tga_argstr);                       \
    va_list _tga_argptr;                                            \
    *_tga_aptr = '\0';                                              \
    va_start(_tga_argptr, lastarg);                                 \
      for (int _tga_i=0;_tga_i<(numargs);_tga_i++) {                \
        /* must be int due to default argument promotion */         \
        int _tga_len = snprintf(_tga_aptr,_tga_aspace," 0x%08x",    \
                           (int)(uint32_t)va_arg(_tga_argptr,int)); \
        _tga_aptr += _tga_len; _tga_aspace -= _tga_len;             \
      }                                                             \
    va_end(_tga_argptr);                                            \
  } while(0)

#define GASNETI_TRACE_AMSHORT(name,tm,rank,handler,flags,numargs) do {               \
  _GASNETI_TRACE_GATHERARGS(numargs,numargs);                                        \
  _GASNETI_STAT_EVENT(A,name);                                                       \
  GASNETI_TRACE_PRINTF(A,(#name": dest=" GASNETI_TMRANKFMT                           \
                                " handler=%i flags=0x%x args:%s",                    \
    GASNETI_TMRANKSTR(tm,rank),handler,flags,_tga_argstr));                          \
} while(0)

#define GASNETI_TRACE_AMMEDIUM(name,tm,rank,handler,source_addr,nbytes,flags,numargs) do {                \
  _GASNETI_TRACE_GATHERARGS(numargs,numargs);                                                             \
  _GASNETI_STAT_EVENT_VAL(A,name,nbytes);                                                                 \
  GASNETI_TRACE_PRINTF(A,(#name": dest=" GASNETI_TMRANKFMT                                                \
                                " handler=%i source_addr=" GASNETI_LADDRFMT                               \
                                " nbytes=%" PRIuSZ " flags=0x%x args:%s",                                 \
    GASNETI_TMRANKSTR(tm,rank),handler,GASNETI_LADDRSTR(source_addr),(size_t)nbytes,flags,_tga_argstr));  \
  GASNETI_TRACE_PRINTF(D,(#name": payload data: %s", gasneti_formatdata(source_addr,nbytes)));            \
} while(0)

#define GASNETI_TRACE_AMLONG(name,tm,rank,handler,source_addr,nbytes,dest_addr,flags,numargs) do {\
  _GASNETI_TRACE_GATHERARGS(numargs,numargs);                                                  \
  _GASNETI_STAT_EVENT_VAL(A,name,nbytes);                                                      \
  GASNETI_TRACE_PRINTF(A,(#name": dest=" GASNETI_TMRANKFMT                                     \
                                " handler=%i source_addr=" GASNETI_LADDRFMT                    \
                                " nbytes=%" PRIuSZ " dest_addr=" GASNETI_LADDRFMT" flags=0x%x args:%s",   \
    GASNETI_TMRANKSTR(tm,rank),handler,GASNETI_LADDRSTR(source_addr),(size_t)nbytes,           \
    GASNETI_LADDRSTR(dest_addr),flags,_tga_argstr));                                           \
  GASNETI_TRACE_PRINTF(D,(#name": payload data: %s", gasneti_formatdata(source_addr,nbytes))); \
} while(0)

#define GASNETI_TRACE_PREP_MEDIUM(name,tm,rank,cbuf,least_pl,most_pl,flags,numargs) do {                  \
  GASNETI_TRACE_PRINTF(A,(#name": dest=" GASNETI_TMRANKFMT " client_buf=" GASNETI_LADDRFMT                \
                                " least_payload=%" PRIuSZ                                                 \
                                " most_payload=%" PRIuSZ " flags=0x%x numargs=%i",                        \
    GASNETI_TMRANKSTR(tm,rank),GASNETI_LADDRSTR(cbuf),(size_t)least_pl,(size_t)most_pl,                   \
    flags,(int)numargs));                                                                                 \
} while(0)

#define GASNETI_TRACE_PREP_LONG(name,tm,rank,cbuf,least_pl,most_pl,dest_addr,flags,numargs) do {          \
  GASNETI_TRACE_PRINTF(A,(#name": dest=" GASNETI_TMRANKFMT " client_buf=" GASNETI_LADDRFMT                \
                                " least_payload=%" PRIuSZ                                                 \
                                " most_payload=%" PRIuSZ " dest_addr=" GASNETI_LADDRFMT                   \
                                " flags=0x%x numargs=%i",                                                 \
    GASNETI_TMRANKSTR(tm,rank),GASNETI_LADDRSTR(cbuf),(size_t)least_pl,(size_t)most_pl,                   \
    GASNETI_LADDRSTR(dest_addr),flags,(int)numargs));                                                     \
} while(0)

#define _GASNETI_TRACE_PREP_RETURN(name,sd) do { \
  gasneti_AM_SrcDesc_t _tpr_sd = (sd);                                                      \
  if (_tpr_sd) {                                                                            \
    _GASNETI_STAT_EVENT_VAL(A,name,_tpr_sd->_size);                                         \
    GASNETI_TRACE_PRINTF(A,(#name": return { Addr=" GASNETI_LADDRFMT " Size=%" PRIuSZ " }", \
                            GASNETI_LADDRSTR(_tpr_sd->_addr), (size_t)(_tpr_sd->_size)));   \
  } else {                                                                                  \
    GASNETI_TRACE_PRINTF(A,(#name": return GEX_AM_SRCDESC_NO_OP"));                         \
  }                                                                                         \
} while(0)

#define GASNETI_TRACE_COMMIT_MEDIUM(name,handler,source_addr,nbytes,numargs) do {              \
  _GASNETI_TRACE_GATHERARGS(numargs,sd_arg); /* assumes sd_arg is final fixed arg */           \
  _GASNETI_STAT_EVENT_VAL(A,name,nbytes);                                                      \
  GASNETI_TRACE_PRINTF(A,(#name": handler=%i nbytes=%" PRIuSZ " args:%s",                      \
                          handler,(size_t)nbytes,_tga_argstr));                                \
  GASNETI_TRACE_PRINTF(D,(#name": payload data: %s", gasneti_formatdata(source_addr,nbytes))); \
} while(0)

#define GASNETI_TRACE_COMMIT_LONG(name,handler,source_addr,nbytes,dest_addr,numargs) do {               \
  _GASNETI_TRACE_GATHERARGS(numargs,sd_arg); /* assumes sd_arg is final fixed arg */                    \
  _GASNETI_STAT_EVENT_VAL(A,name,nbytes);                                                               \
  GASNETI_TRACE_PRINTF(A,(#name": handler=%i nbytes=%" PRIuSZ " dest_addr=" GASNETI_LADDRFMT" args:%s", \
                          handler,(size_t)nbytes,GASNETI_LADDRSTR(dest_addr),_tga_argstr));             \
  GASNETI_TRACE_PRINTF(D,(#name": payload data: %s", gasneti_formatdata(source_addr,nbytes)));          \
} while(0)

#if GASNET_TRACE
  #define GASNETI_TRACE_AMREQUESTSHORT(tm,dest,handler,flags,numargs) \
          GASNETI_TRACE_AMSHORT(AMREQUEST_SHORT,tm,dest,handler,flags,numargs)
  #define GASNETI_TRACE_AMREPLYSHORT(token,handler,flags,numargs) do {   \
          gex_Token_Info_t _tar_info;                                    \
          gex_Token_Info(token, &_tar_info, GEX_TI_SRCRANK);             \
          GASNETI_TRACE_AMSHORT(AMREPLY_SHORT,NULL,_tar_info.gex_srcrank,handler,flags,numargs); \
          GASNETI_TRACE_PRINTF(C,("AMREPLY_SHORT: Reply token: %s",      \
                            gasneti_formatdata(&token, sizeof(token)))); \
  } while(0)

  #define GASNETI_TRACE_AMREQUESTMEDIUM(tm,dest,handler,source_addr,nbytes,flags,numargs) \
          GASNETI_TRACE_AMMEDIUM(AMREQUEST_MEDIUM,tm,dest,handler,source_addr,nbytes,flags,numargs)
  #define GASNETI_TRACE_AMREPLYMEDIUM(token,handler,source_addr,nbytes,flags,numargs) do {      \
          gex_Token_Info_t _tar_info;                                                           \
          gex_Token_Info(token, &_tar_info, GEX_TI_SRCRANK);                                    \
          GASNETI_TRACE_AMMEDIUM(AMREPLY_MEDIUM,NULL,_tar_info.gex_srcrank,handler,source_addr,nbytes,flags,numargs); \
          GASNETI_TRACE_PRINTF(C,("AMREPLY_MEDIUM: Reply token: %s",                      \
                            gasneti_formatdata(&token, sizeof(token))));                  \
  } while(0)

  #define GASNETI_TRACE_AMREQUESTLONG(tm,dest,handler,source_addr,nbytes,dest_addr,flags,numargs) \
          GASNETI_TRACE_AMLONG(AMREQUEST_LONG,tm,dest,handler,source_addr,nbytes,dest_addr,flags,numargs)
  #define GASNETI_TRACE_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,flags,numargs) do {    \
          gex_Token_Info_t _tar_info;                                                                 \
          gex_Token_Info(token, &_tar_info, GEX_TI_SRCRANK);                                          \
          GASNETI_TRACE_AMLONG(AMREPLY_LONG,NULL,_tar_info.gex_srcrank,handler,source_addr,nbytes,dest_addr,flags,numargs); \
          GASNETI_TRACE_PRINTF(C,("AMREPLY_LONG: Reply token: %s",                              \
                            gasneti_formatdata(&token, sizeof(token))));                        \
  } while(0)

  #define GASNETI_TRACE_PREP_REQUESTMEDIUM(tm,dest,cbuf,least_pl,most_pl,flags,numargs) \
          GASNETI_TRACE_PREP_MEDIUM(PREP_REQUEST_MEDIUM,tm,dest,cbuf,least_pl,most_pl,flags,numargs)
  #define GASNETI_TRACE_PREP_REPLYMEDIUM(token,cbuf,least_pl,most_pl,flags,numargs) do { \
          gex_Token_Info_t _tar_info;                                                    \
          gex_Token_Info(token, &_tar_info, GEX_TI_SRCRANK);                             \
          GASNETI_TRACE_PREP_MEDIUM(PREP_REPLY_MEDIUM,NULL,_tar_info.gex_srcrank,cbuf,least_pl,most_pl,flags,numargs); \
  } while(0)

  #define GASNETI_TRACE_PREP_REQUESTLONG(tm,dest,cbuf,least_pl,most_pl,dest_addr,flags,numargs) \
          GASNETI_TRACE_PREP_LONG(PREP_REQUEST_LONG,tm,dest,cbuf,least_pl,most_pl,dest_addr,flags,numargs)
  #define GASNETI_TRACE_PREP_REPLYLONG(token,cbuf,least_pl,most_pl,dest_addr,flags,numargs) do { \
          gex_Token_Info_t _tar_info;                                                            \
          gex_Token_Info(token, &_tar_info, GEX_TI_SRCRANK);                                     \
          GASNETI_TRACE_PREP_LONG(PREP_REPLY_LONG,NULL,_tar_info.gex_srcrank,cbuf,least_pl,most_pl,dest_addr,flags,numargs);\
  } while(0)

  #define GASNETI_TRACE_PREP_RETURN(name,sd) _GASNETI_TRACE_PREP_RETURN(PREP_##name,sd)

  #define GASNETI_TRACE_COMMIT_REQUESTMEDIUM(handler,source_addr,nbytes,numargs) \
          GASNETI_TRACE_COMMIT_MEDIUM(COMMIT_REQUEST_MEDIUM,handler,source_addr,nbytes,numargs)
  #define GASNETI_TRACE_COMMIT_REPLYMEDIUM(handler,source_addr,nbytes,numargs) \
          GASNETI_TRACE_COMMIT_MEDIUM(COMMIT_REPLY_MEDIUM,handler,source_addr,nbytes,numargs)

  #define GASNETI_TRACE_COMMIT_REQUESTLONG(handler,source_addr,nbytes,dest_addr,numargs) \
          GASNETI_TRACE_COMMIT_LONG(COMMIT_REQUEST_LONG,handler,source_addr,nbytes,dest_addr,numargs)
  #define GASNETI_TRACE_COMMIT_REPLYLONG(handler,source_addr,nbytes,dest_addr,numargs) \
          GASNETI_TRACE_COMMIT_LONG(COMMIT_REPLY_LONG,handler,source_addr,nbytes,dest_addr,numargs)

#elif GASNET_STATS
  #define GASNETI_TRACE_AMREQUESTSHORT(tm,dest,handler,flags,numargs) \
     GASNETI_TRACE_EVENT(A,AMREQUEST_SHORT)
  #define GASNETI_TRACE_AMREPLYSHORT(token,handler,flags,numargs) \
     GASNETI_TRACE_EVENT(A,AMREPLY_SHORT)
  #define GASNETI_TRACE_AMREQUESTMEDIUM(tm,dest,handler,source_addr,nbytes,flags,numargs) \
     GASNETI_TRACE_EVENT_VAL(A,AMREQUEST_MEDIUM,nbytes)
  #define GASNETI_TRACE_AMREPLYMEDIUM(token,handler,source_addr,nbytes,flags,numargs) \
     GASNETI_TRACE_EVENT_VAL(A,AMREPLY_MEDIUM,nbytes)
  #define GASNETI_TRACE_AMREQUESTLONG(tm,dest,handler,source_addr,nbytes,dest_addr,flags,numargs) \
     GASNETI_TRACE_EVENT_VAL(A,AMREQUEST_LONG,nbytes)
  #define GASNETI_TRACE_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,flags,numargs) \
     GASNETI_TRACE_EVENT_VAL(A,AMREPLY_LONG,nbytes)
  #define GASNETI_TRACE_PREP_REQUESTMEDIUM(tm,dest,cbuf,least_pl,most_pl,flags,numargs)          ((void)0)
  #define GASNETI_TRACE_PREP_REPLYMEDIUM(token,cbuf,least_pl,most_pl,flags,numargs)              ((void)0)
  #define GASNETI_TRACE_PREP_REQUESTLONG(tm,dest,cbuf,least_pl,most_pl,dest_addr,flags,numargs)  ((void)0)
  #define GASNETI_TRACE_PREP_REPLYLONG(token,cbuf,least_pl,most_pl,dest_addr,flags,numargs)      ((void)0)
  #define GASNETI_TRACE_PREP_RETURN(name,sd) \
     _GASNETI_TRACE_PREP_RETURN(PREP_##name,sd)
  #define GASNETI_TRACE_COMMIT_REQUESTMEDIUM(handler,source_addr,nbytes,numargs) \
     GASNETI_TRACE_EVENT_VAL(A,COMMIT_REQUEST_MEDIUM,nbytes)
  #define GASNETI_TRACE_COMMIT_REPLYMEDIUM(handler,source_addr,nbytes,numargs) \
     GASNETI_TRACE_EVENT_VAL(A,COMMIT_REPLY_MEDIUM,nbytes)
  #define GASNETI_TRACE_COMMIT_REQUESTLONG(handler,source_addr,nbytes,dest_addr,numargs) \
     GASNETI_TRACE_EVENT_VAL(A,COMMIT_REQUEST_LONG,nbytes)
  #define GASNETI_TRACE_COMMIT_REPLYLONG(handler,source_addr,nbytes,dest_addr,numargs) \
     GASNETI_TRACE_EVENT_VAL(A,COMMIT_REPLY_LONG,nbytes)
#else
  #define GASNETI_TRACE_AMREQUESTSHORT(tm,dest,handler,flags,numargs)                                ((void)0)
  #define GASNETI_TRACE_AMREPLYSHORT(token,handler,flags,numargs)                                    ((void)0)
  #define GASNETI_TRACE_AMREQUESTMEDIUM(tm,dest,handler,source_addr,nbytes,flags,numargs)            ((void)0)
  #define GASNETI_TRACE_AMREPLYMEDIUM(token,handler,source_addr,nbytes,flags,numargs)                ((void)0)
  #define GASNETI_TRACE_AMREQUESTLONG(tm,dest,handler,source_addr,nbytes,dest_addr,flags,numargs)    ((void)0)
  #define GASNETI_TRACE_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,flags,numargs)        ((void)0)
  #define GASNETI_TRACE_PREP_REQUESTMEDIUM(tm,dest,cbuf,least_pl,most_pl,flags,numargs)              ((void)0)
  #define GASNETI_TRACE_PREP_REPLYMEDIUM(token,cbuf,least_pl,most_pl,flags,numargs)                  ((void)0)
  #define GASNETI_TRACE_PREP_REQUESTLONG(tm,dest,cbuf,least_pl,most_pl,dest_addr,flags,numargs)      ((void)0)
  #define GASNETI_TRACE_PREP_REPLYLONG(token,cbuf,least_pl,most_pl,dest_addr,flags,numargs)          ((void)0)
  #define GASNETI_TRACE_PREP_RETURN(name,sd)                                                         ((void)0)
  #define GASNETI_TRACE_COMMIT_REQUESTMEDIUM(handler,source_addr,nbytes,numargs)                     ((void)0)
  #define GASNETI_TRACE_COMMIT_REPLYMEDIUM(handler,source_addr,nbytes,numargs)                       ((void)0)
  #define GASNETI_TRACE_COMMIT_REQUESTLONG(handler,source_addr,nbytes,dest_addr,numargs)             ((void)0)
  #define GASNETI_TRACE_COMMIT_REPLYLONG(handler,source_addr,nbytes,dest_addr,numargs)               ((void)0)
#endif
/* ------------------------------------------------------------------------------------ */
/* AM Handler tracing */
#if GASNETI_STATS_OR_TRACE
  #define _GASNETI_TRACE_GATHERHANDLERARGS(numargs, arghandle)              \
    char _tgha_argstr[256];                                                 \
    do {                                                                    \
      char *_tgha_aptr = _tgha_argstr;                                      \
      size_t _tgha_aspace = sizeof(_tgha_argstr);                           \
      *_tgha_aptr = 0;                                                      \
      for (int _tgha_i=0;_tgha_i<(numargs);_tgha_i++) {                     \
        /* here we assume args are stored in an array named by arghandle */ \
        int _tgha_len = snprintf(_tgha_aptr,_tgha_aspace," 0x%08x",         \
                                 (int)((uint32_t*)(arghandle))[_tgha_i]);   \
        _tgha_aptr += _tgha_len; _tgha_aspace -= _tgha_len;                 \
      }                                                                     \
    } while(0)

  #define _GASNETI_TRACE_HANDLER(name, info, timask) do { \
      if ((timask) & GEX_TI_ENTRY) {                                                       \
        const gex_AM_Entry_t * const _th_hentry = info.gex_entry;                          \
        const void * const * _th_fnptr = (const void * const *)&(_th_hentry->gex_fnptr);   \
        const char *_th_hname = _th_hentry->gex_name ? _th_hentry->gex_name : "(none)";    \
        /* TODO-EX: print symbolc flags when available */                                  \
        GASNETI_TRACE_PRINTF(A,(#name": handler: name='%s' fnptr=%p flags=0x%x cdata=%p",  \
                                _th_hname, *_th_fnptr,                                     \
                                _th_hentry->gex_flags,_th_hentry->gex_cdata));             \
      }                                                                                    \
    } while(0)

  #define _GASNETI_TRACE_AMSHORT_HANDLER(name, handlerid, token, numargs, arghandle) do { \
    _GASNETI_TRACE_GATHERHANDLERARGS(numargs, arghandle);                                 \
    _GASNETI_STAT_EVENT(A,name);                                                          \
    gex_Token_Info_t _tah_info;                                                           \
    gex_TI_t _tah_timask = gex_Token_Info(token, &_tah_info, GEX_TI_SRCRANK|GEX_TI_EP|GEX_TI_ENTRY);\
    gex_Client_t _tah_client = _tah_info.gex_ep ? gex_EP_QueryClient(_tah_info.gex_ep) : NULL; \
    const char *_tah_cname = _tah_client ? gex_Client_QueryName(_tah_client) : "N/A";     \
    GASNETI_TRACE_PRINTF(A,(#name": client='%s' src=%i handler=%i args:%s",               \
      _tah_cname,(int)_tah_info.gex_srcrank,(int)(handlerid),_tgha_argstr));              \
    _GASNETI_TRACE_HANDLER(name, _tah_info, _tah_timask);                                 \
    GASNETI_TRACE_PRINTF(C,(#name": token: %s",                                           \
                      gasneti_formatdata((void*)&(token), sizeof(token))));               \
    } while(0)

  #define _GASNETI_TRACE_AMMEDLONG_HANDLER(name, handlerid, token, addr, nbytes, numargs, arghandle) do { \
    _GASNETI_TRACE_GATHERHANDLERARGS(numargs, arghandle);                                                 \
    _GASNETI_STAT_EVENT(A,name);                                                                          \
    gex_Token_Info_t _tah_info;                                                                           \
    gex_TI_t _tah_timask = gex_Token_Info(token, &_tah_info, GEX_TI_SRCRANK|GEX_TI_EP|GEX_TI_ENTRY);      \
    gex_Client_t _tah_client = _tah_info.gex_ep ? gex_EP_QueryClient(_tah_info.gex_ep) : NULL;            \
    const char *_tah_cname = _tah_client ? gex_Client_QueryName(_tah_client) : "N/A";                     \
    GASNETI_TRACE_PRINTF(A,(#name": client='%s' src=%i handler=%i addr=" GASNETI_LADDRFMT                 \
                            " nbytes=%" PRIuSZ " args:%s",                                                \
      _tah_cname,(int)_tah_info.gex_srcrank,(int)(handlerid),GASNETI_LADDRSTR(addr),                      \
      (size_t)nbytes,_tgha_argstr));                                                                      \
    _GASNETI_TRACE_HANDLER(name, _tah_info, _tah_timask);                                                 \
    GASNETI_TRACE_PRINTF(C,(#name": token: %s",                                                           \
                      gasneti_formatdata((void *)&(token), sizeof(token))));                              \
    GASNETI_TRACE_PRINTF(D,(#name": payload data: %s", gasneti_formatdata(addr,nbytes)));                 \
  } while(0)

  #define GASNETI_TRACE_AMSHORT_REQHANDLER(handlerid, token, numargs, arghandle) \
         _GASNETI_TRACE_AMSHORT_HANDLER(AMREQUEST_SHORT_HANDLER, handlerid, token, numargs, arghandle)
  #define GASNETI_TRACE_AMSHORT_REPHANDLER(handlerid, token, numargs, arghandle) \
         _GASNETI_TRACE_AMSHORT_HANDLER(AMREPLY_SHORT_HANDLER, handlerid, token, numargs, arghandle)

  #define GASNETI_TRACE_AMMEDIUM_REQHANDLER(handlerid, token, addr, nbytes, numargs, arghandle) \
         _GASNETI_TRACE_AMMEDLONG_HANDLER(AMREQUEST_MEDIUM_HANDLER, handlerid, token, addr, nbytes, numargs, arghandle)
  #define GASNETI_TRACE_AMMEDIUM_REPHANDLER(handlerid, token, addr, nbytes, numargs, arghandle) \
         _GASNETI_TRACE_AMMEDLONG_HANDLER(AMREPLY_MEDIUM_HANDLER, handlerid, token, addr, nbytes, numargs, arghandle)

  #define GASNETI_TRACE_AMLONG_REQHANDLER(handlerid, token, addr, nbytes, numargs, arghandle) \
         _GASNETI_TRACE_AMMEDLONG_HANDLER(AMREQUEST_LONG_HANDLER, handlerid, token, addr, nbytes, numargs, arghandle)
  #define GASNETI_TRACE_AMLONG_REPHANDLER(handlerid, token, addr, nbytes, numargs, arghandle) \
         _GASNETI_TRACE_AMMEDLONG_HANDLER(AMREPLY_LONG_HANDLER, handlerid, token, addr, nbytes, numargs, arghandle)
#else
  /* Note EMPTY triggers an icc optimizer bug (gasnet bug #1378) */
  #define GASNETI_TRACE_AMSHORT_REQHANDLER(handlerid, token, numargs, arghandle) \
          do {} while (0)
  #define GASNETI_TRACE_AMSHORT_REPHANDLER(handlerid, token, numargs, arghandle) \
          do {} while (0)
  #define GASNETI_TRACE_AMMEDIUM_REQHANDLER(handlerid, token, addr, nbytes, numargs, arghandle) \
          do {} while (0)
  #define GASNETI_TRACE_AMMEDIUM_REPHANDLER(handlerid, token, addr, nbytes, numargs, arghandle) \
          do {} while (0)
  #define GASNETI_TRACE_AMLONG_REQHANDLER(handlerid, token, addr, nbytes, numargs, arghandle) \
          do {} while (0)
  #define GASNETI_TRACE_AMLONG_REPHANDLER(handlerid, token, addr, nbytes, numargs, arghandle) \
          do {} while (0)
#endif
/* ------------------------------------------------------------------------------------ */
/* Vector, Indexed & Strided tracing */
#if GASNETI_STATS_OR_TRACE
  #define _GASNETI_TRACE_PUTVGETV(name,type,tm,rank,dstcount,dstlist,srccount,srclist) do {                 \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                         \
      char *_tmp_str = (char *)gasneti_extern_malloc(gasneti_format_putvgetv_bufsz((dstcount),(srccount))); \
      size_t _totalsz = gasneti_format_putvgetv(_tmp_str,(tm),(rank),(dstcount),(dstlist),(srccount),(srclist)); \
      GASNETI_TRACE_EVENT_VAL(type,name,_totalsz);                                                          \
      GASNETI_TRACE_PRINTF(D,(#name ": %s", _tmp_str));                                                     \
      gasneti_extern_free(_tmp_str);                                                                        \
    } else GASNETI_TRACE_EVENT_VAL(type,name, gasnete_memveclist_stats(dstcount,dstlist)._totalsz);         \
  } while (0)

  #define _GASNETI_TRACE_PUTIGETI(name,type,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen) do {   \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                         \
      char *_tmp_str = (char *)gasneti_extern_malloc(gasneti_format_putigeti_bufsz((dstcount),(srccount))); \
      size_t _totalsz = gasneti_format_putigeti(_tmp_str,(tm),(rank),(dstcount),(dstlist),(dstlen),              \
                                                (srccount),(srclist),(srclen));                             \
      GASNETI_TRACE_EVENT_VAL(type,name,_totalsz);                                                          \
      GASNETI_TRACE_PRINTF(D,(#name ": %s", _tmp_str));                                                     \
      gasneti_extern_free(_tmp_str);                                                                        \
    } else GASNETI_TRACE_EVENT_VAL(type,name, (dstcount)*(dstlen));                                         \
  } while (0)

  #define _GASNETI_TRACE_PUTSGETS(name,type,tm,rank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels) do { \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                             \
      char *_tmp_str = (char *)gasneti_extern_malloc(gasneti_format_putsgets_bufsz(stridelevels));              \
      size_t _totalsz = gasneti_format_putsgets(_tmp_str,NULL,(tm),(rank),(dstaddr),(dststrides),               \
                                                (srcaddr),(srcstrides),(elemsz),(count),(stridelevels));        \
      GASNETI_TRACE_EVENT_VAL(type,name,_totalsz);                                                              \
      GASNETI_TRACE_PRINTF(D,(#name ": %s", _tmp_str));                                                         \
      gasneti_extern_free(_tmp_str);                                                                            \
    } else GASNETI_TRACE_EVENT_VAL(type,name, gasnete_strided_datasize(elemsz,count,stridelevels));             \
  } while (0)

  #define GASNETI_TRACE_PUTV(name,tm,rank,dstcount,dstlist,srccount,srclist) \
     _GASNETI_TRACE_PUTVGETV(name,P,tm,rank,dstcount,dstlist,srccount,srclist)
  #define GASNETI_TRACE_GETV(name,tm,rank,dstcount,dstlist,srccount,srclist) \
     _GASNETI_TRACE_PUTVGETV(name,G,tm,rank,dstcount,dstlist,srccount,srclist)

  #define GASNETI_TRACE_PUTI(name,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen) \
     _GASNETI_TRACE_PUTIGETI(name,P,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen)
  #define GASNETI_TRACE_GETI(name,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen) \
     _GASNETI_TRACE_PUTIGETI(name,G,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen)

  #define GASNETI_TRACE_PUTS(name,tm,rank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels) \
     _GASNETI_TRACE_PUTSGETS(name,P,tm,rank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels)
  #define GASNETI_TRACE_GETS(name,tm,rank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels) \
     _GASNETI_TRACE_PUTSGETS(name,G,tm,rank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels)
#else
  #define GASNETI_TRACE_PUTV(name,tm,rank,dstcount,dstlist,srccount,srclist)
  #define GASNETI_TRACE_GETV(name,tm,rank,dstcount,dstlist,srccount,srclist)
  #define GASNETI_TRACE_PUTI(name,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen)
  #define GASNETI_TRACE_GETI(name,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen)
  #define GASNETI_TRACE_PUTS(name,tm,rank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels)
  #define GASNETI_TRACE_GETS(name,tm,rank,dstaddr,dststrides,srcaddr,srcstrides,elemsz,count,stridelevels)
#endif
/* ------------------------------------------------------------------------------------ */
/* Internal implementation of statistical/tracing output */

typedef uint64_t gasneti_statctr_t;
#define GASNETI_STATCTR_MIN ((gasneti_statctr_t)0)
#define GASNETI_STATCTR_MAX ((gasneti_statctr_t)-1)


#define GASNETI_TICKS_NOW_IFENABLED(type)                      \
  ((GASNETI_STATS_ENABLED(type) || GASNETI_TRACE_ENABLED(type)) ? \
   gasneti_ticks_now():(gasneti_tick_t)0)
typedef struct {
  gasneti_statctr_t _count;
  gasneti_statctr_t _minval;
  gasneti_statctr_t _maxval;
  gasneti_statctr_t _sumval;
} gasneti_stat_intval_t;

typedef struct {
  gasneti_statctr_t _count;
  gasneti_tick_t _minval;
  gasneti_tick_t _maxval;
  gasneti_tick_t _sumval;
} gasneti_stat_timeval_t;

/* startup & cleanup called by GASNet */
extern void gasneti_trace_init(int *_argc, char ***_argv);
extern void gasneti_trace_finish(void);

/* defines all the types */
#define GASNETI_ALLTYPES "GPRSWXBLAICDNH"


/* GASNETI_ALL_STATS lists all the statistics values we gather, 
   in the format: (type,name,value_description)
*/
#define GASNETI_ALL_STATS(CNT,VAL,TIME)                   \
        VAL(G, GET, sz)                                   \
        VAL(G, GET_BULK, sz)                              \
        VAL(G, GET_NB, sz)                                \
        VAL(G, GET_NB_BULK, sz)                           \
        VAL(G, GET_NB_VAL, sz)                            \
        VAL(G, GET_NBI, sz)                               \
        VAL(G, GET_NBI_BULK, sz)                          \
        VAL(G, GET_VAL, sz)                               \
                                                          \
        VAL(G, GET_LOCAL, sz)                             \
        VAL(G, GET_BULK_LOCAL, sz)                        \
        VAL(G, GET_NB_LOCAL, sz)                          \
        VAL(G, GET_NB_BULK_LOCAL, sz)                     \
        VAL(G, GET_NB_VAL_LOCAL, sz)                      \
        VAL(G, GET_NBI_LOCAL, sz)                         \
        VAL(G, GET_NBI_BULK_LOCAL, sz)                    \
        VAL(G, GET_VAL_LOCAL, sz)                         \
                                                          \
        VAL(P, PUT, sz)                                   \
        VAL(P, PUT_BULK, sz)                              \
        VAL(P, PUT_NB, sz)                                \
        VAL(P, PUT_NB_BULK, sz)                           \
        VAL(P, PUT_NB_VAL, sz)                            \
        VAL(P, PUT_NBI, sz)                               \
        VAL(P, PUT_NBI_BULK, sz)                          \
        VAL(P, PUT_NBI_VAL, sz)                           \
        VAL(P, PUT_VAL, sz)                               \
                                                          \
        VAL(P, PUT_LOCAL, sz)                             \
        VAL(P, PUT_BULK_LOCAL, sz)                        \
        VAL(P, PUT_NB_LOCAL, sz)                          \
        VAL(P, PUT_NB_BULK_LOCAL, sz)                     \
        VAL(P, PUT_NB_VAL_LOCAL, sz)                      \
        VAL(P, PUT_NBI_LOCAL, sz)                         \
        VAL(P, PUT_NBI_BULK_LOCAL, sz)                    \
        VAL(P, PUT_NBI_VAL_LOCAL, sz)                     \
        VAL(P, PUT_VAL_LOCAL, sz)                         \
                                                          \
        VAL(G, GETV_BULK, sz)                             \
        VAL(G, GETV_NB_BULK, sz)                          \
        VAL(G, GETV_NBI_BULK, sz)                         \
        VAL(G, GETI_BULK, sz)                             \
        VAL(G, GETI_NB_BULK, sz)                          \
        VAL(G, GETI_NBI_BULK, sz)                         \
        VAL(G, GETS_BULK, sz)                             \
        VAL(G, GETS_NB_BULK, sz)                          \
        VAL(G, GETS_NBI_BULK, sz)                         \
                                                          \
        VAL(P, PUTV_BULK, sz)                             \
        VAL(P, PUTV_NB_BULK, sz)                          \
        VAL(P, PUTV_NBI_BULK, sz)                         \
        VAL(P, PUTI_BULK, sz)                             \
        VAL(P, PUTI_NB_BULK, sz)                          \
        VAL(P, PUTI_NBI_BULK, sz)                         \
        VAL(P, PUTS_BULK, sz)                             \
        VAL(P, PUTS_NB_BULK, sz)                          \
        VAL(P, PUTS_NBI_BULK, sz)                         \
                                                          \
        CNT(R, RATOMIC_NB_I32, cnt)                       \
        CNT(R, RATOMIC_NB_U32, cnt)                       \
        CNT(R, RATOMIC_NB_I64, cnt)                       \
        CNT(R, RATOMIC_NB_U64, cnt)                       \
        CNT(R, RATOMIC_NB_FLT, cnt)                       \
        CNT(R, RATOMIC_NB_DBL, cnt)                       \
        CNT(R, RATOMIC_NBI_I32, cnt)                      \
        CNT(R, RATOMIC_NBI_U32, cnt)                      \
        CNT(R, RATOMIC_NBI_I64, cnt)                      \
        CNT(R, RATOMIC_NBI_U64, cnt)                      \
        CNT(R, RATOMIC_NBI_FLT, cnt)                      \
        CNT(R, RATOMIC_NBI_DBL, cnt)                      \
                                                          \
        /* TODO-EX: "SYNCNB" name is out-of-date */       \
        VAL(S, TEST_SYNCNB, success)                      \
        VAL(S, TEST_SYNCNB_ALL, success)                  \
        VAL(S, TEST_SYNCNB_SOME, success)                 \
        TIME(S, WAIT_SYNCNB, waittime)                    \
        TIME(S, WAIT_SYNCNB_ALL, waittime)                \
        TIME(S, WAIT_SYNCNB_SOME, waittime)               \
                                                          \
        VAL(S, TEST_SYNCNBI_ALL, success)                 \
        VAL(S, TEST_SYNCNBI_GETS, success)                \
        VAL(S, TEST_SYNCNBI_PUTS, success)                \
        VAL(S, TEST_SYNCNBI, success)                     \
        TIME(S, WAIT_SYNCNBI_ALL, waittime)               \
        TIME(S, WAIT_SYNCNBI_GETS, waittime)              \
        TIME(S, WAIT_SYNCNBI_PUTS, waittime)              \
        TIME(S, WAIT_SYNCNBI, waittime)                   \
                                                          \
        VAL(I, END_NBI_ACCESSREGION, numops)              \
                                                          \
        CNT(B, BARRIER_NOTIFY, cnt)                       \
        TIME(B, BARRIER_NOTIFYWAIT, notify-wait interval) \
        TIME(B, BARRIER_ASYNC_COMPLETION, notify-wait interval) \
        TIME(B, BARRIER_WAIT, waittime)                   \
        TIME(B, BARRIER, single-phase barrier time)       \
        VAL(B, BARRIER_TRY, success)                      \
                                                          \
        TIME(L, HSL_LOCK, waittime)                       \
        TIME(L, HSL_UNLOCK, holdtime)                     \
        VAL(L, HSL_TRYLOCK, success)                      \
                                                          \
        CNT(A, AMREQUEST_SHORT, cnt)                      \
        VAL(A, AMREQUEST_MEDIUM, sz)                      \
        VAL(A, AMREQUEST_LONG, sz)                        \
        CNT(A, AMREPLY_SHORT, cnt)                        \
        VAL(A, AMREPLY_MEDIUM, sz)                        \
        VAL(A, AMREPLY_LONG, sz)                          \
                                                          \
        VAL(A, PREP_REQUEST_MEDIUM, sz)                   \
        VAL(A, PREP_REQUEST_LONG, sz)                     \
        VAL(A, PREP_REPLY_MEDIUM, sz)                     \
        VAL(A, PREP_REPLY_LONG, sz)                       \
        VAL(A, COMMIT_REQUEST_MEDIUM, sz)                 \
        VAL(A, COMMIT_REQUEST_LONG, sz)                   \
        VAL(A, COMMIT_REPLY_MEDIUM, sz)                   \
        VAL(A, COMMIT_REPLY_LONG, sz)                     \
                                                          \
        CNT(A, AMREQUEST_SHORT_HANDLER, cnt)              \
        CNT(A, AMREQUEST_MEDIUM_HANDLER, cnt)             \
        CNT(A, AMREQUEST_LONG_HANDLER, cnt)               \
        CNT(A, AMREPLY_SHORT_HANDLER, cnt)                \
        CNT(A, AMREPLY_MEDIUM_HANDLER, cnt)               \
        CNT(A, AMREPLY_LONG_HANDLER, cnt)                 \
                                                          \
        CNT(I, AMPOLL, cnt)                               \
                                                          \
        VAL(I, GASNET_MALLOC, sz)                         \
        VAL(I, GASNET_FREE, sz)                           \
                                                          \
        GASNETC_CONDUIT_STATS(CNT,VAL,TIME)               \
        GASNETE_CONDUIT_STATS(CNT,VAL,TIME)               

/* GASNETC_CONDUIT_STATS and GASNETE_CONDUIT_STATS provide a way for conduits 
   to declare their own statistics (which should be given type C or I)
 */
#ifndef GASNETC_CONDUIT_STATS
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME)
#endif
#ifndef GASNETE_CONDUIT_STATS
#define GASNETE_CONDUIT_STATS(CNT,VAL,TIME)
#endif

/* these are legal even without STATS/TRACE */
extern size_t gasneti_format_memveclist_bufsz(size_t _count);
extern gasneti_memveclist_stats_t gasneti_format_memveclist(char *_buf, size_t _count, gex_Memvec_t const *_list);
extern size_t gasneti_format_putvgetv_bufsz(size_t _dstcount, size_t _srccount);
extern size_t gasneti_format_putvgetv(char *_buf, gex_TM_t _tm, gex_Rank_t _rank,
                                    size_t _dstcount, gex_Memvec_t const _dstlist[], 
                                    size_t _srccount, gex_Memvec_t const _srclist[]);

extern size_t gasneti_format_addrlist_bufsz(size_t _count);
extern gasneti_addrlist_stats_t gasneti_format_addrlist(char *_buf, size_t _count, void * const *_list, size_t _len);
extern size_t gasneti_format_putigeti_bufsz(size_t _dstcount, size_t _srccount);
extern size_t gasneti_format_putigeti(char *_buf, gex_TM_t _tm, gex_Rank_t _rank,
                                    size_t _dstcount, void * const _dstlist[], size_t _dstlen,
                                    size_t _srccount, void * const _srclist[], size_t _srclen);

extern size_t gasneti_format_strides_bufsz(size_t _count);
extern void gasneti_format_strides(char *_buf, size_t _count, const ptrdiff_t *_list);
extern size_t gasneti_format_putsgets_bufsz(size_t _stridelevels);
extern size_t gasneti_format_putsgets(char *_buf, void *_pstats, 
                                    gex_TM_t _tm, gex_Rank_t _rank,
                                    void *_dstaddr, const ptrdiff_t _dststrides[],
                                    void *_srcaddr, const ptrdiff_t _srcstrides[],
                                    size_t _elemsz, const size_t _count[], size_t _stridelevels);

extern const char *gasneti_formattm(gex_TM_t _tm);

// Prints a *set* of values to a caller-provided buffer.
// Call with NULL buffer returns required length.
extern size_t gasneti_format_dt(char *_buf, gex_DT_t _dt);
extern size_t gasneti_format_op(char *_buf, gex_OP_t _op);
extern size_t gasneti_format_ti(char *_buf, gex_TI_t _ti);

GASNETI_FORMAT_PRINTF(gasneti_dynsprintf,1,2,
extern char *gasneti_dynsprintf(const char *_format,...));

#if GASNETI_STATS_OR_TRACE
  #define _GASNETI_BUILD_ENUM(type,name,desc) GASNETI_STAT_##name,
  typedef enum {
    GASNETI_ALL_STATS(_GASNETI_BUILD_ENUM, _GASNETI_BUILD_ENUM, _GASNETI_BUILD_ENUM)
    GASNETI_STAT_COUNT
  } gasneti_statidx_t;
  #undef _GASNETI_BUILD_ENUM
  typedef struct {
    const char * const _type;
    const char * const _name;
    const char * const _desc;
  } gasneti_statinfo_t;
  extern gasneti_statinfo_t gasneti_stats[];

  extern FILE *gasneti_tracefile;
  extern FILE *gasneti_statsfile;
  extern char *gasneti_formatdata(void *_p, size_t _nbytes);
  extern void gasneti_trace_output(const char *_type, const char *_msg, int _traceheader);
  extern void gasneti_stats_output(const char *_type, const char *_msg, int _traceheader);

  extern char gasneti_tracetypes[];
  extern char gasneti_statstypes[];
  extern char gasneti_trace_maskstr[];
  extern char gasneti_stats_maskstr[];
  extern void gasneti_trace_updatemask(const char *_newmask, char *_maskstr, char *_types);

  extern int gasneti_trace_suppresslocal;
#endif
#if GASNET_TRACE
  #define GASNETI_TRACE_ENABLED(type) (gasneti_tracetypes[(int)*(char*)#type])
  #define GASNETI_TRACE_GETMASK()     ((const char *)gasneti_trace_maskstr)
  #define GASNETI_TRACE_SETMASK(mask) gasneti_trace_updatemask(mask, gasneti_trace_maskstr, gasneti_tracetypes)
  #define GASNETI_TRACE_GET_TRACELOCAL()        (!gasneti_trace_suppresslocal)
  #define GASNETI_TRACE_SET_TRACELOCAL(newval)  do {                                \
    int _val = (newval);                                                            \
    GASNETI_TRACE_PRINTF(I,("%s GASNET_TRACELOCAL",(_val?"Enabling":"Disabling"))); \
    gasneti_trace_suppresslocal = !_val;                                            \
  } while (0)
  #define GASNETI_MAYBE_TRACEFILE gasneti_tracefile
#else
  #define GASNETI_TRACE_ENABLED(type) 0
  #define GASNETI_MAYBE_TRACEFILE ((FILE *)NULL)
#endif
#if GASNET_STATS
  #define GASNETI_STATS_ENABLED(type) (gasneti_statstypes[(int)*(char*)#type])
  #define GASNETI_STATS_GETMASK()     ((const char *)gasneti_stats_maskstr)
  #define GASNETI_STATS_SETMASK(mask) gasneti_trace_updatemask(mask, gasneti_stats_maskstr, gasneti_statstypes)
#else
  #define GASNETI_STATS_ENABLED(type) 0
#endif


#if GASNET_TRACE
  #define _GASNETI_TRACE_EVENT(type, name) \
    GASNETI_TRACE_PRINTF(type, ("%s", #name))
  #define _GASNETI_TRACE_EVENT_VAL(type, name, val) \
    GASNETI_TRACE_PRINTF(type, ("%s: %s = %6" PRIu64,        \
        #name, gasneti_stats[(int)GASNETI_STAT_##name]._desc, (uint64_t)val))
  #define _GASNETI_TRACE_EVENT_TIME(type, name, time)        \
    GASNETI_TRACE_PRINTF(type, ("%s: %s = %6.3fus",            \
        #name, gasneti_stats[(int)GASNETI_STAT_##name]._desc, \
        gasneti_ticks_to_ns(time)/1000.0))
#else
  #define _GASNETI_TRACE_EVENT(type, name)             ((void)0)
  #define _GASNETI_TRACE_EVENT_VAL(type, name, val)    ((void)0)
  #define _GASNETI_TRACE_EVENT_TIME(type, name, time)  ((void)0)
#endif


#if GASNET_STATS
  #define _GASNETI_DECL_CTR(type,name,desc) \
    extern gasneti_statctr_t gasneti_stat_ctr_##name;
  #define _GASNETI_DECL_INTVAL(type,name,desc) \
    extern gasneti_stat_intval_t gasneti_stat_intval_##name;
  #define _GASNETI_DECL_TIMEVAL(type,name,desc) \
    extern gasneti_stat_timeval_t gasneti_stat_timeval_##name;
  GASNETI_ALL_STATS(_GASNETI_DECL_CTR, _GASNETI_DECL_INTVAL, _GASNETI_DECL_TIMEVAL)
  #undef _GASNETI_DECL_CTR
  #undef _GASNETI_DECL_INTVAL
  #undef _GASNETI_DECL_TIMEVAL

  extern void gasneti_stat_count_accumulate(gasneti_statctr_t *_pctr);
  extern void gasneti_stat_intval_accumulate(gasneti_stat_intval_t *_pintval, gasneti_statctr_t _val);
  extern void gasneti_stat_timeval_accumulate(gasneti_stat_timeval_t *_pintval, gasneti_tick_t _val);
  #define _GASNETI_STAT_EVENT(type, name) do {                 \
    if (GASNETI_STATS_ENABLED(type))                           \
      gasneti_stat_count_accumulate(&gasneti_stat_ctr_##name); \
  } while (0)
  #define _GASNETI_STAT_EVENT_VAL(type, name, val) do {                                  \
    if (GASNETI_STATS_ENABLED(type))                                                     \
      gasneti_stat_intval_accumulate(&gasneti_stat_intval_##name,(gasneti_statctr_t)val);\
  } while (0)
  #define _GASNETI_STAT_EVENT_TIME(type, name, time) do {                                  \
    if (GASNETI_STATS_ENABLED(type))                                                       \
      gasneti_stat_timeval_accumulate(&gasneti_stat_timeval_##name,(gasneti_tick_t)time);\
  } while (0)
#else
  #define _GASNETI_STAT_EVENT(type, name)            ((void)0)
  #define _GASNETI_STAT_EVENT_VAL(type, name, val)   ((void)0)
  #define _GASNETI_STAT_EVENT_TIME(type, name, time) ((void)0)
#endif
#define GASNETI_STAT_EVENT      _GASNETI_STAT_EVENT
#define GASNETI_STAT_EVENT_VAL  _GASNETI_STAT_EVENT_VAL
#define GASNETI_STAT_EVENT_TIME _GASNETI_STAT_EVENT_TIME

/* ------------------------------------------------------------------------------------ */

#endif
