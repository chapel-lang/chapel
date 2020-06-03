/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet.h $
 * Description: GASNet -> GASNet-EX transitional header
 * Copyright 2016, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_H
#define _GASNET_H
#define _INCLUDED_GASNET_H

#include <gasnetex.h>
#include <gasnet_tools.h>

GASNETT_BEGIN_EXTERNC
GASNETI_BEGIN_NOWARN

/* ------------------------------------------------------------------------------------ */
/*
  Globals
  =====================
*/
extern gex_Client_t      gasneti_thunk_client;
extern gex_EP_t          gasneti_thunk_endpoint;
extern gex_TM_t          gasneti_thunk_tm;
extern gex_Segment_t     gasneti_thunk_segment;

/* ------------------------------------------------------------------------------------ */
/*
  Compile-time constants
  =====================
*/
#define SIZEOF_GASNET_REGISTER_VALUE_T SIZEOF_GEX_RMA_VALUE_T

/* public spec version numbers */
#define GASNET_SPEC_VERSION_MAJOR GASNETI_SPEC_VERSION_MAJOR
#define GASNET_SPEC_VERSION_MINOR GASNETI_SPEC_VERSION_MINOR

/*  legacy name for major spec version number */
#define GASNET_VERSION GASNET_SPEC_VERSION_MAJOR

/* ------------------------------------------------------------------------------------ */
/*
  Base types
  ==========
*/

typedef gex_Rank_t gasnet_node_t;
typedef gex_Token_t gasnet_token_t;
typedef gex_AM_Index_t gasnet_handler_t;
typedef gex_AM_Arg_t gasnet_handlerarg_t;
typedef gex_RMA_Value_t gasnet_register_value_t;

typedef gex_Event_t gasnet_handle_t;
#define GASNET_INVALID_HANDLE GEX_EVENT_INVALID

typedef struct {
   void *addr;
   size_t len;
} gasnet_memvec_t;

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/

GASNETT_INLINE(gasnet_init)
int gasnet_init(int *_argc, char ***_argv) {
  gex_Client_t _g2ex_client;
  gex_EP_t     _g2ex_ep;
  gex_TM_t     _g2ex_tm;
  return gex_Client_Init (&_g2ex_client, 
                          &_g2ex_ep, 
                          &_g2ex_tm,
                          "LEGACY", _argc, _argv,
                           GASNETI_FLAG_INIT_LEGACY | GEX_FLAG_USES_GASNET1);
}

extern int gasnetc_attach( gex_TM_t               _tm,
                           gasnet_handlerentry_t  *_table,
                           int                    _numentries,
                           uintptr_t              _segsize);
extern void gasneti_legacy_attach_checks(int _checksegment);
GASNETT_INLINE(gasnet_attach)
int gasnet_attach( gasnet_handlerentry_t *_table, int _numentries,
                   uintptr_t _segsize, uintptr_t _minheapoffset ) {
  gasneti_legacy_attach_checks(0);
  int _result = gasnetc_attach( gasneti_thunk_tm, _table, _numentries, _segsize);
  #if GASNET_SEGMENT_EVERYTHING
    gasneti_legacy_attach_checks(0);
  #else
    gasneti_legacy_attach_checks(1);
  #endif
  return _result;
}

GASNETT_INLINE(gasnet_QueryGexObjects)
void gasnet_QueryGexObjects( gex_Client_t      *_client_p,
                             gex_EP_t          *_endpoint_p,
                             gex_TM_t          *_tm_p,
                             gex_Segment_t     *_segment_p) {
  GASNETI_CHECKATTACH();
  if (_client_p)   *_client_p   = gasneti_thunk_client;
  if (_endpoint_p) *_endpoint_p = gasneti_thunk_endpoint;
  if (_tm_p)       *_tm_p       = gasneti_thunk_tm;
  if (_segment_p)  *_segment_p  = gasneti_thunk_segment;
}

#define gasnet_mynode() (GASNETI_CHECKINIT(), gex_System_QueryJobRank())
#define gasnet_nodes()  (GASNETI_CHECKINIT(), gex_System_QueryJobSize())

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Query Functions
  ==============================
*/

#define gasnet_AMMaxMedium()       MIN(gex_AM_LUBRequestMedium(),\
                                       gex_AM_LUBReplyMedium())
#define gasnet_AMMaxLongRequest()  gex_AM_LUBRequestLong()
#define gasnet_AMMaxLongReply()    gex_AM_LUBReplyLong()
#define gasnet_AMMaxArgs()         ((size_t)gex_AM_MaxArgs())

