/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_am.h $
 * Description: GASNet header for conduit-independent code for Active Messages
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2018, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_AM_H
#define _GASNET_AM_H

/* ------------------------------------------------------------------------------------ */
/* common error-checking code for AM request/reply entry points */

// TODO-EX: GASNETI_CHECK_ERRR should *not* be returning the error code - need error handling callback instead

#define GASNETI_COMMON_AMREQUESTSHORT(tm,rank,handler,flags,numargs) do {      \
    GASNETI_CHECKATTACH();                                                     \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_CLIENT));              \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_ALLOC));               \
    gasneti_assert_int(numargs ,>=, 0);                                        \
    gasneti_assert_int(numargs ,<=, gex_AM_MaxArgs());                         \
    GASNETI_TRACE_AMREQUESTSHORT(tm,rank,handler,flags,numargs);               \
    GASNETI_CHECK_ERRR((rank >= gasneti_nodes),BAD_ARG,"node index too high"); \
  } while (0)
#define GASNETI_COMMON_AMREQUESTMEDIUM(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs) do { \
    GASNETI_CHECKATTACH();                                                           \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_CLIENT));                    \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_ALLOC));                     \
    gasneti_assert_int(numargs ,>=, 0);                                        \
    gasneti_assert_int(numargs ,<=, gex_AM_MaxArgs());                         \
    GASNETI_TRACE_AMREQUESTMEDIUM(tm,rank,handler,source_addr,nbytes,flags,numargs); \
    GASNETI_CHECK_ERRR((rank >= gasneti_nodes),BAD_ARG,"node index too high");       \
    GASNETI_CHECK_ERRR((nbytes > gex_AM_MaxRequestMedium(tm,rank,lc_opt,flags,numargs)),\
                       BAD_ARG,"nbytes too large");                                  \
    GASNETI_CHECK_ERRR((lc_opt == NULL),BAD_ARG,"lc_opt=NULL is invalid");           \
    GASNETI_CHECK_ERRR((lc_opt == GEX_EVENT_DEFER),BAD_ARG,"EVENT_DEFER is invalid for Requests"); \
  } while (0)
#define GASNETI_COMMON_AMREQUESTLONG(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs) do { \
    GASNETI_CHECKATTACH();                                                                   \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_CLIENT));                            \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_ALLOC));                             \
    gasneti_assert_int(numargs ,>=, 0);                                                      \
    gasneti_assert_int(numargs ,<=, gex_AM_MaxArgs());                                       \
    GASNETI_TRACE_AMREQUESTLONG(tm,rank,handler,source_addr,nbytes,dest_addr,flags,numargs); \
    GASNETI_CHECK_ERRR((rank >= gasneti_nodes),BAD_ARG,"node index too high");               \
    GASNETI_CHECK_ERRR((nbytes > gex_AM_MaxRequestLong(tm,rank,lc_opt,flags,numargs)),       \
                       BAD_ARG,"nbytes too large");                                          \
    GASNETI_CHECK_ERRR((lc_opt == NULL),BAD_ARG,"lc_opt=NULL is invalid");                   \
    GASNETI_CHECK_ERRR((lc_opt == GEX_EVENT_DEFER),BAD_ARG,"EVENT_DEFER is invalid for Requests"); \
  } while (0)
#define GASNETI_COMMON_AMREPLYSHORT(token,handler,flags,numargs) do {    \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_CLIENT));  \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_ALLOC));   \
    gasneti_assert_int(numargs ,>=, 0);                            \
    gasneti_assert_int(numargs ,<=, gex_AM_MaxArgs());             \
    GASNETI_TRACE_AMREPLYSHORT(token,handler,flags,numargs);       \
  } while (0)
#define GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,lc_opt,flags,numargs) do { \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_CLIENT));                   \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_ALLOC));                    \
    gasneti_assert_int(numargs ,>=, 0);                                             \
    gasneti_assert_int(numargs ,<=, gex_AM_MaxArgs());                              \
    GASNETI_TRACE_AMREPLYMEDIUM(token,handler,source_addr,nbytes,flags,numargs);    \
    GASNETI_CHECK_ERRR((nbytes > gasnetc_Token_MaxReplyMedium(token,lc_opt,flags,numargs)),\
                       BAD_ARG,"nbytes too large");                                 \
    GASNETI_CHECK_ERRR((lc_opt == NULL),BAD_ARG,"lc_opt=NULL is invalid");          \
    GASNETI_CHECK_ERRR((lc_opt == GEX_EVENT_DEFER),BAD_ARG,"EVENT_DEFER is invalid for Replies"); \
    GASNETI_CHECK_ERRR((lc_opt == GEX_EVENT_GROUP),BAD_ARG,"EVENT_GROUP is invalid for Replies"); \
  } while (0)
#define GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs) do { \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_CLIENT));                           \
    gasneti_assert(! (flags & GEX_FLAG_AM_PREPARE_LEAST_ALLOC));                            \
    gasneti_assert_int(numargs ,>=, 0);                                                     \
    gasneti_assert_int(numargs ,<=, gex_AM_MaxArgs());                                      \
    GASNETI_TRACE_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,flags,numargs);    \
    GASNETI_CHECK_ERRR((nbytes > gasnetc_Token_MaxReplyLong(token,lc_opt,flags,numargs)),   \
                       BAD_ARG,"nbytes too large");                                         \
    GASNETI_CHECK_ERRR((lc_opt == NULL),BAD_ARG,"lc_opt=NULL is invalid");                  \
    GASNETI_CHECK_ERRR((lc_opt == GEX_EVENT_DEFER),BAD_ARG,"EVENT_DEFER is invalid for Replies"); \
    GASNETI_CHECK_ERRR((lc_opt == GEX_EVENT_GROUP),BAD_ARG,"EVENT_GROUP is invalid for Replies"); \
  } while (0)

/* ------------------------------------------------------------------------------------ */
/* utility macros for dispatching AM handlers */

#define _gasneti_harg(a,b,c) ,gex_AM_Arg_t
#define _gasneti_harg_pass(Nm1,N,Np1) ,_pArgs[Nm1]

