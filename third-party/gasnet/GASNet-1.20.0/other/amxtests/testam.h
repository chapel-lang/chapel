/*   $Source: /var/local/cvs/gasnet/other/amxtests/testam.h,v $
 *     $Date: 2009/03/29 04:03:07 $
 * $Revision: 1.15 $
 * Description: AMX test
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#ifdef TEST_GASNET
  #include "gasnet_tools.h"
  #include "test.h"
  typedef gasnet_handlerarg_t handlerarg_t;
 #ifdef GASNET_USE_STRICT_PROTOTYPES
  typedef void *handler_fn_t;
 #else
  typedef void (*handler_fn_t)();
 #endif
  typedef gasnet_token_t token_t;
  typedef size_t bufsize_t;
  gasnett_atomic_t numreq = gasnett_atomic_init(0);
  gasnett_atomic_t numrep = gasnett_atomic_init(0);
  #define INCREQ() gasnett_atomic_increment(&numreq,0)
  #define INCREP() gasnett_atomic_increment(&numrep,0)
  #define NUMREQ() gasnett_atomic_read(&numreq,0)
  #define NUMREP() gasnett_atomic_read(&numrep,0)
  #define RequestShort(num,args)                GASNET_Safe(gasnet_AMRequestShort##num args)
  #define RequestMedium(num,args)               GASNET_Safe(gasnet_AMRequestMedium##num args)
  #define RequestLong(num,AMargs,GASNETargs)    GASNET_Safe(gasnet_AMRequestLong##num GASNETargs)
  #define RequestLongAsync(num,AMargs,GASNETargs)    GASNET_Safe(gasnet_AMRequestLongAsync##num GASNETargs)
  #define ReplyShort(num,args)                  GASNET_Safe(gasnet_AMReplyShort##num args)
  #define ReplyMedium(num,args)                 GASNET_Safe(gasnet_AMReplyMedium##num args)
  #define ReplyLong(num,AMargs,GASNETargs)      GASNET_Safe(gasnet_AMReplyLong##num GASNETargs)
  #define NUMHANDLERS_PER_TYPE     (gasnet_AMMaxArgs()+1)
  #define MYPROC                   (gasnet_mynode())
  #define NUMPROCS                 (gasnet_nodes())
  #define MYSEG                    (TEST_MYSEG())
  #define ENDPOINT
  #define GETPARTNER(token)  gasnet_node_t partner; GASNET_Safe(gasnet_AMGetMsgSource(token, &partner))
#else
  #include "apputils.h"
  typedef int handlerarg_t;
  typedef amx_handler_fn_t handler_fn_t;
  typedef void *token_t;
  typedef int bufsize_t;
  int numreq = 0;
  int numrep = 0;
  #define INCREQ() numreq++
  #define INCREP() numrep++
  #define NUMREQ() (numreq)
  #define NUMREP() (numrep)
  #define RequestShort(num,args)                AM_Safe(AM_Request##num args)
  #define RequestMedium(num,args)               AM_Safe(AM_RequestI##num args)
  #define RequestLong(num,AMargs,GASNETargs)    AM_Safe(AM_RequestXfer##num AMargs)
  /* AM_RequestXferAsync generates errors if cannot be sent immediately - don't use it */
  #define RequestLongAsync(num,AMargs,GASNETargs) AM_Safe(AM_RequestXfer##num AMargs)
  #define ReplyShort(num,args)                  AM_Safe(AM_Reply##num args)
  #define ReplyMedium(num,args)                 AM_Safe(AM_ReplyI##num args)
  #define ReplyLong(num,AMargs,GASNETargs)      AM_Safe(AM_ReplyXfer##num AMargs)
  #define NUMHANDLERS_PER_TYPE     (AM_MaxShort()+1)
  #define MYPROC                   (AMX_SPMDMyProc())
  #define NUMPROCS                 (AMX_SPMDNumProcs())
  void *VMseg;
  int VMsegsz;
  #define MYSEG                    (VMseg)
  #define ENDPOINT  ep,
  #define GETPARTNER(token)
  #define FATALERR                  AMX_FatalErr
#endif
#define ALLAM_DONE(iters) ((int)NUMREP() == (int)(NUMHANDLERS_PER_TYPE*4*(iters)))