GASNETT_INLINE(gasnet_AMGetMsgSource)
int gasnet_AMGetMsgSource(gasnet_token_t _token, gasnet_node_t *_srcrank) {
  gex_Token_Info_t _info;
  gex_TI_t _rc = gex_Token_Info(_token, &_info, GEX_TI_SRCRANK);
  gasneti_assert(_rc & GEX_TI_SRCRANK);
  *_srcrank = _info.gex_srcrank;
  return 0;
}

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request/Reply Functions
  ======================================
  These all return 0 in EX for non-immediate, which matches the GASNET_OK success return code in GASNet-1
*/

#define gasnet_AMRequestShort0(dest, handler) \
       gex_AM_RequestShort0(gasneti_thunk_tm, dest, handler, 0)
#define gasnet_AMRequestShort1(dest, handler, a0) \
       gex_AM_RequestShort1(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0)
#define gasnet_AMRequestShort2(dest, handler, a0, a1) \
       gex_AM_RequestShort2(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1)
#define gasnet_AMRequestShort3(dest, handler, a0, a1, a2) \
       gex_AM_RequestShort3(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2)
#define gasnet_AMRequestShort4(dest, handler, a0, a1, a2, a3) \
       gex_AM_RequestShort4(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3)

#define gasnet_AMRequestShort5(dest, handler, a0, a1, a2, a3, a4) \
       gex_AM_RequestShort5(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4)
#define gasnet_AMRequestShort6(dest, handler, a0, a1, a2, a3, a4, a5) \
       gex_AM_RequestShort6(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5)
#define gasnet_AMRequestShort7(dest, handler, a0, a1, a2, a3, a4, a5, a6) \
       gex_AM_RequestShort7(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6)
#define gasnet_AMRequestShort8(dest, handler, a0, a1, a2, a3, a4, a5, a6, a7) \
       gex_AM_RequestShort8(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7)

#define gasnet_AMRequestShort9( dest, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gex_AM_RequestShort9(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8)
#define gasnet_AMRequestShort10(dest, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gex_AM_RequestShort10(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9)
#define gasnet_AMRequestShort11(dest, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gex_AM_RequestShort11(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10)
#define gasnet_AMRequestShort12(dest, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gex_AM_RequestShort12(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11)

#define gasnet_AMRequestShort13(dest, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gex_AM_RequestShort13(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12)
#define gasnet_AMRequestShort14(dest, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gex_AM_RequestShort14(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13)
#define gasnet_AMRequestShort15(dest, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gex_AM_RequestShort15(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14)
#define gasnet_AMRequestShort16(dest, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gex_AM_RequestShort16(gasneti_thunk_tm, dest, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14, (gex_AM_Arg_t)a15)
/* ------------------------------------------------------------------------------------ */
#define gasnet_AMRequestMedium0(dest, handler, source_addr, nbytes) \
       gex_AM_RequestMedium0(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0)
#define gasnet_AMRequestMedium1(dest, handler, source_addr, nbytes, a0) \
       gex_AM_RequestMedium1(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0)
#define gasnet_AMRequestMedium2(dest, handler, source_addr, nbytes, a0, a1) \
       gex_AM_RequestMedium2(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1)
#define gasnet_AMRequestMedium3(dest, handler, source_addr, nbytes, a0, a1, a2) \
       gex_AM_RequestMedium3(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2)
#define gasnet_AMRequestMedium4(dest, handler, source_addr, nbytes, a0, a1, a2, a3) \
       gex_AM_RequestMedium4(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3)

#define gasnet_AMRequestMedium5(dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4) \
       gex_AM_RequestMedium5(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4)
#define gasnet_AMRequestMedium6(dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5) \
       gex_AM_RequestMedium6(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5)
#define gasnet_AMRequestMedium7(dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6) \
       gex_AM_RequestMedium7(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6)
#define gasnet_AMRequestMedium8(dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7) \
       gex_AM_RequestMedium8(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7)

#define gasnet_AMRequestMedium9( dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gex_AM_RequestMedium9(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8)
#define gasnet_AMRequestMedium10(dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gex_AM_RequestMedium10(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9)
#define gasnet_AMRequestMedium11(dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gex_AM_RequestMedium11(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10)
#define gasnet_AMRequestMedium12(dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gex_AM_RequestMedium12(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11)

#define gasnet_AMRequestMedium13(dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gex_AM_RequestMedium13(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12)
#define gasnet_AMRequestMedium14(dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gex_AM_RequestMedium14(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13)
#define gasnet_AMRequestMedium15(dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gex_AM_RequestMedium15(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14)
#define gasnet_AMRequestMedium16(dest, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gex_AM_RequestMedium16(gasneti_thunk_tm, dest, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14, (gex_AM_Arg_t)a15)
/* ------------------------------------------------------------------------------------ */
#define gasnet_AMRequestLong0(dest, handler, source_addr, nbytes, dest_addr) \
       gex_AM_RequestLong0(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0)