#define _gasneti_Short_handlerfn_typedefN(Nm1,N,Np1) \
  typedef void (*gasneti_Short_handlerfn_type_##N)(gex_Token_t GASNETI_META_DES##N(GASNETI_META_EMPTY,_gasneti_harg));
GASNETI_META_ASC16(_gasneti_Short_handlerfn_typedefN,_gasneti_Short_handlerfn_typedefN)

#define _gasneti_Short_RunCaseN(Nm1,N,Np1) \
  case N: ((gasneti_Short_handlerfn_type_##N)_phandlerfn)(_token GASNETI_META_ASC##N(GASNETI_META_EMPTY,_gasneti_harg_pass)); break;

#define GASNETI_RUN_HANDLER_SHORT(isReq, hid, phandlerfn, token, pArgs, numargs) do { \
  gex_AM_Fn_t const _phandlerfn = (gex_AM_Fn_t)(phandlerfn);                    \
  gex_Token_t const _token = (token);                                           \
  gex_AM_Arg_t const * const _pArgs = (gex_AM_Arg_t *)(pArgs);                  \
  int const _numargs = (numargs);                                               \
  if (isReq) GASNETI_TRACE_AMSHORT_REQHANDLER(hid, _token, _numargs, _pArgs);   \
  else       GASNETI_TRACE_AMSHORT_REPHANDLER(hid, _token, _numargs, _pArgs);   \
  gasneti_assert(_phandlerfn); gasneti_assert(_token);                          \
  gasneti_assert(_pArgs || !_numargs);                                          \
  switch (_numargs) {                                                           \
    GASNETI_META_DES16(_gasneti_Short_RunCaseN,_gasneti_Short_RunCaseN)         \
    default: gasneti_unreachable_error(("Invalid numargs=%i",_numargs));        \
  }                                                                             \
  GASNETI_TRACE_PRINTF(A,("AM%s_SHORT_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
} while (0)

#define _gasneti_MedLong_handlerfn_typedefN(Nm1,N,Np1) \
  typedef void (*gasneti_MedLong_handlerfn_type_##N)(gex_Token_t, void *, size_t GASNETI_META_DES##N(GASNETI_META_EMPTY,_gasneti_harg));
GASNETI_META_ASC16(_gasneti_MedLong_handlerfn_typedefN,_gasneti_MedLong_handlerfn_typedefN)

#define _gasneti_MedLong_RunCaseN(Nm1,N,Np1) \
  case N: ((gasneti_MedLong_handlerfn_type_##N)_phandlerfn)(_token, _pData, _datalen GASNETI_META_ASC##N(GASNETI_META_EMPTY,_gasneti_harg_pass)); break;

#define _GASNETI_RUN_HANDLER_MEDLONG(MEDLONG, isReq, hid, phandlerfn, token, pArgs, numargs, pData, datalen, extrachecks) do { \
  gex_AM_Fn_t const _phandlerfn = (gex_AM_Fn_t)(phandlerfn);                    \
  gex_Token_t const _token = (token);                                           \
  gex_AM_Arg_t const * const _pArgs = (gex_AM_Arg_t *)(pArgs);                  \
  int const _numargs = (numargs);                                               \
  void * const _pData = (void *)(pData);                                        \
  size_t const _datalen = (datalen);                                            \
  if (isReq) GASNETI_TRACE_AM##MEDLONG##_REQHANDLER(hid, _token, _pData, _datalen, _numargs, _pArgs); \
  else       GASNETI_TRACE_AM##MEDLONG##_REPHANDLER(hid, _token, _pData, _datalen, _numargs, _pArgs); \
  gasneti_assert(_phandlerfn); gasneti_assert(_token);                          \
  gasneti_assert(_pArgs || !_numargs);                                          \
  gasneti_assert(_pData || !_datalen);                                          \
  extrachecks;                                                                  \
  switch (_numargs) {                                                           \
    GASNETI_META_DES16(_gasneti_MedLong_RunCaseN,_gasneti_MedLong_RunCaseN)     \
    default: gasneti_unreachable_error(("Invalid numargs=%i",_numargs));        \
  }                                                                             \
} while (0)

// by default, we guarantee double-word alignment for data payload of medium xfers
#ifndef GASNETI_MEDBUF_ALIGNMENT
#define GASNETI_MEDBUF_ALIGNMENT 8
#endif

#define GASNETI_RUN_HANDLER_MEDIUM(isReq, hid, phandlerfn, token, pArgs, numargs, pData, datalen) do {    \
  _GASNETI_RUN_HANDLER_MEDLONG(MEDIUM, isReq, hid, phandlerfn, token, pArgs, numargs, pData, datalen,     \
               if (_datalen) gasneti_assert_uint(((uintptr_t)_pData) % GASNETI_MEDBUF_ALIGNMENT ,==, 0)); \
  GASNETI_TRACE_PRINTF(A,("AM%s_MEDIUM_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
} while (0)

#define GASNETI_RUN_HANDLER_LONG(isReq, hid, phandlerfn, token, pArgs, numargs, pData, datalen) do {      \
  _GASNETI_RUN_HANDLER_MEDLONG(LONG, isReq, hid, phandlerfn, token, pArgs, numargs, pData, datalen,       \
                     ((void)0));                                                                          \
  GASNETI_TRACE_PRINTF(A,("AM%s_LONG_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY")));   \
} while (0)

/* ------------------------------------------------------------------------------------ */
/* AM handler registration and management */

typedef enum {
  gasneti_Short=0,
  gasneti_Medium=1,
  gasneti_Long=2
} gasneti_category_t;

/* default AM handler for unregistered entries - prints a fatal error */
GASNETI_COLD
extern void gasneti_defaultAMHandler(gex_Token_t token);

extern int gasneti_amtbl_init(gex_AM_Entry_t *output);
extern int gasneti_amregister( gex_AM_Entry_t *output,
                               gex_AM_Entry_t *input, int numentries,
                               int lowlimit, int highlimit,
                               int dontcare, int *numregistered);
extern int gasneti_amregister_client(gex_AM_Entry_t *output,
                                     gex_AM_Entry_t *input, size_t numentries);
extern int gasneti_amregister_legacy(gex_AM_Entry_t *output,
                                     gasnet_handlerentry_t *input, int numentries);

#if GASNET_DEBUG
  extern void gasneti_amtbl_check(const gex_AM_Entry_t *entry, int nargs,
                                  gasneti_category_t category, int isReq);
#else
  #define gasneti_amtbl_check(entry, nargs, category, isReq) ((void)0)
#endif

// AM "catch all" for defaultAMHandler and similar
#define GASNETI_FLAG_AM_ANY \
     ( GEX_FLAG_AM_SHORT|GEX_FLAG_AM_MEDIUM|GEX_FLAG_AM_LONG | \
       GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_REPLY )

/* ------------------------------------------------------------------------------------ */
/* common logic for gex_Token_Info() */

// OR of all the required bits
#define GASNETI_TI_REQUIRED (GEX_TI_SRCRANK | GEX_TI_EP)

#if GASNET_DEBUG
  extern gex_TI_t gasneti_token_info_return(gex_TI_t result, gex_Token_Info_t * info, gex_TI_t mask);
  #define GASNETI_TOKEN_INFO_RETURN gasneti_token_info_return
#else
  #define GASNETI_TOKEN_INFO_RETURN(result, info, mask) (result)
#endif

/* ------------------------------------------------------------------------------------ */
/* common logic for Negotiated Payload AMs */

// Common argument processing and trace/stats
#if GASNET_DEBUG
  extern void gasneti_init_sd_poison(gasneti_AM_SrcDesc_t sd);
  extern int gasneti_test_sd_poison(void *addr, size_t len);

  #define _GASNETI_CHECK_PREPARE(cbuf, least_payload, most_payload, limit, lc_opt, nargs, is_req, cat) \
    do {                                                                                                 \
      const char *_reqrep = is_req ? "Request" : "Reply";                                                \
      if (cbuf == NULL) {                                                                                \
        if (lc_opt != NULL)                                                                              \
          gasneti_fatalerror("gex_AM_Prepare%s" _STRINGIFY(cat) ": "                                     \
                             "only NULL is a valid lc_opt value when client_buf is NULL", _reqrep);      \
      } else if (lc_opt == NULL) {                                                                       \
        gasneti_fatalerror("gex_AM_Prepare%s" _STRINGIFY(cat) ": lc_opt must be non-NULL "               \
                           "when client_buf is non-NULL", _reqrep);                                      \
      } else if (is_req) {                                                                               \
        if (!gasneti_leaf_is_pointer(lc_opt) && (lc_opt != GEX_EVENT_NOW) && (lc_opt != GEX_EVENT_GROUP))\
          gasneti_fatalerror("gex_AM_Prepare%s" _STRINGIFY(cat) ": only pointer-to-event, "              \
                             "GEX_EVENT_NOW and GEX_EVENT_GROUP are valid lc_opt values "                \
                             "when client_buf is non-NULL", _reqrep);                                    \
      } else {                                                                                           \
        if (!gasneti_leaf_is_pointer(lc_opt) && (lc_opt != GEX_EVENT_NOW))                               \
          gasneti_fatalerror("gex_AM_Prepare%s" _STRINGIFY(cat) ": only pointer-to-event, "              \
                             "and GEX_EVENT_NOW are valid lc_opt values "                                \
                             "when client_buf is non-NULL", _reqrep);                                    \
      }                                                                                                  \
      if (lc_opt && gasneti_leaf_is_pointer(lc_opt)) {                                                   \
        *lc_opt = GEX_EVENT_NO_OP;                                                                       \
      }                                                                                                  \
      if (nargs > gex_AM_MaxArgs())                                                                      \
        gasneti_fatalerror("gex_AM_Prepare%s" _STRINGIFY(cat) ": "                                       \
                           "numargs larger than gex_AM_MaxArgs() (%u > %u)",                             \
                           _reqrep, (unsigned int)nargs, (unsigned int)gex_AM_MaxArgs());                \
      if (least_payload > most_payload)                                                                  \
        gasneti_fatalerror("gex_AM_Prepare%s" _STRINGIFY(cat) ": "                                       \
                           "least_payload larger than most_payload (%"PRIuPTR" > %"PRIuPTR")",           \
                           _reqrep, (uintptr_t)least_payload, (uintptr_t)most_payload);                  \
      if (least_payload > limit)                                                                         \
        gasneti_fatalerror("gex_AM_Prepare%s" _STRINGIFY(cat) ": least_payload larger than gex_AM_Max%s" \
                           _STRINGIFY(cat) "() (%"PRIuPTR" > %"PRIuPTR")",                               \
                           _reqrep, _reqrep, (uintptr_t)least_payload, (uintptr_t)limit);                \
    } while(0)
  #define GASNETI_COMMON_PREP_REQ(sd,tm,dest,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs,cat) \
    do {                                                                                   \
      GASNETI_TRACE_PREP_REQUEST##cat(tm,dest,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs);\
      sd->_category  = (int)gasneti_##cat;                                                 \
      sd->_dest_addr = dest_addr;                                                          \
      sd->_nargs     = nargs;                                                              \
      gex_Flags_t tmp_flags = flags | (cbuf ? GEX_FLAG_AM_PREPARE_LEAST_CLIENT             \
                                            : GEX_FLAG_AM_PREPARE_LEAST_ALLOC);            \
      size_t limit = gex_AM_MaxRequest##cat(tm,dest,lc_opt,tmp_flags,nargs);               \
      if (dest >= gex_TM_QuerySize(tm))                                                    \
        gasneti_fatalerror("gex_AM_PrepareRequest" _STRINGIFY(cat) ": "                    \
                           "destination rank out-of-range (%lu >= %lu)",                   \
                           (unsigned long)dest, (unsigned long)gex_TM_QuerySize(tm));      \
      _GASNETI_CHECK_PREPARE(cbuf,least_pl,most_pl,limit,lc_opt,nargs,1,cat);              \
    } while(0)
  #define GASNETI_COMMON_PREP_REP(sd,token,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs,cat) \
    do {                                                                               \
      GASNETI_TRACE_PREP_REPLY##cat(token,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs);\
      sd->_category  = (int)gasneti_##cat;                                             \
      sd->_dest_addr = dest_addr;                                                      \
      sd->_nargs     = nargs;                                                          \
      gex_Flags_t tmp_flags = flags | (cbuf ? GEX_FLAG_AM_PREPARE_LEAST_CLIENT         \
                                            : GEX_FLAG_AM_PREPARE_LEAST_ALLOC);        \
      size_t limit = gasnetc_Token_MaxReply##cat(token,lc_opt,tmp_flags,nargs);        \
      _GASNETI_CHECK_PREPARE(cbuf,least_pl,most_pl,limit,lc_opt,nargs,0,cat);          \
    } while(0)

  #define _GASNETI_CHECK_COMMIT(sd,handler,nbytes,dest_addr,nargs,is_req,cat) \
    do {                                                                                                 \
      const char *_reqrep = is_req ? "Request" : "Reply";                                                \
      if (!sd)                                                                                           \
        gasneti_fatalerror("gex_AM_Commit%s" _STRINGIFY(cat) "%d: "                                      \
                           "passed invalid gex_AM_SrcDesc (GEX_AM_SRCDESC_NO_OP == 0)", _reqrep, nargs); \
      if (sd->_thread != _gasneti_mythread_slow())                                                       \
        gasneti_fatalerror("gex_AM_Commit%s" _STRINGIFY(cat) "%d: "                                      \
                           "return from Prepare passed to Commit in a different thread", _reqrep, nargs);\
      if (sd->_isreq != is_req)                                                                          \
        gasneti_fatalerror("gex_AM_Commit%s" _STRINGIFY(cat) "%d: "                                      \
                           "paired with incompatible Prepare (%s)",                                      \
                           _reqrep, nargs, (sd->_isreq?"Request":"Reply"));                              \
      if (sd->_category != (int)gasneti_##cat)                                                           \
        gasneti_fatalerror("gex_AM_Commit%s" _STRINGIFY(cat) "%d: "                                      \
                           "paired with incompatible Prepare (%s)",                                      \
                           _reqrep, nargs, (sd->_category==(int)gasneti_Long?"Long":"Medium"));          \
      if (sd->_nargs != nargs)                                                                           \
        gasneti_fatalerror("gex_AM_Commit%s" _STRINGIFY(cat) "%d: "                                      \
                           "paired with incompatible Prepare (nargs = %d)",                              \
                           _reqrep, nargs, sd->_nargs);                                                  \
      if (sd->_size < nbytes)                                                                            \
        gasneti_fatalerror("gex_AM_Commit%s" _STRINGIFY(cat) "%d: "                                      \
                           "nbytes larger than returned from Prepare (%"PRIuPTR" > %"PRIuPTR")",         \
                           _reqrep, nargs, (uintptr_t)nbytes, (uintptr_t)sd->_size);                     \
      if ((sd->_dest_addr != NULL) &&                                                                    \
          !((dest_addr == sd->_dest_addr) || ((dest_addr == NULL) && (nbytes == 0))))                    \
        gasneti_fatalerror("gex_AM_Commit%s" _STRINGIFY(cat) "%d: "                                      \
                           "dest_addr does not match the value passed to Prepare", _reqrep, nargs);      \
      if (sd->_gex_buf) {                                                                                \
        if (gasneti_test_sd_poison(sd->_gex_buf, nbytes))                                                \
          gasneti_fatalerror("gex_AM_Commit%s" _STRINGIFY(cat) "%d: "                                    \
                             "client did not write to the GASNet-provided buffer",                       \
                             _reqrep, nargs);                                                            \
      }                                                                                                  \
    } while(0)
  #define GASNETI_COMMON_COMMIT_REQ(sd,handler,nbytes,dest_addr,nargs,cat) do {            \
      GASNETI_TRACE_COMMIT_REQUEST##cat(handler,sd->_addr,sd->_size,dest_addr,sd->_nargs); \
      _GASNETI_CHECK_COMMIT(sd,handler,nbytes,dest_addr,nargs,1,cat);                      \
    } while(0)
  #define GASNETI_COMMON_COMMIT_REP(sd,handler,nbytes,dest_addr,nargs,cat) do {            \
      GASNETI_TRACE_COMMIT_REPLY##cat(handler,sd->_addr,sd->_size,dest_addr,sd->_nargs);   \
      _GASNETI_CHECK_COMMIT(sd,handler,nbytes,dest_addr,nargs,0,cat);                      \
    } while(0)
#else
  #define gasneti_init_sd_poison(sd) ((void)0)

  #if GASNET_TRACE // Need to store nargs from Prep to Commit to trace the args
    #define _GASNETI_COMMON_PREP_NARGS(sd,nargs) ((void)(sd->_nargs = nargs))
  #else
    #define _GASNETI_COMMON_PREP_NARGS(sd,nargs) ((void)0)
  #endif
  #define GASNETI_COMMON_PREP_REQ(sd,tm,dest,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs,cat) do { \
      _GASNETI_COMMON_PREP_NARGS(sd,nargs);                                                        \
      GASNETI_TRACE_PREP_REQUEST##cat(tm,dest,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs); \
    } while(0)
  #define GASNETI_COMMON_PREP_REP(sd,token,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs,cat) do { \
      _GASNETI_COMMON_PREP_NARGS(sd,nargs);                                                    \
      GASNETI_TRACE_PREP_REPLY##cat(token,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs); \
    } while(0)
  #define GASNETI_COMMON_COMMIT_REQ(sd,handler,nbytes,dest_addr,nargs_arg,cat) \
          GASNETI_TRACE_COMMIT_REQUEST##cat(handler,sd->_addr,sd->_size,dest_addr,sd->_nargs)
  #define GASNETI_COMMON_COMMIT_REP(sd,handler,nbytes,dest_addr,nargs_arg,cat) \
          GASNETI_TRACE_COMMIT_REPLY##cat(handler,sd->_addr,sd->_size,dest_addr,sd->_nargs)
#endif

#define GASNETI_TRACE_PREP_REQUESTMedium(tm,dest,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs) \
        GASNETI_TRACE_PREP_REQUESTMEDIUM(tm,dest,cbuf,least_pl,most_pl,flags,nargs)
#define GASNETI_TRACE_PREP_REQUESTLong(tm,dest,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs) \
        GASNETI_TRACE_PREP_REQUESTLONG(tm,dest,cbuf,least_pl,most_pl,dest_addr,flags,nargs)
#define GASNETI_TRACE_PREP_REPLYMedium(token,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs) \
        GASNETI_TRACE_PREP_REPLYMEDIUM(token,cbuf,least_pl,most_pl,flags,nargs)
#define GASNETI_TRACE_PREP_REPLYLong(token,cbuf,least_pl,most_pl,dest_addr,lc_opt,flags,nargs) \
        GASNETI_TRACE_PREP_REPLYLONG(token,cbuf,least_pl,most_pl,dest_addr,flags,nargs)

#define GASNETI_TRACE_COMMIT_REQUESTMedium(handler,source_addr,nbytes,dest_addr,numargs) \
        GASNETI_TRACE_COMMIT_REQUESTMEDIUM(handler,source_addr,nbytes,numargs)
#define GASNETI_TRACE_COMMIT_REQUESTLong(handler,source_addr,nbytes,dest_addr,numargs) \
        GASNETI_TRACE_COMMIT_REQUESTLONG(handler,source_addr,nbytes,dest_addr,numargs)
#define GASNETI_TRACE_COMMIT_REPLYMedium(handler,source_addr,nbytes,dest_addr,numargs) \
        GASNETI_TRACE_COMMIT_REPLYMEDIUM(handler,source_addr,nbytes,numargs)
#define GASNETI_TRACE_COMMIT_REPLYLong(handler,source_addr,nbytes,dest_addr,numargs) \
        GASNETI_TRACE_COMMIT_REPLYLONG(handler,source_addr,nbytes,dest_addr,numargs)

#ifndef _GEX_AM_SRCDESC_T
// Allocate a buffer (use IFF client_buf is NULL)
GASNETI_INLINE(gasneti_prepare_alloc_buffer)
void *gasneti_prepare_alloc_buffer(gasneti_AM_SrcDesc_t sd)
{
    size_t size = sd->_size;
#if GASNET_DEBUG
    // Allocate at least one byte because zero-byte allocation
    // returns NULL which then leads to ambiguity in argument checking.
    if (!size) size = 1;
#endif
    return (sd->_gex_buf = sd->_addr = gasneti_malloc(size));
}

#if GASNETI_NEED_INIT_SRCDESC
void gasneti_init_srcdesc(GASNETI_THREAD_FARG_ALONE);
#endif

// Get the thread-specfic SD for Requests
// Initializing on first call if necessary (DEBUG or when using THREADINFO_OPT)
GASNETI_INLINE(gasneti_init_request_srcdesc)
gasneti_AM_SrcDesc_t gasneti_init_request_srcdesc(GASNETI_THREAD_FARG_ALONE)
{
#if GASNETI_NEED_INIT_SRCDESC
  if_pf (! GASNETI_MYTHREAD->sd_is_init) gasneti_init_srcdesc(GASNETI_THREAD_PASS_ALONE);
#endif
  gasneti_AM_SrcDesc_t sd = &GASNETI_MYTHREAD->request_sd;
#if GASNET_DEBUG
  if (sd->_magic._u == GASNETI_AM_SRCDESC_MAGIC) {
    gasneti_fatalerror("Bad state - likely due to back-to-back gex_AM_PrepareRequest*() calls");
  }
  GASNETI_CHECK_MAGIC(sd, GASNETI_AM_SRCDESC_BAD_MAGIC);
  GASNETI_INIT_MAGIC(sd, GASNETI_AM_SRCDESC_MAGIC);
#endif
  sd->_gex_buf = NULL;
  return sd;
}

// Get the thread-specfic SD for Replies
// Initializing on first call if necessary (DEBUG or when using THREADINFO_OPT)
GASNETI_INLINE(gasneti_init_reply_srcdesc)
gasneti_AM_SrcDesc_t gasneti_init_reply_srcdesc(GASNETI_THREAD_FARG_ALONE)
{
#if GASNETI_NEED_INIT_SRCDESC
  if_pf (! GASNETI_MYTHREAD->sd_is_init) gasneti_init_srcdesc(GASNETI_THREAD_PASS_ALONE);
#endif
  gasneti_AM_SrcDesc_t sd = &GASNETI_MYTHREAD->reply_sd;
#if GASNET_DEBUG
  if (sd->_magic._u == GASNETI_AM_SRCDESC_MAGIC) {
    gasneti_fatalerror("Bad state - likely due to back-to-back gex_AM_PrepareReply*() calls");
  }
  GASNETI_CHECK_MAGIC(sd, GASNETI_AM_SRCDESC_BAD_MAGIC);
  GASNETI_INIT_MAGIC(sd, GASNETI_AM_SRCDESC_MAGIC);
#endif
  sd->_gex_buf = NULL;
  return sd;
}

// Return a thread-specfic SD to its "inactive" state
GASNETI_INLINE(gasneti_reset_srcdesc)
void gasneti_reset_srcdesc(gasneti_AM_SrcDesc_t sd)
{
#if GASNET_DEBUG
  if (sd->_magic._u == GASNETI_AM_SRCDESC_BAD_MAGIC) {
    gasneti_fatalerror("Bad state - likely due to back-to-back gex_AM_Commit%s*() calls",
                       sd->_isreq ? "Request" : "Reply");
  }
  GASNETI_CHECK_MAGIC(sd, GASNETI_AM_SRCDESC_MAGIC);
  GASNETI_INIT_MAGIC(sd, GASNETI_AM_SRCDESC_BAD_MAGIC);
#endif
}

GASNETI_INLINE(gasneti_prepare_common) GASNETI_WARN_UNUSED_RESULT
void *gasneti_prepare_common(
                       gasneti_AM_SrcDesc_t sd,
                       const void          *client_buf,
                       size_t               size,
                       gex_Event_t         *lc_opt,
                       gex_Flags_t          flags,
                       unsigned int         nargs)
{
    sd->_is_nbrhd = 0;
    sd->_lc_opt = lc_opt;
    sd->_flags  = flags;
    sd->_nargs  = nargs;
    sd->_size   = size;
    if (client_buf) {
        sd->_addr = (/*non-const*/void *)client_buf;
        return NULL;
    } else {
        return gasneti_prepare_alloc_buffer(sd);
    }
}

GASNETI_INLINE(gasneti_prepare_request_common) GASNETI_WARN_UNUSED_RESULT
void *gasneti_prepare_request_common(
                       gasneti_AM_SrcDesc_t sd,
                       gex_TM_t             tm,
                       gex_Rank_t           rank,
                       const void          *client_buf,
                       size_t               size,
                       gex_Event_t         *lc_opt,
                       gex_Flags_t          flags,
                       unsigned int         nargs)
{
    sd->_dest._request._tm   = tm;
    sd->_dest._request._rank = rank;
    return gasneti_prepare_common(sd, client_buf, size, lc_opt, flags, nargs);
}

GASNETI_INLINE(gasneti_prepare_reply_common) GASNETI_WARN_UNUSED_RESULT
void *gasneti_prepare_reply_common(
                       gasneti_AM_SrcDesc_t sd,
                       gex_Token_t          token,
                       const void          *client_buf,
                       size_t               size,
                       gex_Event_t         *lc_opt,
                       gex_Flags_t          flags,
                       unsigned int         nargs)
{
    sd->_dest._reply._token = token;
    return gasneti_prepare_common(sd, client_buf, size, lc_opt, flags, nargs);
}
#endif // _GEX_AM_SRCDESC_T

/*
  INTERNAL Active Message Request/Reply w/ va_list
  ================================================
  NOTE:
    These functions do not TRACE or perform complete argument validation.
    These Request functions do not AMPoll.
    Those are the caller's responsibility.
*/

extern int gasnetc_AMRequestMediumV(
                gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                void *source_addr, size_t nbytes,
                gex_Event_t *lc_opt, gex_Flags_t flags,
                int numargs, va_list argptr GASNETI_THREAD_FARG);
#ifndef gasneti_AMRequestMediumV
#define gasneti_AMRequestMediumV(tm,rank,hidx,src_addr,nbytes,lc_opt,flags,nargs,args) \
        gasnetc_AMRequestMediumV(tm,rank,hidx,src_addr,nbytes,lc_opt,flags,nargs,args GASNETI_THREAD_GET)
#endif
extern int gasnetc_AMRequestLongV(
                gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                void *source_addr, size_t nbytes, void *dest_addr,
                gex_Event_t *lc_opt, gex_Flags_t flags,
                int numargs, va_list argptr GASNETI_THREAD_FARG);
#ifndef gasneti_AMRequestLongV
#define gasneti_AMRequestLongV(tm,rank,hidx,src_addr,nbytes,dst_addr,lc_opt,flags,nargs,args) \
        gasnetc_AMRequestLongV(tm,rank,hidx,src_addr,nbytes,dst_addr,lc_opt,flags,nargs,args GASNETI_THREAD_GET)
#endif

extern int gasnetc_AMReplyMediumV(
                gex_Token_t token, gex_AM_Index_t handler,
                void *source_addr, size_t nbytes,
                gex_Event_t *lc_opt, gex_Flags_t flags,
                int numargs, va_list argptr);
#ifndef gasneti_AMReplyMediumV
#define gasneti_AMReplyMediumV(token,hidx,src_addr,nbytes,lc_opt,flags,nargs,args) \
        gasnetc_AMReplyMediumV(token,hidx,src_addr,nbytes,lc_opt,flags,nargs,args)
#endif
extern int gasnetc_AMReplyLongV(
                gex_Token_t token, gex_AM_Index_t handler,
                void *source_addr, size_t nbytes, void *dest_addr,
                gex_Event_t *lc_opt, gex_Flags_t flags,
                int numargs, va_list argptr);
#ifndef gasneti_AMReplyLongV
#define gasneti_AMReplyLongV(token,hidx,src_addr,nbytes,dst_addr,lc_opt,flags,nargs,args) \
        gasnetc_AMReplyLongV(token,hidx,src_addr,nbytes,dst_addr,lc_opt,flags,nargs,args)
#endif

/* ------------------------------------------------------------------------------------ */

// GASNETC_MAX_{ARGS,MEDIUM,LONG}_NBRHD
// These are compile-time constants used by the "neighborhood" AM support,
// which includes "loopback" (same-process) and "AMPSHM" (shared-memory).
// As described below, these defaults are not suitable for all conduits.

#ifndef GASNETC_MAX_ARGS_NBRHD
  // Assumes gex_AM_MaxArgs() is a compile time constant.
  // If not, the conduit must define GASNETC_MAX_ARGS_NBRHD to a compile-time
  // constant in its gasnet_core_fwd.h.
  // The value may be a conservative upper-bound if the real value cannot be
  // known until run time (at the cost of wasted memory).
  #define GASNETC_MAX_ARGS_NBRHD   (gex_AM_MaxArgs())
#endif
#ifndef GASNETC_MAX_MEDIUM_NBRHD
  // Assumes gex_AM_LUB{Request,Reply}Medium() expand to compile-time constants
  // AND that the LUB is the *greatest* upper-bound.  If either property is not
  // true for a given conduit, then it must define GASNETC_MAX_MEDIUM_NBRHD to
  // an appropriate compile-time constant bound in its gasnet_core_fwd.h.
  // The value may be a conservative upper-bound if the real value cannot be
  // known until run time (at the cost of wasted memory).
  #define GASNETC_MAX_MEDIUM_NBRHD MAX(gex_AM_LUBRequestMedium(),gex_AM_LUBReplyMedium())
#endif
#ifndef GASNETC_MAX_LONG_NBRHD
  // Same assumptions and usage as GASNETC_MAX_MEDIUM_NBRHD, above, but for Long.
  #define GASNETC_MAX_LONG_NBRHD MAX(gex_AM_LUBRequestLong(),gex_AM_LUBReplyLong())
#endif

// GASNETC_GET_HANDLER provides a conduit with the means to control how the
// neighborhood AM support accesses the AM handler table.
#ifndef GASNETC_GET_HANDLER
  /* Assumes conduit has gasnetc_handler[] as in template-conduit */
  // TODO-EX: gasnetc_handler to be replaced w/ per-endpoint data when defined
  #define gasnetc_get_hentry(_ep,_index) (&gasnetc_handler[(_index)])
  #define gasnetc_get_handler(_ep,_index,_field) (gasnetc_get_hentry((_ep),(_index))->gex_##_field)
#endif

/* ------------------------------------------------------------------------------------ */
// Buffer management for loopback (same-process) Medium AMs

#include <gasnet_core_internal.h> /* for gasnetc_handler[] */

#ifndef gasneti_loopback_alloc_medium_buffer // allows conduit-specifc overrides
    extern void gasneti_loopback_cleanup_threaddata(void *buf);
    GASNETI_INLINE(gasneti_loopback_alloc_medium_buffer)
    void *gasneti_loopback_alloc_medium_buffer(int isReq GASNETI_THREAD_FARG) {
        gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
        if_pf (! mythread->loopback_requestBuf) {
            // Allocate both buffers, ensuring GASNETI_MEDBUF_ALIGNMENT (dflt 8-byte) alignment of each
            size_t sz = GASNETI_ALIGNUP(GASNETC_MAX_MEDIUM_NBRHD,8) + GASNETC_MAX_MEDIUM_NBRHD;
            uint8_t *buf = gasneti_malloc_aligned(GASNETI_MEDBUF_ALIGNMENT, sz);
            gasneti_leak_aligned(buf);
            mythread->loopback_requestBuf = buf;
            mythread->loopback_replyBuf =   buf + GASNETI_ALIGNUP(GASNETC_MAX_MEDIUM_NBRHD,8);
            gasnete_register_threadcleanup(gasneti_loopback_cleanup_threaddata, buf);
        }
        return isReq ? mythread->loopback_requestBuf : mythread->loopback_replyBuf;
    }

    #define gasneti_loopback_free_medium_buffer(buf, isReq_and_TI) ((void)0)
#endif

/* ------------------------------------------------------------------------------------ */
// Types and macros common to loopback and PSHM
// TODO-EX: "promote" conduit-independent gasnetc_nbrhd_* interfaces to gasneti_nbrhd_*

typedef struct {
  gex_Token_Info_t ti;
#if GASNET_DEBUG
  int8_t   handlerRunning; 
  int8_t   replyIssued;    
#endif
#if GASNETI_THREADINFO_OPT
  gasnet_threadinfo_t threadinfo;
#endif
} gasnetc_nbrhd_token_t;

#define gasnetc_token_in_nbrhd(tok) ((uintptr_t)(tok)&1)

#define GASNETI_POST_THREADINFO_FROM_NBRHD_TOKEN(token) \
        GASNET_POST_THREADINFO((gasneti_assert(gasnetc_token_in_nbrhd(token)), \
                                ((gasnetc_nbrhd_token_t *)(1^(uintptr_t)(token)))->threadinfo))

// gasnetc_nbrhd_token_init is used to forge a conduit-independent nbrhd token that can be used to invoke a client AM handler
// It currently has three main clients:
//   Loopback AM delivery, PSHM AM delivery, VIS peer completion callback support
// NOTE 1: The caller of this function is responsible for populating real_token->ti.gex_is_long after return
// NOTE 2: src_jobrank is permitted to specify remote ranks, in order to support VIS PC
// NOTE 3: If real_token->threadinfo is to be used, caller must set after return
GASNETI_INLINE(gasnetc_nbrhd_token_init)
gex_Token_t gasnetc_nbrhd_token_init(
                        gasnetc_nbrhd_token_t *real_token,
                        gex_Rank_t src_jobrank,
                        gex_AM_Entry_t *entry,
                        int isReq)
{
    gasneti_assert(!((uintptr_t)real_token & 1));
    //gasneti_assert(GASNETI_NBRHD_JOBRANK_IS_LOCAL(src_jobrank)); // allow VIS PC to fake a remote token using this routine
  #if !PLATFORM_COMPILER_PGI // Bug 3587
    // generic msgsource() requires srcrank first
    gasneti_assert(!offsetof(gasnetc_nbrhd_token_t,ti.gex_srcrank));
  #endif
    real_token->ti.gex_srcrank = src_jobrank;
    real_token->ti.gex_ep = gasneti_THUNK_EP;
    real_token->ti.gex_entry = entry;
    real_token->ti.gex_is_req = isReq;
  #if GASNET_DEBUG
    real_token->handlerRunning = 1;
    real_token->replyIssued = 0;
  #endif
    return (gex_Token_t)(1|(uintptr_t)real_token);
}

extern gex_TI_t gasnetc_nbrhd_Token_Info(
                gex_Token_t         token,
                gex_Token_Info_t    *info,
                gex_TI_t            mask);

#ifdef GASNETC_ENTERING_HANDLER_HOOK
  #define GASNETC_NBRHD_ENTERING_HANDLER_HOOK GASNETC_ENTERING_HANDLER_HOOK
#else
  /* extern void enterHook(int cat, int isReq, int handlerId, gex_Token_t *token,
   *                       void *buf, size_t nbytes, int numargs, gex_AM_Arg_t *args);
   */
  #define GASNETC_NBRHD_ENTERING_HANDLER_HOOK(cat,isReq,handlerId,token,buf,nbytes,numargs,args) ((void)0)
#endif
#ifdef GASNETC_LEAVING_HANDLER_HOOK
  #define GASNETC_NBRHD_LEAVING_HANDLER_HOOK GASNETC_LEAVING_HANDLER_HOOK
#else
  /* extern void leaveHook(int cat, int isReq);
   */
  #define GASNETC_NBRHD_LEAVING_HANDLER_HOOK(cat,isReq) ((void)0)
#endif

#if GASNET_DEBUG
  #define gasnetc_token_pre_reply_checks(token) do { \
    gasneti_assert(gasnetc_token_in_nbrhd(token));                                     \
    gasnetc_nbrhd_token_t *real_token = (gasnetc_nbrhd_token_t *)(1^(uintptr_t)token); \
    gasneti_assert(real_token);                                                        \
    gasneti_assert(real_token->ti.gex_is_req);                                         \
    gasneti_assert(!real_token->replyIssued);                                          \
  } while (0)
  #define gasnetc_token_post_reply_checks(token, retval) do { \
    gasnetc_nbrhd_token_t *real_token = (gasnetc_nbrhd_token_t *)(1^(uintptr_t)token); \
    real_token->replyIssued = !retval;                                                 \
  } while (0)