typedef struct {
  double doublevar;
  uint64_t int64var;
  int idx;
} testam_payload_t;
#define TESTAM_DOUBLEVAR_VAL  (2.5f)
#define TESTAM_INT64VAR_VAL   (0xFACEFEEDDEAFBEEFllu)

#define ABASE ((handlerarg_t)0xBABE7000)

#define A1  (ABASE + 1)
#define A2  (ABASE + 2)
#define A3  (ABASE + 3)
#define A4  (ABASE + 4)
#define A5  (ABASE + 5)
#define A6  (ABASE + 6)
#define A7  (ABASE + 7)
#define A8  (ABASE + 8)
#define A9  (ABASE + 9)
#define A10 (ABASE + 10)
#define A11 (ABASE + 11)
#define A12 (ABASE + 12)
#define A13 (ABASE + 13)
#define A14 (ABASE + 14)
#define A15 (ABASE + 15)
#define A16 (ABASE + 16)

#define FA0
#define FA1  FA0 , handlerarg_t a1
#define FA2  FA1 , handlerarg_t a2
#define FA3  FA2 , handlerarg_t a3
#define FA4  FA3 , handlerarg_t a4
#define FA5  FA4 , handlerarg_t a5
#define FA6  FA5 , handlerarg_t a6
#define FA7  FA6 , handlerarg_t a7
#define FA8  FA7 , handlerarg_t a8
#define FA9  FA8 , handlerarg_t a9
#define FA10 FA9 , handlerarg_t a10
#define FA11 FA10, handlerarg_t a11
#define FA12 FA11, handlerarg_t a12
#define FA13 FA12, handlerarg_t a13
#define FA14 FA13, handlerarg_t a14
#define FA15 FA14, handlerarg_t a15
#define FA16 FA15, handlerarg_t a16

#define aa0
#define aa1  aa0 , a1
#define aa2  aa1 , a2
#define aa3  aa2 , a3
#define aa4  aa3 , a4
#define aa5  aa4 , a5
#define aa6  aa5 , a6
#define aa7  aa6 , a7
#define aa8  aa7 , a8
#define aa9  aa8 , a9
#define aa10 aa9 , a10
#define aa11 aa10, a11
#define aa12 aa11, a12
#define aa13 aa12, a13
#define aa14 aa13, a14
#define aa15 aa14, a15
#define aa16 aa15, a16

#define AA0  
#define AA1  AA0 , A1
#define AA2  AA1 , A2
#define AA3  AA2 , A3
#define AA4  AA3 , A4
#define AA5  AA4 , A5
#define AA6  AA5 , A6
#define AA7  AA6 , A7
#define AA8  AA7 , A8
#define AA9  AA8 , A9
#define AA10 AA9 , A10
#define AA11 AA10, A11
#define AA12 AA11, A12
#define AA13 AA12, A13
#define AA14 AA13, A14
#define AA15 AA14, A15
#define AA16 AA15, A16

#define CA0  0
#define CA1  CA0 || a1!=A1
#define CA2  CA1 || a2!=A2
#define CA3  CA2 || a3!=A3
#define CA4  CA3 || a4!=A4
#define CA5  CA4 || a5!=A5
#define CA6  CA5 || a6!=A6
#define CA7  CA6 || a7!=A7
#define CA8  CA7 || a8!=A8
#define CA9  CA8 || a9!=A9
#define CA10 CA9 || a10!=A10
#define CA11 CA10|| a11!=A11
#define CA12 CA11|| a12!=A12
#define CA13 CA12|| a13!=A13
#define CA14 CA13|| a14!=A14
#define CA15 CA14|| a15!=A15
#define CA16 CA15|| a16!=A16