#define gasnet_AMRequestLong1(dest, handler, source_addr, nbytes, dest_addr, a0) \
       gex_AM_RequestLong1(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0)
#define gasnet_AMRequestLong2(dest, handler, source_addr, nbytes, dest_addr, a0, a1) \
       gex_AM_RequestLong2(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1)
#define gasnet_AMRequestLong3(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2) \
       gex_AM_RequestLong3(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2)
#define gasnet_AMRequestLong4(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3) \
       gex_AM_RequestLong4(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3)

#define gasnet_AMRequestLong5(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4) \
       gex_AM_RequestLong5(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4)
#define gasnet_AMRequestLong6(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5) \
       gex_AM_RequestLong6(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5)
#define gasnet_AMRequestLong7(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6) \
       gex_AM_RequestLong7(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6)
#define gasnet_AMRequestLong8(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7) \
       gex_AM_RequestLong8(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7)

#define gasnet_AMRequestLong9( dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gex_AM_RequestLong9(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8)
#define gasnet_AMRequestLong10(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gex_AM_RequestLong10(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9)
#define gasnet_AMRequestLong11(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gex_AM_RequestLong11(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10)
#define gasnet_AMRequestLong12(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gex_AM_RequestLong12(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11)

#define gasnet_AMRequestLong13(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gex_AM_RequestLong13(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12)
#define gasnet_AMRequestLong14(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gex_AM_RequestLong14(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13)
#define gasnet_AMRequestLong15(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gex_AM_RequestLong15(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14)
#define gasnet_AMRequestLong16(dest, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gex_AM_RequestLong16(gasneti_thunk_tm, dest, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14, (gex_AM_Arg_t)a15)
/* ------------------------------------------------------------------------------------ */
#define gasnet_AMRequestLongAsync0 gasnet_AMRequestLong0
#define gasnet_AMRequestLongAsync1 gasnet_AMRequestLong1
#define gasnet_AMRequestLongAsync2 gasnet_AMRequestLong2
#define gasnet_AMRequestLongAsync3 gasnet_AMRequestLong3
#define gasnet_AMRequestLongAsync4 gasnet_AMRequestLong4
#define gasnet_AMRequestLongAsync5 gasnet_AMRequestLong5
#define gasnet_AMRequestLongAsync6 gasnet_AMRequestLong6
#define gasnet_AMRequestLongAsync7 gasnet_AMRequestLong7
#define gasnet_AMRequestLongAsync8 gasnet_AMRequestLong8
#define gasnet_AMRequestLongAsync9 gasnet_AMRequestLong9
#define gasnet_AMRequestLongAsync10 gasnet_AMRequestLong10
#define gasnet_AMRequestLongAsync11 gasnet_AMRequestLong11
#define gasnet_AMRequestLongAsync12 gasnet_AMRequestLong12
#define gasnet_AMRequestLongAsync13 gasnet_AMRequestLong13
#define gasnet_AMRequestLongAsync14 gasnet_AMRequestLong14
#define gasnet_AMRequestLongAsync15 gasnet_AMRequestLong15
#define gasnet_AMRequestLongAsync16 gasnet_AMRequestLong16

/* ------------------------------------------------------------------------------------ */

#define gasnet_AMReplyShort0(token, handler) \
       gex_AM_ReplyShort0(token, handler, 0)
#define gasnet_AMReplyShort1(token, handler, a0) \
       gex_AM_ReplyShort1(token, handler, 0, (gex_AM_Arg_t)a0)
#define gasnet_AMReplyShort2(token, handler, a0, a1) \
       gex_AM_ReplyShort2(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1)
#define gasnet_AMReplyShort3(token, handler, a0, a1, a2) \
       gex_AM_ReplyShort3(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2)
#define gasnet_AMReplyShort4(token, handler, a0, a1, a2, a3) \
       gex_AM_ReplyShort4(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3)

#define gasnet_AMReplyShort5(token, handler, a0, a1, a2, a3, a4) \
       gex_AM_ReplyShort5(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4)
#define gasnet_AMReplyShort6(token, handler, a0, a1, a2, a3, a4, a5) \
       gex_AM_ReplyShort6(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5)
#define gasnet_AMReplyShort7(token, handler, a0, a1, a2, a3, a4, a5, a6) \
       gex_AM_ReplyShort7(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6)
#define gasnet_AMReplyShort8(token, handler, a0, a1, a2, a3, a4, a5, a6, a7) \
       gex_AM_ReplyShort8(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7)