#else
  #define gasnetc_token_pre_reply_checks(token) ((void)0)
  #define gasnetc_token_post_reply_checks(token,retval) ((void)0)
#endif

/* ------------------------------------------------------------------------------------ */
// Code shared by loopback (same-process) FP and NP

// After sd, next 3 params (isFixed, isReq, category) will be manifest constants
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_loopback_prepare_inner)
int gasnetc_loopback_prepare_inner(
                        gasneti_AM_SrcDesc_t sd, const int isFixed,
                        const int isReq, const gasneti_category_t category,
                        const void *client_buf,
                        size_t least_payload, size_t most_payload,
                        void *dest_addr, gex_Event_t *lc_opt,
                        gex_Flags_t flags, unsigned int nargs
                        GASNETI_THREAD_FARG)
{
  sd->_nargs = nargs;
  if (category == gasneti_Medium) {
    sd->_void_p = gasneti_loopback_alloc_medium_buffer(isReq GASNETI_THREAD_PASS);
  }

  gasneti_assert(sd->_tofree == NULL);
  if (isFixed) {
    sd->_addr = (/*non-const*/void *)client_buf;
  } else {
    const size_t limit = (category == gasneti_Long) ? GASNETC_MAX_LONG_NBRHD : GASNETC_MAX_MEDIUM_NBRHD;
    const size_t size = MIN(limit, most_payload);
    sd->_size = size;

    if (client_buf) {
      sd->_addr = (/*non-const*/void *)client_buf;
      gasneti_leaf_finish(lc_opt);
    } else if (category == gasneti_Medium) {
      sd->_addr = sd->_gex_buf = sd->_void_p;
    } else if (size <= GASNETC_MAX_MEDIUM_NBRHD) {
      // Long can use medium buffer at less cost than calling malloc
      sd->_addr = sd->_gex_buf = gasneti_loopback_alloc_medium_buffer(isReq GASNETI_THREAD_PASS);
    } else {
      sd->_tofree = gasneti_prepare_alloc_buffer(sd);
    }
  }

  return 0;
}