/* we use handlers [SHORT_REQ_BASE...(SHORT_REQ_BASE+120)] */
#ifndef SHORT_REQ_BASE
#define SHORT_REQ_BASE      100
#endif
#define SHORT_0REQ_HANDLER  (SHORT_REQ_BASE+0)
#define SHORT_1REQ_HANDLER  (SHORT_REQ_BASE+1)
#define SHORT_2REQ_HANDLER  (SHORT_REQ_BASE+2)
#define SHORT_3REQ_HANDLER  (SHORT_REQ_BASE+3)
#define SHORT_4REQ_HANDLER  (SHORT_REQ_BASE+4)
#define SHORT_5REQ_HANDLER  (SHORT_REQ_BASE+5)
#define SHORT_6REQ_HANDLER  (SHORT_REQ_BASE+6)
#define SHORT_7REQ_HANDLER  (SHORT_REQ_BASE+7)
#define SHORT_8REQ_HANDLER  (SHORT_REQ_BASE+8)
#define SHORT_9REQ_HANDLER  (SHORT_REQ_BASE+9)
#define SHORT_10REQ_HANDLER (SHORT_REQ_BASE+10)
#define SHORT_11REQ_HANDLER (SHORT_REQ_BASE+11)
#define SHORT_12REQ_HANDLER (SHORT_REQ_BASE+12)
#define SHORT_13REQ_HANDLER (SHORT_REQ_BASE+13)
#define SHORT_14REQ_HANDLER (SHORT_REQ_BASE+14)
#define SHORT_15REQ_HANDLER (SHORT_REQ_BASE+15)
#define SHORT_16REQ_HANDLER (SHORT_REQ_BASE+16)

#define SHORT_REP_BASE      (SHORT_REQ_BASE+20)
#define SHORT_0REP_HANDLER  (SHORT_REP_BASE+0)
#define SHORT_1REP_HANDLER  (SHORT_REP_BASE+1)
#define SHORT_2REP_HANDLER  (SHORT_REP_BASE+2)
#define SHORT_3REP_HANDLER  (SHORT_REP_BASE+3)
#define SHORT_4REP_HANDLER  (SHORT_REP_BASE+4)
#define SHORT_5REP_HANDLER  (SHORT_REP_BASE+5)
#define SHORT_6REP_HANDLER  (SHORT_REP_BASE+6)
#define SHORT_7REP_HANDLER  (SHORT_REP_BASE+7)
#define SHORT_8REP_HANDLER  (SHORT_REP_BASE+8)
#define SHORT_9REP_HANDLER  (SHORT_REP_BASE+9)
#define SHORT_10REP_HANDLER (SHORT_REP_BASE+10)
#define SHORT_11REP_HANDLER (SHORT_REP_BASE+11)
#define SHORT_12REP_HANDLER (SHORT_REP_BASE+12)
#define SHORT_13REP_HANDLER (SHORT_REP_BASE+13)
#define SHORT_14REP_HANDLER (SHORT_REP_BASE+14)
#define SHORT_15REP_HANDLER (SHORT_REP_BASE+15)
#define SHORT_16REP_HANDLER (SHORT_REP_BASE+16)

#define MEDIUM_REQ_BASE      (SHORT_REQ_BASE+40)
#define MEDIUM_0REQ_HANDLER  (MEDIUM_REQ_BASE+0)
#define MEDIUM_1REQ_HANDLER  (MEDIUM_REQ_BASE+1)
#define MEDIUM_2REQ_HANDLER  (MEDIUM_REQ_BASE+2)
#define MEDIUM_3REQ_HANDLER  (MEDIUM_REQ_BASE+3)
#define MEDIUM_4REQ_HANDLER  (MEDIUM_REQ_BASE+4)
#define MEDIUM_5REQ_HANDLER  (MEDIUM_REQ_BASE+5)
#define MEDIUM_6REQ_HANDLER  (MEDIUM_REQ_BASE+6)
#define MEDIUM_7REQ_HANDLER  (MEDIUM_REQ_BASE+7)
#define MEDIUM_8REQ_HANDLER  (MEDIUM_REQ_BASE+8)
#define MEDIUM_9REQ_HANDLER  (MEDIUM_REQ_BASE+9)
#define MEDIUM_10REQ_HANDLER (MEDIUM_REQ_BASE+10)
#define MEDIUM_11REQ_HANDLER (MEDIUM_REQ_BASE+11)
#define MEDIUM_12REQ_HANDLER (MEDIUM_REQ_BASE+12)
#define MEDIUM_13REQ_HANDLER (MEDIUM_REQ_BASE+13)
#define MEDIUM_14REQ_HANDLER (MEDIUM_REQ_BASE+14)
#define MEDIUM_15REQ_HANDLER (MEDIUM_REQ_BASE+15)
#define MEDIUM_16REQ_HANDLER (MEDIUM_REQ_BASE+16)