#define gasnet_AMReplyShort9( token, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gex_AM_ReplyShort9(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8)
#define gasnet_AMReplyShort10(token, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gex_AM_ReplyShort10(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9)
#define gasnet_AMReplyShort11(token, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gex_AM_ReplyShort11(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10)
#define gasnet_AMReplyShort12(token, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gex_AM_ReplyShort12(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11)

#define gasnet_AMReplyShort13(token, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gex_AM_ReplyShort13(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12)
#define gasnet_AMReplyShort14(token, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gex_AM_ReplyShort14(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13)
#define gasnet_AMReplyShort15(token, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gex_AM_ReplyShort15(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14)
#define gasnet_AMReplyShort16(token, handler, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gex_AM_ReplyShort16(token, handler, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14, (gex_AM_Arg_t)a15)
/* ------------------------------------------------------------------------------------ */
#define gasnet_AMReplyMedium0(token, handler, source_addr, nbytes) \
       gex_AM_ReplyMedium0(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0)
#define gasnet_AMReplyMedium1(token, handler, source_addr, nbytes, a0) \
       gex_AM_ReplyMedium1(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0)
#define gasnet_AMReplyMedium2(token, handler, source_addr, nbytes, a0, a1) \
       gex_AM_ReplyMedium2(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1)
#define gasnet_AMReplyMedium3(token, handler, source_addr, nbytes, a0, a1, a2) \
       gex_AM_ReplyMedium3(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2)
#define gasnet_AMReplyMedium4(token, handler, source_addr, nbytes, a0, a1, a2, a3) \
       gex_AM_ReplyMedium4(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3)

#define gasnet_AMReplyMedium5(token, handler, source_addr, nbytes, a0, a1, a2, a3, a4) \
       gex_AM_ReplyMedium5(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4)
#define gasnet_AMReplyMedium6(token, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5) \
       gex_AM_ReplyMedium6(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5)
#define gasnet_AMReplyMedium7(token, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6) \
       gex_AM_ReplyMedium7(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6)
#define gasnet_AMReplyMedium8(token, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7) \
       gex_AM_ReplyMedium8(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7)

#define gasnet_AMReplyMedium9( token, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gex_AM_ReplyMedium9(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8)
#define gasnet_AMReplyMedium10(token, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gex_AM_ReplyMedium10(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9)
#define gasnet_AMReplyMedium11(token, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gex_AM_ReplyMedium11(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10)
#define gasnet_AMReplyMedium12(token, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gex_AM_ReplyMedium12(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11)

#define gasnet_AMReplyMedium13(token, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gex_AM_ReplyMedium13(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12)
#define gasnet_AMReplyMedium14(token, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gex_AM_ReplyMedium14(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13)
#define gasnet_AMReplyMedium15(token, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gex_AM_ReplyMedium15(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14)
#define gasnet_AMReplyMedium16(token, handler, source_addr, nbytes, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gex_AM_ReplyMedium16(token, handler, source_addr, nbytes, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14, (gex_AM_Arg_t)a15)
/* ------------------------------------------------------------------------------------ */
#define gasnet_AMReplyLong0(token, handler, source_addr, nbytes, dest_addr) \
       gex_AM_ReplyLong0(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0)
#define gasnet_AMReplyLong1(token, handler, source_addr, nbytes, dest_addr, a0) \
       gex_AM_ReplyLong1(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0)
#define gasnet_AMReplyLong2(token, handler, source_addr, nbytes, dest_addr, a0, a1) \
       gex_AM_ReplyLong2(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1)
#define gasnet_AMReplyLong3(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2) \
       gex_AM_ReplyLong3(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2)
#define gasnet_AMReplyLong4(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3) \
       gex_AM_ReplyLong4(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3)

#define gasnet_AMReplyLong5(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4) \
       gex_AM_ReplyLong5(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4)
#define gasnet_AMReplyLong6(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5) \
       gex_AM_ReplyLong6(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5)
#define gasnet_AMReplyLong7(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6) \
       gex_AM_ReplyLong7(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6)
#define gasnet_AMReplyLong8(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7) \
       gex_AM_ReplyLong8(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7)

#define gasnet_AMReplyLong9( token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8 ) \
        gex_AM_ReplyLong9(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8)
#define gasnet_AMReplyLong10(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
        gex_AM_ReplyLong10(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9)
#define gasnet_AMReplyLong11(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
        gex_AM_ReplyLong11(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10)
#define gasnet_AMReplyLong12(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
        gex_AM_ReplyLong12(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11)

#define gasnet_AMReplyLong13(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
        gex_AM_ReplyLong13(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12)
#define gasnet_AMReplyLong14(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
        gex_AM_ReplyLong14(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13)
#define gasnet_AMReplyLong15(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
        gex_AM_ReplyLong15(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14)