// After sd, next 3 params (isFixed, isReq, category) will be manifest constants
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_loopback_commit_inner)
void gasnetc_loopback_commit_inner(
                        gasneti_AM_SrcDesc_t sd, const int isFixed,
                        const int isReq, const gasneti_category_t category,
                        gex_AM_Index_t handler, size_t nbytes,
                        void *dest_addr, va_list argptr
                        GASNETI_THREAD_FARG)
{
  const int numargs = sd->_nargs;

  // Stage payload to final location, buf
  void *buf;
  switch (category) {
    case gasneti_Short:
        buf = NULL;
        break;
    case gasneti_Medium:
        buf = sd->_void_p;
        if (isFixed || (buf != sd->_addr)) GASNETI_MEMCPY_SAFE_EMPTY(buf, sd->_addr, nbytes);
        break;
    case gasneti_Long:
        buf = dest_addr;
        GASNETI_MEMCPY_SAFE_EMPTY(buf, sd->_addr, nbytes);
        break;
    default: gasneti_unreachable_error(("Invalid category=%i",(int)category));
  }

  gex_AM_Arg_t pargs[GASNETC_MAX_ARGS_NBRHD];
  gex_EP_t ep = NULL; // TODO-EX: get true value
  gex_AM_Entry_t *handler_entry = gasnetc_get_hentry(ep, handler);
  gex_AM_Fn_t handler_fn = handler_entry->gex_fnptr;

  gasnetc_nbrhd_token_t real_token;
  const gex_Token_t token = gasnetc_nbrhd_token_init(&real_token, gasneti_mynode, handler_entry, isReq);
#if GASNETI_THREADINFO_OPT
  real_token.threadinfo = GASNETI_MYTHREAD;
#endif
  real_token.ti.gex_is_long = (category == gasneti_Long);

  gasneti_assert_int(numargs ,>=, 0);
  gasneti_assert_int(numargs ,<=, GASNETC_MAX_ARGS_NBRHD);
  gasneti_amtbl_check(handler_entry, numargs, category, isReq);

  for (int i = 0; i < numargs; i++) {
    pargs[i] = (gex_AM_Arg_t)va_arg(argptr, gex_AM_Arg_t);
  }

  GASNETC_NBRHD_ENTERING_HANDLER_HOOK(category,isReq,handler,token,buf,nbytes,numargs,pargs);
  switch (category) {
    case gasneti_Short:
        GASNETI_RUN_HANDLER_SHORT(isReq,handler,handler_fn,token,pargs,numargs);
        break;
    case gasneti_Medium:
        GASNETI_RUN_HANDLER_MEDIUM(isReq,handler,handler_fn,token,pargs,numargs,buf,nbytes);
        break;
    case gasneti_Long:
        GASNETI_RUN_HANDLER_LONG(isReq,handler,handler_fn,token,pargs,numargs,buf,nbytes);
        break;
    default: gasneti_unreachable_error(("Invalid category=%i",(int)category));
  }
  GASNETC_NBRHD_LEAVING_HANDLER_HOOK(category,isReq);

  #if GASNET_DEBUG  
    real_token.handlerRunning = 0;
  #endif

  if (category == gasneti_Medium) {
    gasneti_loopback_free_medium_buffer(buf, isReq GASNETI_THREAD_PASS);
  } else if(!isFixed && sd->_gex_buf && (sd->_size <= GASNETC_MAX_MEDIUM_NBRHD)) {
    gasneti_assert(category == gasneti_Long);
    gasneti_loopback_free_medium_buffer(sd->_gex_buf, isReq GASNETI_THREAD_PASS);
  } else if (sd->_tofree) { // Branch to avoid free(NULL) library call overhead for NPAM/cb
    gasneti_free(sd->_tofree);
    sd->_tofree = NULL;
  }
}