#define MEDIUM_REP_BASE      (MEDIUM_REQ_BASE+20)
#define MEDIUM_0REP_HANDLER  (MEDIUM_REP_BASE+0)
#define MEDIUM_1REP_HANDLER  (MEDIUM_REP_BASE+1)
#define MEDIUM_2REP_HANDLER  (MEDIUM_REP_BASE+2)
#define MEDIUM_3REP_HANDLER  (MEDIUM_REP_BASE+3)
#define MEDIUM_4REP_HANDLER  (MEDIUM_REP_BASE+4)
#define MEDIUM_5REP_HANDLER  (MEDIUM_REP_BASE+5)
#define MEDIUM_6REP_HANDLER  (MEDIUM_REP_BASE+6)
#define MEDIUM_7REP_HANDLER  (MEDIUM_REP_BASE+7)
#define MEDIUM_8REP_HANDLER  (MEDIUM_REP_BASE+8)
#define MEDIUM_9REP_HANDLER  (MEDIUM_REP_BASE+9)
#define MEDIUM_10REP_HANDLER (MEDIUM_REP_BASE+10)
#define MEDIUM_11REP_HANDLER (MEDIUM_REP_BASE+11)
#define MEDIUM_12REP_HANDLER (MEDIUM_REP_BASE+12)
#define MEDIUM_13REP_HANDLER (MEDIUM_REP_BASE+13)
#define MEDIUM_14REP_HANDLER (MEDIUM_REP_BASE+14)
#define MEDIUM_15REP_HANDLER (MEDIUM_REP_BASE+15)
#define MEDIUM_16REP_HANDLER (MEDIUM_REP_BASE+16)

#define LONG_REQ_BASE      (MEDIUM_REQ_BASE+40)
#define LONG_0REQ_HANDLER  (LONG_REQ_BASE+0)
#define LONG_1REQ_HANDLER  (LONG_REQ_BASE+1)
#define LONG_2REQ_HANDLER  (LONG_REQ_BASE+2)
#define LONG_3REQ_HANDLER  (LONG_REQ_BASE+3)
#define LONG_4REQ_HANDLER  (LONG_REQ_BASE+4)
#define LONG_5REQ_HANDLER  (LONG_REQ_BASE+5)
#define LONG_6REQ_HANDLER  (LONG_REQ_BASE+6)
#define LONG_7REQ_HANDLER  (LONG_REQ_BASE+7)
#define LONG_8REQ_HANDLER  (LONG_REQ_BASE+8)
#define LONG_9REQ_HANDLER  (LONG_REQ_BASE+9)
#define LONG_10REQ_HANDLER (LONG_REQ_BASE+10)
#define LONG_11REQ_HANDLER (LONG_REQ_BASE+11)
#define LONG_12REQ_HANDLER (LONG_REQ_BASE+12)
#define LONG_13REQ_HANDLER (LONG_REQ_BASE+13)
#define LONG_14REQ_HANDLER (LONG_REQ_BASE+14)
#define LONG_15REQ_HANDLER (LONG_REQ_BASE+15)
#define LONG_16REQ_HANDLER (LONG_REQ_BASE+16)

#define LONG_REP_BASE      (LONG_REQ_BASE+20)
#define LONG_0REP_HANDLER  (LONG_REP_BASE+0)
#define LONG_1REP_HANDLER  (LONG_REP_BASE+1)
#define LONG_2REP_HANDLER  (LONG_REP_BASE+2)
#define LONG_3REP_HANDLER  (LONG_REP_BASE+3)
#define LONG_4REP_HANDLER  (LONG_REP_BASE+4)
#define LONG_5REP_HANDLER  (LONG_REP_BASE+5)
#define LONG_6REP_HANDLER  (LONG_REP_BASE+6)
#define LONG_7REP_HANDLER  (LONG_REP_BASE+7)
#define LONG_8REP_HANDLER  (LONG_REP_BASE+8)
#define LONG_9REP_HANDLER  (LONG_REP_BASE+9)
#define LONG_10REP_HANDLER (LONG_REP_BASE+10)
#define LONG_11REP_HANDLER (LONG_REP_BASE+11)
#define LONG_12REP_HANDLER (LONG_REP_BASE+12)
#define LONG_13REP_HANDLER (LONG_REP_BASE+13)
#define LONG_14REP_HANDLER (LONG_REP_BASE+14)
#define LONG_15REP_HANDLER (LONG_REP_BASE+15)
#define LONG_16REP_HANDLER (LONG_REP_BASE+16)