#define gasnet_AMReplyLong16(token, handler, source_addr, nbytes, dest_addr, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
        gex_AM_ReplyLong16(token, handler, source_addr, nbytes, dest_addr, GEX_EVENT_NOW, 0, (gex_AM_Arg_t)a0, (gex_AM_Arg_t)a1, (gex_AM_Arg_t)a2, (gex_AM_Arg_t)a3, (gex_AM_Arg_t)a4, (gex_AM_Arg_t)a5, (gex_AM_Arg_t)a6, (gex_AM_Arg_t)a7, (gex_AM_Arg_t)a8, (gex_AM_Arg_t)a9, (gex_AM_Arg_t)a10, (gex_AM_Arg_t)a11, (gex_AM_Arg_t)a12, (gex_AM_Arg_t)a13, (gex_AM_Arg_t)a14, (gex_AM_Arg_t)a15)

/* ------------------------------------------------------------------------------------ */
/*
  Variable-Argument Active Message Request/Reply Functions
  ========================================================
  These were never a normative feature of GASNet-1, but are provided as best-effort for
  backwards compatibility of clients who may rely upon them.
  They will only be available for compilers supporting C99-style vararg macros - this is 
  guaranteed by std C99 and C++11, but can otherwise be forced with -DGASNETI_FORCE_VA_ARG

  Note these differed in signature from the similar feature in EX in that they require the
  client to explicitly pass the argument count before the argument list
*/

#if GASNETI_USING_VA_ARG

  #define _GASNETI_ARGCOUNT_VAL(_stem, ...) _stem
  #define GASNETI_ARGCOUNT_VAL(...) _GASNETI_ARGCOUNT_VAL(__VA_ARGS__,gasneti_dummy)

  // First element of VA_ARGS below is the (possibly computed) argument count, 
  // which is checked against the actual argument count and then discarded by folding into the flags arg
  #define gasnet_AMRequestShort(node,hidx,...) ( \
          gasneti_assert_reason_always(GASNETI_ARGCOUNT_VAL(__VA_ARGS__) == GASNETI_AMNUMARGS(__VA_ARGS__), \
                                       "Argument count mismatch to gasnet_AMRequestShort()"), \
          GASNETI_AMVA(RequestShort,__VA_ARGS__)(gasneti_thunk_tm,node,hidx,0&&__VA_ARGS__) )

  #define gasnet_AMRequestMedium(node,hidx,src_addr,nbytes,...) ( \
          gasneti_assert_reason_always(GASNETI_ARGCOUNT_VAL(__VA_ARGS__) == GASNETI_AMNUMARGS(__VA_ARGS__), \
                                       "Argument count mismatch to gasnet_AMRequestMedium()"), \
          GASNETI_AMVA(RequestMedium,__VA_ARGS__)(gasneti_thunk_tm,node,hidx,src_addr,nbytes,GEX_EVENT_NOW,0&&__VA_ARGS__) )

  #define gasnet_AMRequestLong(node,hidx,src_addr,nbytes,dst_addr,...) ( \
          gasneti_assert_reason_always(GASNETI_ARGCOUNT_VAL(__VA_ARGS__) == GASNETI_AMNUMARGS(__VA_ARGS__), \
                                       "Argument count mismatch to gasnet_AMRequestLong()"), \
          GASNETI_AMVA(RequestLong,__VA_ARGS__)(gasneti_thunk_tm,node,hidx,src_addr,nbytes,dst_addr,GEX_EVENT_NOW,0&&__VA_ARGS__) )

  #define gasnet_AMRequestLongAsync(node,hidx,src_addr,nbytes,dst_addr,...) ( \
          gasneti_assert_reason_always(GASNETI_ARGCOUNT_VAL(__VA_ARGS__) == GASNETI_AMNUMARGS(__VA_ARGS__), \
                                       "Argument count mismatch to gasnet_AMRequestLongAsync()"), \
          GASNETI_AMVA(RequestLong,__VA_ARGS__)(gasneti_thunk_tm,node,hidx,src_addr,nbytes,dst_addr,GEX_EVENT_NOW,0&&__VA_ARGS__) )

  #define gasnet_AMReplyShort(token,hidx,...) ( \
          gasneti_assert_reason_always(GASNETI_ARGCOUNT_VAL(__VA_ARGS__) == GASNETI_AMNUMARGS(__VA_ARGS__), \
                                       "Argument count mismatch to gasnet_AMReplyShort()"), \
          GASNETI_AMVA(ReplyShort,__VA_ARGS__)(token,hidx,0&&__VA_ARGS__) )

  #define gasnet_AMReplyMedium(token,hidx,src_addr,nbytes,...) ( \
          gasneti_assert_reason_always(GASNETI_ARGCOUNT_VAL(__VA_ARGS__) == GASNETI_AMNUMARGS(__VA_ARGS__), \
                                       "Argument count mismatch to gasnet_AMReplyMedium()"), \
          GASNETI_AMVA(ReplyMedium,__VA_ARGS__)(token,hidx,src_addr,nbytes,GEX_EVENT_NOW,0&&__VA_ARGS__) )

  #define gasnet_AMReplyLong(token,hidx,src_addr,nbytes,dst_addr,...) ( \
          gasneti_assert_reason_always(GASNETI_ARGCOUNT_VAL(__VA_ARGS__) == GASNETI_AMNUMARGS(__VA_ARGS__), \
                                       "Argument count mismatch to gasnet_AMReplyLong()"), \
          GASNETI_AMVA(ReplyLong,__VA_ARGS__)(token,hidx,src_addr,nbytes,dst_addr,GEX_EVENT_NOW,0&&__VA_ARGS__) )