/* ------------------------------------------------------------------------------------ */
// FP-AM for loopback (same-process)

// First 2 params (isReq, category) will be manifest constants
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_loopback_ReqRepGeneric)
int gasnetc_loopback_ReqRepGeneric(
                         int isReq, gasneti_category_t category,
                         gex_AM_Index_t handler,
                         void *source_addr, int nbytes, void *dest_addr, 
                         gex_Flags_t flags, int numargs, va_list argptr
                         GASNETI_THREAD_FARG)
{
  struct gasneti_AM_SrcDesc the_sd;
  the_sd._tofree = NULL;

  gasnetc_loopback_prepare_inner(&the_sd, 1, isReq, category, source_addr, 0, 0,
                                 dest_addr, NULL, flags, numargs GASNETI_THREAD_PASS);

  gasneti_assume(the_sd._tofree == NULL); // in case the optimizer lost track
  gasnetc_loopback_commit_inner(&the_sd, 1, isReq, category, handler, nbytes,
                                dest_addr, argptr GASNETI_THREAD_PASS);

  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
// FP-AM for "nbrhd" (PSHM and loopback)
// NOTE: except for IMMEDIATE no-op case, always synchronous LC

// Parameter 'category' will be a manifest constant
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_nbrhd_RequestGeneric)
int gasnetc_nbrhd_RequestGeneric(
                         gasneti_category_t category,
                         gex_Rank_t jobrank, gex_AM_Index_t handler, 
                         void *source_addr, int nbytes, void *dest_ptr, 
                         gex_Flags_t flags, int numargs, va_list argptr
                         GASNETI_THREAD_FARG)
{
  gasneti_assert(GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank));