/* ------------------------------------------------------------------------------------ */
#define SHORTHANDLERS(num)                                                         \
  void short_##num##req_handler(token_t token FA##num) {                           \
    if (CA##num)                                                                   \
      FATALERR("Arg mismatch in short_%sreq_handler on P%i\n", #num, (int)MYPROC); \
    INCREQ();                                                                      \
    ReplyShort(num,(token, SHORT_##num##REP_HANDLER aa##num));                     \
  }                                                                                \
  void short_##num##rep_handler(token_t token FA##num) {                           \
    if (CA##num)                                                                   \
      FATALERR("Arg mismatch in short_%srep_handler on P%i\n", #num, (int)MYPROC); \
    INCREP();                                                                      \
  }

#define MEDIUMHANDLERS(num)                                                            \
  void medium_##num##req_handler(token_t token, void *buf, bufsize_t nbytes FA##num) { \
    testam_payload_t *payload = (testam_payload_t *)buf;                               \
    if (CA##num)                                                                       \
      FATALERR("Arg mismatch in medium_%sreq_handler on P%i\n", #num, (int)MYPROC);    \
    if (nbytes != sizeof(testam_payload_t) || payload->idx != num ||                   \
        payload->doublevar != TESTAM_DOUBLEVAR_VAL ||                                  \
        payload->int64var != TESTAM_INT64VAR_VAL)                                      \
      FATALERR("buf mismatch in medium_%sreq_handler on P%i: nbytes=%i, buf=%i\n",     \
                       #num, (int)MYPROC, (int)nbytes, payload->idx);                  \
    INCREQ();                                                                          \
    payload->idx = -payload->idx;                                                      \
    ReplyMedium(num,(token, MEDIUM_##num##REP_HANDLER, buf, nbytes aa##num));          \
    memset(buf, 0xBB, sizeof(testam_payload_t));                                       \
  }                                                                                    \
  void medium_##num##rep_handler(token_t token, void *buf, bufsize_t nbytes FA##num) { \
    testam_payload_t *payload = (testam_payload_t *)buf;                               \
    if (CA##num)                                                                       \
      FATALERR("Arg mismatch in medium_%srep_handler on P%i\n", #num, (int)MYPROC);    \
    if (nbytes != sizeof(testam_payload_t) || payload->idx != -num ||                  \
        payload->doublevar != TESTAM_DOUBLEVAR_VAL ||                                  \
        payload->int64var != TESTAM_INT64VAR_VAL)                                      \
      FATALERR("buf mismatch in medium_%srep_handler on P%i: nbytes=%i, buf=%i\n",     \
                       #num, (int)MYPROC, (int)nbytes, payload->idx);                  \
    INCREP();                                                                          \
  }

#define LONGHANDLERS(num)                                                                     \
  void long_##num##req_handler(token_t token, void *buf, bufsize_t nbytes FA##num) {          \
    testam_payload_t *payload = (testam_payload_t *)buf;                                      \
    testam_payload_t mybuf;                                                                   \
    GETPARTNER(token);                                                                        \
    if (CA##num)                                                                              \
      FATALERR("Arg mismatch in long_%sreq_handler on P%i\n", #num, (int)MYPROC);             \
    if (nbytes != sizeof(testam_payload_t) ||                                                 \
        buf != ((testam_payload_t*)MYSEG)+num ||                                              \
        payload->idx != num ||                                                                \
        payload->doublevar != TESTAM_DOUBLEVAR_VAL ||                                         \
        payload->int64var != TESTAM_INT64VAR_VAL)                                             \
      FATALERR("buf mismatch in long_%sreq_handler on P%i: nbytes=%i, buf=%i\n",              \
                       #num, (int)MYPROC, (int)nbytes, payload->idx);                         \
    mybuf = *payload;                                                                         \
    mybuf.idx = -mybuf.idx;                                                                   \
    INCREQ();                                                                                 \
    ReplyLong(num,(token, (NUMHANDLERS_PER_TYPE+num)*sizeof(testam_payload_t),                \
                   LONG_##num##REP_HANDLER, &mybuf, nbytes aa##num),                          \
                  (token, LONG_##num##REP_HANDLER, &mybuf, nbytes,                            \
                   ((testam_payload_t*)TEST_SEG(partner))+NUMHANDLERS_PER_TYPE+num aa##num)); \
    memset(&mybuf, 0xBB, sizeof(testam_payload_t));                                           \
  }                                                                                           \
  void long_##num##rep_handler(token_t token, void *buf, bufsize_t nbytes FA##num) {          \
    testam_payload_t *payload = (testam_payload_t *)buf;                                      \
    if (CA##num)                                                                              \
      FATALERR("Arg mismatch in long_%srep_handler on P%i\n", #num, (int)MYPROC);             \
    if (nbytes != sizeof(testam_payload_t) ||                                                 \
        buf != ((testam_payload_t*)MYSEG)+NUMHANDLERS_PER_TYPE+num ||                         \
        payload->idx != -num ||                                                               \
        payload->doublevar != TESTAM_DOUBLEVAR_VAL ||                                         \
        payload->int64var != TESTAM_INT64VAR_VAL)                                             \
      FATALERR("buf mismatch in long_%srep_handler on P%i: nbytes=%i, buf=%i\n",              \
                       #num, (int)MYPROC, (int)nbytes, payload->idx);                         \
    INCREP();                                                                                 \
  }

#define HANDLERS(num) \
  SHORTHANDLERS(num)  \
  MEDIUMHANDLERS(num) \
  LONGHANDLERS(num)   

HANDLERS(0)
HANDLERS(1)
HANDLERS(2)
HANDLERS(3)
HANDLERS(4)
HANDLERS(5)
HANDLERS(6)
HANDLERS(7)
HANDLERS(8)
HANDLERS(9)
HANDLERS(10)
HANDLERS(11)
HANDLERS(12)
HANDLERS(13)
HANDLERS(14)
HANDLERS(15)
HANDLERS(16)

#define SETUP_ALLAM_ID(num) do {                                                    \
  AM_Safe(AM_SetHandler(ep, SHORT_##num##REQ_HANDLER,  (handler_fn_t)short_##num##req_handler));  \
  AM_Safe(AM_SetHandler(ep, MEDIUM_##num##REQ_HANDLER, (handler_fn_t)medium_##num##req_handler)); \
  AM_Safe(AM_SetHandler(ep, LONG_##num##REQ_HANDLER,   (handler_fn_t)long_##num##req_handler));   \
  AM_Safe(AM_SetHandler(ep, SHORT_##num##REP_HANDLER,  (handler_fn_t)short_##num##rep_handler));  \
  AM_Safe(AM_SetHandler(ep, MEDIUM_##num##REP_HANDLER, (handler_fn_t)medium_##num##rep_handler)); \
  AM_Safe(AM_SetHandler(ep, LONG_##num##REP_HANDLER,   (handler_fn_t)long_##num##rep_handler));   \
  } while(0)
#define SETUP_ALLAM() do { \
  SETUP_ALLAM_ID(0);       \
  SETUP_ALLAM_ID(1);       \
  SETUP_ALLAM_ID(2);       \
  SETUP_ALLAM_ID(3);       \
  SETUP_ALLAM_ID(4);       \
  SETUP_ALLAM_ID(5);       \
  SETUP_ALLAM_ID(6);       \
  SETUP_ALLAM_ID(7);       \
  SETUP_ALLAM_ID(8);       \
  SETUP_ALLAM_ID(9);       \
  SETUP_ALLAM_ID(10);      \
  SETUP_ALLAM_ID(11);      \
  SETUP_ALLAM_ID(12);      \
  SETUP_ALLAM_ID(13);      \
  SETUP_ALLAM_ID(14);      \
  SETUP_ALLAM_ID(15);      \
  SETUP_ALLAM_ID(16);      \
} while (0)

#define ALLAM_HANDLERS_ID(num)                                           \
  { SHORT_##num##REQ_HANDLER,  (handler_fn_t)short_##num##req_handler }, \
  { MEDIUM_##num##REQ_HANDLER, (handler_fn_t)medium_##num##req_handler}, \
  { LONG_##num##REQ_HANDLER,   (handler_fn_t)long_##num##req_handler  }, \
  { SHORT_##num##REP_HANDLER,  (handler_fn_t)short_##num##rep_handler }, \
  { MEDIUM_##num##REP_HANDLER, (handler_fn_t)medium_##num##rep_handler}, \
  { LONG_##num##REP_HANDLER,   (handler_fn_t)long_##num##rep_handler  }  

#define ALLAM_HANDLERS() \
  ALLAM_HANDLERS_ID(0),  \
  ALLAM_HANDLERS_ID(1),  \
  ALLAM_HANDLERS_ID(2),  \
  ALLAM_HANDLERS_ID(3),  \
  ALLAM_HANDLERS_ID(4),  \
  ALLAM_HANDLERS_ID(5),  \
  ALLAM_HANDLERS_ID(6),  \
  ALLAM_HANDLERS_ID(7),  \
  ALLAM_HANDLERS_ID(8),  \
  ALLAM_HANDLERS_ID(9),  \
  ALLAM_HANDLERS_ID(10), \
  ALLAM_HANDLERS_ID(11), \
  ALLAM_HANDLERS_ID(12), \
  ALLAM_HANDLERS_ID(13), \
  ALLAM_HANDLERS_ID(14), \
  ALLAM_HANDLERS_ID(15), \
  ALLAM_HANDLERS_ID(16)

#define ALLAM_REQ_ID(num, partner)  do {                                                           \
  static testam_payload_t asyncbuf; /* static buf for data lifetime reqt of RequestLongAsync */    \
  static testam_payload_t medbuf, longbuf;                                                         \
  asyncbuf.doublevar = TESTAM_DOUBLEVAR_VAL;                                                       \
  asyncbuf.int64var = TESTAM_INT64VAR_VAL;                                                         \
  asyncbuf.idx = num;                                                                              \
  RequestShort(num,(ENDPOINT partner,  SHORT_##num##REQ_HANDLER AA##num));                         \
  memcpy(&medbuf, &asyncbuf, sizeof(testam_payload_t));                                            \
  RequestMedium(num,(ENDPOINT partner, MEDIUM_##num##REQ_HANDLER,                                  \
                     &medbuf, sizeof(testam_payload_t) AA##num));                                  \
  memset(&medbuf, 0xBB, sizeof(testam_payload_t)); /* ensure we can overwrite srcmem */            \
  memcpy(&longbuf, &asyncbuf, sizeof(testam_payload_t));                                           \
  RequestLong(num,(ENDPOINT partner, sizeof(testam_payload_t)*num,                                 \
                   LONG_##num##REQ_HANDLER, &longbuf, sizeof(testam_payload_t) AA##num),           \
                  (ENDPOINT partner, LONG_##num##REQ_HANDLER, &longbuf, sizeof(testam_payload_t),  \
                   ((testam_payload_t*)TEST_SEG(partner))+num AA##num));                           \
  memset(&longbuf, 0xBB, sizeof(testam_payload_t)); /* ensure we can overwrite srcmem */           \
  RequestLongAsync(num,(ENDPOINT partner, sizeof(testam_payload_t)*num,                            \
                   LONG_##num##REQ_HANDLER, &asyncbuf, sizeof(testam_payload_t) AA##num),          \
                  (ENDPOINT partner, LONG_##num##REQ_HANDLER, &asyncbuf, sizeof(testam_payload_t), \
                   ((testam_payload_t*)TEST_SEG(partner))+num AA##num));                           \
} while (0)                                                                                        \

#define ALLAM_REQ(partner)  do { \
  ALLAM_REQ_ID(0, partner);      \
  ALLAM_REQ_ID(1, partner);      \
  ALLAM_REQ_ID(2, partner);      \
  ALLAM_REQ_ID(3, partner);      \
  ALLAM_REQ_ID(4, partner);      \
  ALLAM_REQ_ID(5, partner);      \
  ALLAM_REQ_ID(6, partner);      \
  ALLAM_REQ_ID(7, partner);      \
  ALLAM_REQ_ID(8, partner);      \
  ALLAM_REQ_ID(9, partner);      \
  ALLAM_REQ_ID(10, partner);     \
  ALLAM_REQ_ID(11, partner);     \
  ALLAM_REQ_ID(12, partner);     \
  ALLAM_REQ_ID(13, partner);     \
  ALLAM_REQ_ID(14, partner);     \
  ALLAM_REQ_ID(15, partner);     \
  ALLAM_REQ_ID(16, partner);     \
} while (0)