#endif

/* ------------------------------------------------------------------------------------ */
/* Handler-safe locks */

#define gasnet_hsl_t    	gex_HSL_t
#define gasnet_hsl_init	        gex_HSL_Init
#define gasnet_hsl_destroy	gex_HSL_Destroy
#define gasnet_hsl_lock         gex_HSL_Lock
#define gasnet_hsl_unlock	gex_HSL_Unlock
#define gasnet_hsl_trylock	gex_HSL_Trylock
#define GASNET_HSL_INITIALIZER	GEX_HSL_INITIALIZER

/* ------------------------------------------------------------------------------------ */
/* Blocking Put and Get */

#define gasnet_put(node,dest,src,nbytes) \
                ((void)gex_RMA_PutBlocking(gasneti_thunk_tm,node,dest,src,nbytes,0))
#define gasnet_get(dest,node,src,nbytes) \
                ((void)gex_RMA_GetBlocking(gasneti_thunk_tm,dest,node,src,nbytes,0))
#define gasnet_put_bulk gasnet_put
#define gasnet_get_bulk gasnet_get

/* ------------------------------------------------------------------------------------ */
/* Implicit-handle non-blocking Put and Get */

#define gasnet_put_nbi(node,dest,src,nbytes) \
                ((void)gex_RMA_PutNBI(gasneti_thunk_tm,node,dest,src,nbytes,GEX_EVENT_NOW,0))
#define gasnet_put_nbi_bulk(node,dest,src,nbytes) \
                ((void)gex_RMA_PutNBI(gasneti_thunk_tm,node,dest,src,nbytes,GEX_EVENT_DEFER,0))
#define gasnet_get_nbi(dest,node,src,nbytes) \
                ((void)gex_RMA_GetNBI(gasneti_thunk_tm,dest,node,src,nbytes,0))
#define gasnet_get_nbi_bulk gasnet_get_nbi

/* ------------------------------------------------------------------------------------ */
/* Explicit-handle non-blocking Put and Get */

#define gasnet_put_nb(node,dest,src,nbytes) \
                gex_RMA_PutNB(gasneti_thunk_tm,node,dest,src,nbytes,GEX_EVENT_NOW,0)
#define gasnet_put_nb_bulk(node,dest,src,nbytes) \
                gex_RMA_PutNB(gasneti_thunk_tm,node,dest,src,nbytes,GEX_EVENT_DEFER,0)
#define gasnet_get_nb(dest,node,src,nbytes) \
                gex_RMA_GetNB(gasneti_thunk_tm,dest,node,src,nbytes,0)
#define gasnet_get_nb_bulk gasnet_get_nb

/* ------------------------------------------------------------------------------------ */
/* Value Gets - blocking and explicit-handle non-blocking */

#define gasnet_get_val(node,src,nbytes) \
                gex_RMA_GetBlockingVal(gasneti_thunk_tm,node,src,nbytes,0)

typedef struct {
  gex_RMA_Value_t gasneti_valget_value;
  gex_Event_t     gasneti_valget_event;
} *gasnet_valget_handle_t;

GASNETT_INLINE(gasnet_get_nb_val)
gasnet_valget_handle_t gasnet_get_nb_val(gasnet_node_t _node, void *_src, size_t _nbytes)
{
  gasnet_valget_handle_t _result = (gasnet_valget_handle_t)gasneti_extern_malloc(sizeof(*_result));
#ifdef PLATFORM_ARCH_BIG_ENDIAN
  void *_dest = (void*)((uintptr_t)&(_result->gasneti_valget_value) + sizeof(gex_RMA_Value_t) - _nbytes);
#else /* little-endian */
  void *_dest = &_result->gasneti_valget_value;
#endif
  _result->gasneti_valget_value = 0;
  //assert(_nbytes > 0 && _nbytes <= sizeof(gex_RMA_Value_t));
  _result->gasneti_valget_event = gex_RMA_GetNB(gasneti_thunk_tm, _dest, _node, _src, _nbytes, 0);
  return _result;
}