#if GASNET_PSHM
  switch(category) {
    case gasneti_Short:
        return gasneti_AMPSHM_RequestShort(jobrank, handler, flags, numargs, argptr GASNETI_THREAD_PASS);
        break;
    case gasneti_Medium:
        return gasneti_AMPSHM_RequestMedium(jobrank, handler, source_addr, nbytes,
                                            flags, numargs, argptr GASNETI_THREAD_PASS);
        break;
    case gasneti_Long:
        return gasneti_AMPSHM_RequestLong(jobrank, handler, source_addr, nbytes, dest_ptr,
                                          flags, numargs, argptr GASNETI_THREAD_PASS);
        break;
    default: gasneti_unreachable_error(("Invalid category=%i",(int)category));
  }
  return 0;
#else
  return gasnetc_loopback_ReqRepGeneric(
                               1, category, handler,
                               source_addr, nbytes, dest_ptr, 
                               flags, numargs, argptr GASNETI_THREAD_PASS); 
#endif
}

// Parameter 'category' will be a manifest constant
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_nbrhd_ReplyGeneric)
int gasnetc_nbrhd_ReplyGeneric(
                         gasneti_category_t category,
                         gex_Token_t token, gex_AM_Index_t handler,
                         void *source_addr, int nbytes, void *dest_ptr, 
                         gex_Flags_t flags, int numargs, va_list argptr) {
  gasnetc_token_pre_reply_checks(token);
  int retval;
#if GASNET_PSHM
  switch(category) {
    case gasneti_Short:
        retval = gasneti_AMPSHM_ReplyShort(token, handler, flags, numargs, argptr);
        break;
    case gasneti_Medium:
        retval = gasneti_AMPSHM_ReplyMedium(token, handler, source_addr, nbytes,
                                          flags, numargs, argptr);
        break;
    case gasneti_Long:
        retval = gasneti_AMPSHM_ReplyLong(token, handler, source_addr, nbytes, dest_ptr,
                                        flags, numargs, argptr);
        break;
    default: gasneti_unreachable_error(("Invalid category=%i",(int)category));
  }
#else
  GASNETI_POST_THREADINFO_FROM_NBRHD_TOKEN(token);
  retval = gasnetc_loopback_ReqRepGeneric(
                                 0, category, handler,
                                 source_addr, nbytes, dest_ptr, 
                                 flags, numargs, argptr GASNETI_THREAD_PASS);
#endif
  gasnetc_token_post_reply_checks(token, retval);
  return retval;
}

/* ------------------------------------------------------------------------------------ */
// NP-AM for loopback (same-process, thus lacking destination arguments)

// After sd, next 2 params (isReq, category) will be manifest constants
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_loopback_Prepare)
int gasnetc_loopback_Prepare(
                        gasneti_AM_SrcDesc_t sd,
                        const int isReq, const gasneti_category_t category,
                        const void *client_buf,
                        size_t least_payload, size_t most_payload,
                        void *dest_addr, gex_Event_t *lc_opt,
                        gex_Flags_t flags, unsigned int nargs)
{
  GASNET_POST_THREADINFO(sd->_thread);
  gasneti_assert(sd->_is_nbrhd);
  return gasnetc_loopback_prepare_inner(
                        sd, 0, isReq, category, client_buf,
                        least_payload, most_payload, dest_addr, lc_opt,
                        flags, nargs GASNETI_THREAD_PASS);
}

// After sd, next 2 params (isReq, category) will be manifest constants
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_loopback_Commit)
void gasnetc_loopback_Commit(
                        gasneti_AM_SrcDesc_t sd,
                        const int isReq, const gasneti_category_t category,
                        gex_AM_Index_t handler, size_t nbytes,
                        void *dest_addr, va_list argptr)
{
  GASNET_POST_THREADINFO(sd->_thread);
  gasneti_assert(sd->_is_nbrhd);
  gasnetc_loopback_commit_inner(
                        sd, 0, isReq, category, handler, nbytes,
                        dest_addr, argptr GASNETI_THREAD_PASS);
}