GASNETT_INLINE(gasnet_wait_syncnb_valget)
gasnet_register_value_t gasnet_wait_syncnb_valget(gasnet_valget_handle_t _handle)
{
  gex_RMA_Value_t _result;
  gex_Event_Wait(_handle->gasneti_valget_event);
  _result = _handle->gasneti_valget_value;
  gasneti_extern_free(_handle);
  return _result;
}


/* ------------------------------------------------------------------------------------ */
/* Value Puts - blocking, and explicit- and implicit-handle non-blocking */

#define gasnet_put_val(node,dest,value,nbytes) \
         ((void)gex_RMA_PutBlockingVal(gasneti_thunk_tm,node,dest,value,nbytes,0))
#define gasnet_put_nb_val(node,dest,value,nbytes) \
                gex_RMA_PutNBVal(gasneti_thunk_tm,node,dest,value,nbytes,0)
#define gasnet_put_nbi_val(node,dest,value,nbytes) \
         ((void)gex_RMA_PutNBIVal(gasneti_thunk_tm,node,dest,value,nbytes,0))

/* ------------------------------------------------------------------------------------ */
/* Memset */

extern gex_Event_t gasneti_legacy_memset_nb (gex_Rank_t _node, void *_dest, int _val, size_t _nbytes GASNETI_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
extern int         gasneti_legacy_memset_nbi(gex_Rank_t _node, void *_dest, int _val, size_t _nbytes GASNETI_THREAD_FARG);

#define gasnet_memset(node,dest,val,nbytes)     gasnet_wait_syncnb(gasneti_legacy_memset_nb(node,dest,val,nbytes GASNETI_THREAD_GET))
#define gasnet_memset_nb(node,dest,val,nbytes)  gasneti_legacy_memset_nb(node,dest,val,nbytes GASNETI_THREAD_GET)
#define gasnet_memset_nbi(node,dest,val,nbytes) ((void)gasneti_legacy_memset_nbi(node,dest,val,nbytes GASNETI_THREAD_GET))

/* ------------------------------------------------------------------------------------ */
/* Explicit-handle sync operations */

#define gasnet_try_syncnb_nopoll(h)          gex_Event_Test(h)
#define gasnet_try_syncnb_some_nopoll(ph,sz) gex_Event_TestSome(ph,sz,0)
#define gasnet_try_syncnb_all_nopoll(ph,sz)  gex_Event_TestAll(ph,sz,0)

#define gasnet_try_syncnb(h)          (gasnet_AMPoll(),gex_Event_Test(h))
#define gasnet_try_syncnb_some(ph,sz) (gasnet_AMPoll(),gex_Event_TestSome(ph,sz,0))
#define gasnet_try_syncnb_all(ph,sz)  (gasnet_AMPoll(),gex_Event_TestAll(ph,sz,0))

#define gasnet_wait_syncnb(h)          gex_Event_Wait(h)
#define gasnet_wait_syncnb_some(ph,sz) gex_Event_WaitSome(ph,sz,0)
#define gasnet_wait_syncnb_all(ph,sz)  gex_Event_WaitAll(ph,sz,0)

/* ------------------------------------------------------------------------------------ */
/* Implicit-handle sync operations */

#define gasnet_try_syncnbi_gets() (gasnet_AMPoll(),gex_NBI_Test(GEX_EC_GET,0))
#define gasnet_try_syncnbi_puts() (gasnet_AMPoll(),gex_NBI_Test(GEX_EC_PUT,0))
#define gasnet_try_syncnbi_all()  (gasnet_AMPoll(),gex_NBI_Test(GEX_EC_ALL,0))

#define gasnet_wait_syncnbi_gets() gex_NBI_Wait(GEX_EC_GET,0)
#define gasnet_wait_syncnbi_puts() gex_NBI_Wait(GEX_EC_PUT,0)
#define gasnet_wait_syncnbi_all()  gex_NBI_Wait(GEX_EC_ALL,0)

/* ------------------------------------------------------------------------------------ */
/* Implicit-handle access regions */

#define gasnet_begin_nbi_accessregion() gex_NBI_BeginAccessRegion(0)
#define gasnet_end_nbi_accessregion()   gex_NBI_EndAccessRegion(0)

/* ------------------------------------------------------------------------------------ */
/* No-interrupt sections - GASNet-1 compliant empty implementation */

#define gasnet_hold_interrupts()   ((void)0)
#define gasnet_resume_interrupts() ((void)0)

/* ------------------------------------------------------------------------------------ */
/* VIS */

#define gasnet_putv_bulk(dstrank,dstcount,dstlist,srccount,srclist) \
 ((void)gex_VIS_VectorPutBlocking(gasneti_thunk_tm,dstrank,dstcount,(gex_Memvec_t*)(dstlist),srccount,(gex_Memvec_t*)(srclist),0))
#define gasnet_getv_bulk(dstcount,dstlist,srcrank,srccount,srclist) \
 ((void)gex_VIS_VectorGetBlocking(gasneti_thunk_tm,dstcount,(gex_Memvec_t*)(dstlist),srcrank,srccount,(gex_Memvec_t*)(srclist),0))
#define gasnet_putv_nb_bulk(dstrank,dstcount,dstlist,srccount,srclist) \
       gex_VIS_VectorPutNB(gasneti_thunk_tm,dstrank,dstcount,(gex_Memvec_t*)(dstlist),srccount,(gex_Memvec_t*)(srclist),0)
#define gasnet_getv_nb_bulk(dstcount,dstlist,srcrank,srccount,srclist) \
       gex_VIS_VectorGetNB(gasneti_thunk_tm,dstcount,(gex_Memvec_t*)(dstlist),srcrank,srccount,(gex_Memvec_t*)(srclist),0)
#define gasnet_putv_nbi_bulk(dstrank,dstcount,dstlist,srccount,srclist) \
 ((void)gex_VIS_VectorPutNBI(gasneti_thunk_tm,dstrank,dstcount,(gex_Memvec_t*)(dstlist),srccount,(gex_Memvec_t*)(srclist),0))
#define gasnet_getv_nbi_bulk(dstcount,dstlist,srcrank,srccount,srclist) \
 ((void)gex_VIS_VectorGetNBI(gasneti_thunk_tm,dstcount,(gex_Memvec_t*)(dstlist),srcrank,srccount,(gex_Memvec_t*)(srclist),0))
#define gasnet_puti_bulk(dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen) \
 ((void)gex_VIS_IndexedPutBlocking(gasneti_thunk_tm,dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen,0))
#define gasnet_geti_bulk(dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen) \
 ((void)gex_VIS_IndexedGetBlocking(gasneti_thunk_tm,dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen,0))
#define gasnet_puti_nb_bulk(dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen) \
       gex_VIS_IndexedPutNB(gasneti_thunk_tm,dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen,0)
#define gasnet_geti_nb_bulk(dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen) \
       gex_VIS_IndexedGetNB(gasneti_thunk_tm,dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen,0)
#define gasnet_puti_nbi_bulk(dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen) \
 ((void)gex_VIS_IndexedPutNBI(gasneti_thunk_tm,dstrank,dstcount,dstlist,dstlen,srccount,srclist,srclen,0))
#define gasnet_geti_nbi_bulk(dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen) \
 ((void)gex_VIS_IndexedGetNBI(gasneti_thunk_tm,dstcount,dstlist,dstlen,srcrank,srccount,srclist,srclen,0))

// dedicated g2ex wrappers translate the Strided metadata from legacy to EX format
#define gasnet_puts_bulk(dstrank,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_puts_bulk(gasneti_thunk_tm,dstrank,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETI_THREAD_GET)
#define gasnet_gets_bulk(dstaddr,dststrides,srcrank,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_gets_bulk(gasneti_thunk_tm,dstaddr,dststrides,srcrank,srcaddr,srcstrides,count,stridelevels GASNETI_THREAD_GET)
#define gasnet_puts_nb_bulk(dstrank,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_puts_nb_bulk(gasneti_thunk_tm,dstrank,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETI_THREAD_GET)
#define gasnet_gets_nb_bulk(dstaddr,dststrides,srcrank,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_gets_nb_bulk(gasneti_thunk_tm,dstaddr,dststrides,srcrank,srcaddr,srcstrides,count,stridelevels GASNETI_THREAD_GET)
#define gasnet_puts_nbi_bulk(dstrank,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_puts_nbi_bulk(gasneti_thunk_tm,dstrank,dstaddr,dststrides,srcaddr,srcstrides,count,stridelevels GASNETI_THREAD_GET)
#define gasnet_gets_nbi_bulk(dstaddr,dststrides,srcrank,srcaddr,srcstrides,count,stridelevels) \
       _gasnet_gets_nbi_bulk(gasneti_thunk_tm,dstaddr,dststrides,srcrank,srcaddr,srcstrides,count,stridelevels GASNETI_THREAD_GET)

/* ------------------------------------------------------------------------------------ */
GASNETI_END_NOWARN
GASNETT_END_EXTERNC

#endif