/* ------------------------------------------------------------------------------------ */
// NP-AM for "nbrhd" (PSHM and loopback)

// Parameter 'category' will be a manifest constant
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_nbrhd_PrepareRequest)
gasneti_AM_SrcDesc_t gasnetc_nbrhd_PrepareRequest(
                        gasneti_AM_SrcDesc_t sd,
                        gasneti_category_t   category,
                        gex_Rank_t           jobrank,
                        const void          *client_buf,
                        size_t               least_payload,
                        size_t               most_payload,
                        void                *dest_addr,
                        gex_Event_t         *lc_opt,
                        gex_Flags_t          flags,
                        unsigned int         nargs)
{
  gasneti_assert(GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank));
  sd->_is_nbrhd = 1;
  int imm;
#if GASNET_PSHM
  if (category == gasneti_Medium) {
    imm = gasnetc_AMPSHM_PrepareRequestMedium(sd, jobrank, client_buf, least_payload, most_payload,
                                               lc_opt, flags, nargs);
  } else {
    imm = gasnetc_AMPSHM_PrepareRequestLong(sd, jobrank, client_buf, least_payload, most_payload,
                                             dest_addr, lc_opt, flags, nargs);
  }
#else
  imm = gasnetc_loopback_Prepare(sd, 1, category, client_buf, least_payload, most_payload,
                                  dest_addr, lc_opt, flags, nargs);
#endif
  if (imm) {
    if (sd->_tofree) { // Branch to avoid free(NULL) library call overhead for NPAM/cb
      gasneti_free(sd->_tofree);
      sd->_tofree = NULL;
    }
    gasneti_reset_srcdesc(sd);
    sd = NULL; // GEX_AM_SRCDESC_NO_OP
  } else {
    gasneti_init_sd_poison(sd);
  }
  return sd;
}

// Parameter 'category' will be a manifest constant
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_nbrhd_CommitRequest)
void gasnetc_nbrhd_CommitRequest(
                        gasneti_AM_SrcDesc_t sd,
                        gasneti_category_t   category,
                        gex_AM_Index_t       handler,
                        size_t               nbytes,
                        void                *dest_addr,
                        va_list              argptr)
{
#if GASNET_PSHM
  if (category == gasneti_Medium) {
    gasnetc_AMPSHM_CommitRequestMedium(sd, handler, nbytes, argptr);
  } else {
    gasnetc_AMPSHM_CommitRequestLong(sd, handler, nbytes, dest_addr, argptr);
  }
#else
  gasnetc_loopback_Commit(sd, 1, category, handler, nbytes, dest_addr, argptr);
#endif
}

// Parameter 'category' will be a manifest constant
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_nbrhd_PrepareReply)
gasneti_AM_SrcDesc_t gasnetc_nbrhd_PrepareReply(
                        gasneti_category_t   category,
                        gex_Token_t          token,
                        const void          *client_buf,
                        size_t               least_payload,
                        size_t               most_payload,
                        void                *dest_addr,
                        gex_Event_t         *lc_opt,
                        gex_Flags_t          flags,
                        unsigned int         nargs)
{
  GASNETI_POST_THREADINFO_FROM_NBRHD_TOKEN(token);
  gasneti_AM_SrcDesc_t sd = gasneti_init_reply_srcdesc(GASNETI_THREAD_PASS_ALONE);
  sd->_is_nbrhd = 1;

  if (category == gasneti_Medium) {
    GASNETI_COMMON_PREP_REP(sd,token,client_buf,least_payload,most_payload,dest_addr,lc_opt,flags,nargs,Medium);
  } else {
    GASNETI_COMMON_PREP_REP(sd,token,client_buf,least_payload,most_payload,dest_addr,lc_opt,flags,nargs,Long);
  }

  gasnetc_token_pre_reply_checks(token);
  int imm;
#if GASNET_PSHM
  if (category == gasneti_Medium) {
    imm = gasnetc_AMPSHM_PrepareReplyMedium(sd, token, client_buf, least_payload, most_payload,
                                             lc_opt, flags, nargs);
  } else {
    imm = gasnetc_AMPSHM_PrepareReplyLong(sd, token, client_buf, least_payload, most_payload,
                                           dest_addr, lc_opt, flags, nargs);
  }
#else
  imm = gasnetc_loopback_Prepare(sd, 0, category, client_buf, least_payload, most_payload,
                                  dest_addr, lc_opt, flags, nargs);
#endif
  gasnetc_token_post_reply_checks(token, imm);
  if (imm) {
    if (sd->_tofree) { // Branch to avoid free(NULL) library call overhead for NPAM/cb
      gasneti_free(sd->_tofree);
      sd->_tofree = NULL;
    }
    gasneti_reset_srcdesc(sd);
    sd = NULL; // GEX_AM_SRCDESC_NO_OP
  } else {
    gasneti_init_sd_poison(sd);
  }
  return sd;
}

// Parameter 'category' will be a manifest constant
// which should lead to specialization of the code upon inlining.
GASNETI_INLINE(gasnetc_nbrhd_CommitReply)
void gasnetc_nbrhd_CommitReply(
                        gasneti_AM_SrcDesc_t sd,
                        gasneti_category_t   category,
                        gex_AM_Index_t       handler,
                        size_t               nbytes,
                        void                *dest_addr,
                        va_list              argptr)
{
#if GASNET_PSHM
  if (category == gasneti_Medium) {
    gasnetc_AMPSHM_CommitReplyMedium(sd, handler, nbytes, argptr);
  } else {
    gasnetc_AMPSHM_CommitReplyLong(sd, handler, nbytes, dest_addr, argptr);
  }
#else
  gasnetc_loopback_Commit(sd, 0, category, handler, nbytes, dest_addr, argptr);
#endif
}

#endif
