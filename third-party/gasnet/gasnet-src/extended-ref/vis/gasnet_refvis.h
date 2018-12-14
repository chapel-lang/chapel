/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/vis/gasnet_refvis.h $
 * Description: GASNet Vector, Indexed & Strided conduit header
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_REFVIS_H
#define _GASNET_REFVIS_H

/*---------------------------------------------------------------------------------*/
/* ***  Parameters *** */
/*---------------------------------------------------------------------------------*/
/* following is non-zero iff it's safe to destroy the metadata input arrays
   immediately after initiating each function
*/
#ifndef GASNETE_PUTV_ALLOWS_VOLATILE_METADATA
#define GASNETE_PUTV_ALLOWS_VOLATILE_METADATA 1
#endif
#ifndef GASNETE_GETV_ALLOWS_VOLATILE_METADATA
#define GASNETE_GETV_ALLOWS_VOLATILE_METADATA 1
#endif
#ifndef GASNETE_PUTI_ALLOWS_VOLATILE_METADATA
#define GASNETE_PUTI_ALLOWS_VOLATILE_METADATA 1
#endif
#ifndef GASNETE_GETI_ALLOWS_VOLATILE_METADATA
#define GASNETE_GETI_ALLOWS_VOLATILE_METADATA 1
#endif
#ifndef GASNETE_PUTS_ALLOWS_VOLATILE_METADATA
#define GASNETE_PUTS_ALLOWS_VOLATILE_METADATA 1
#endif
#ifndef GASNETE_GETS_ALLOWS_VOLATILE_METADATA
#define GASNETE_GETS_ALLOWS_VOLATILE_METADATA 1
#endif

/* GASNETE_LOOPING_DIMS: first level of strided performance:
  number of non-trivial striding dimensions to support using an N deep loop nest 
*/
#ifndef GASNETE_LOOPING_DIMS
  #define GASNETE_LOOPING_DIMS 8
#endif

/* GASNETE_DIRECT_DIMS: second level of strided performance:
  number of non-trivial striding dimensions to support using statically allocated metadata 
  (only affects the operation of requests with non-trivial dimensions > GASNETE_LOOPING_DIMS)
*/
#ifndef GASNETE_DIRECT_DIMS
#define GASNETE_DIRECT_DIMS 15
#endif

/* GASNETE_RANDOM_SELECTOR: use random VIS algorithm selection 
  (mostly useful for correctness debugging) */
#ifndef GASNETE_RANDOM_SELECTOR
#define GASNETE_RANDOM_SELECTOR 0
#endif

#ifndef GASNETE_USE_REMOTECONTIG_GATHER_SCATTER // whether or not to compile in this alg
#define GASNETE_USE_REMOTECONTIG_GATHER_SCATTER 1
#endif
#ifndef GASNETE_USE_REMOTECONTIG_GATHER_SCATTER_DEFAULT // runtime alg enable default
#define GASNETE_USE_REMOTECONTIG_GATHER_SCATTER_DEFAULT 1
#endif

#ifndef GASNETE_USE_AMPIPELINE // whether or not to compile in this alg
#define GASNETE_USE_AMPIPELINE 1
#endif
#ifndef GASNETE_USE_AMPIPELINE_DEFAULT // runtime alg enable default
#define GASNETE_USE_AMPIPELINE_DEFAULT 1
#endif

/*---------------------------------------------------------------------------------*/
/* ***  Handlers *** */
/*---------------------------------------------------------------------------------*/
/* conduits may override this to relocate the ref-vis handlers */
#ifndef GASNETE_VIS_HANDLER_BASE
#define GASNETE_VIS_HANDLER_BASE 100
#endif

#define _hidx_gasnete_putv_AMPipeline_reqh    (GASNETE_VIS_HANDLER_BASE+0)
#define _hidx_gasnete_putvis_AMPipeline_reph  (GASNETE_VIS_HANDLER_BASE+1)
#define _hidx_gasnete_getv_AMPipeline_reqh    (GASNETE_VIS_HANDLER_BASE+2)
#define _hidx_gasnete_getv_AMPipeline_reph    (GASNETE_VIS_HANDLER_BASE+3)
#define _hidx_gasnete_puti_AMPipeline_reqh    (GASNETE_VIS_HANDLER_BASE+4)
#define _hidx_gasnete_geti_AMPipeline_reqh    (GASNETE_VIS_HANDLER_BASE+5)
#define _hidx_gasnete_geti_AMPipeline_reph    (GASNETE_VIS_HANDLER_BASE+6)
#define _hidx_gasnete_puts_AMPipeline_reqh    (GASNETE_VIS_HANDLER_BASE+7)
#define _hidx_gasnete_puts_AMPipeline1_reqh   (GASNETE_VIS_HANDLER_BASE+8)
#define _hidx_gasnete_puts_AMPipeline1_reph   (GASNETE_VIS_HANDLER_BASE+9)
#define _hidx_gasnete_gets_AMPipeline_reqh    (GASNETE_VIS_HANDLER_BASE+10)
#define _hidx_gasnete_gets_AMPipeline_reph    (GASNETE_VIS_HANDLER_BASE+11)

#define _hidx_gasnete_vis_pcthunk_reqh        (GASNETE_VIS_HANDLER_BASE+12)

/*---------------------------------------------------------------------------------*/

#if GASNETE_USE_AMPIPELINE
  MEDIUM_HANDLER_DECL(gasnete_putv_AMPipeline_reqh,2,3);
  SHORT_HANDLER_DECL(gasnete_putvis_AMPipeline_reph,1,2);
  MEDIUM_HANDLER_DECL(gasnete_getv_AMPipeline_reqh,2,3);
  MEDIUM_HANDLER_DECL(gasnete_getv_AMPipeline_reph,2,3);
  MEDIUM_HANDLER_DECL(gasnete_puti_AMPipeline_reqh,5,6);
  MEDIUM_HANDLER_DECL(gasnete_geti_AMPipeline_reqh,5,6);
  MEDIUM_HANDLER_DECL(gasnete_geti_AMPipeline_reph,2,3);
 #if GASNETE_PUTS_AMPIPELINE || !defined(GASNETE_PUTS_AMPIPELINE)
  MEDIUM_HANDLER_DECL(gasnete_puts_AMPipeline_reqh,5,7);
  MEDIUM_HANDLER_DECL(gasnete_puts_AMPipeline1_reqh,4,6);
  SHORT_HANDLER_DECL(gasnete_puts_AMPipeline1_reph,1,2);
  #define GASNETE_PUTS_AMPIPELINE_HANDLERS() \
    gasneti_handler_tableentry_with_bits(gasnete_puts_AMPipeline_reqh,5,7,REQUEST,MEDIUM,0), \
    gasneti_handler_tableentry_with_bits(gasnete_puts_AMPipeline1_reqh,4,6,REQUEST,MEDIUM,0),\
    gasneti_handler_tableentry_with_bits(gasnete_puts_AMPipeline1_reph,1,2,REPLY,SHORT,0),
 #else
  #define GASNETE_PUTS_AMPIPELINE_HANDLERS() 
 #endif
 #if GASNETE_GETS_AMPIPELINE || !defined(GASNETE_GETS_AMPIPELINE)
  MEDIUM_HANDLER_DECL(gasnete_gets_AMPipeline_reqh,6,9);
  MEDIUM_HANDLER_DECL(gasnete_gets_AMPipeline_reph,2,3);
  #define GASNETE_GETS_AMPIPELINE_HANDLERS() \
    gasneti_handler_tableentry_with_bits(gasnete_gets_AMPipeline_reqh,6,9,REQUEST,MEDIUM,0),   \
    gasneti_handler_tableentry_with_bits(gasnete_gets_AMPipeline_reph,2,3,REPLY,MEDIUM,0),
 #else
  #define GASNETE_GETS_AMPIPELINE_HANDLERS() 
 #endif
  #define GASNETE_STRIDED_HANDLERS() \
          GASNETE_PUTS_AMPIPELINE_HANDLERS() \
          GASNETE_GETS_AMPIPELINE_HANDLERS() 

  #define GASNETE_VIS_AMPIPELINE_HANDLERS()                               \
    gasneti_handler_tableentry_with_bits(gasnete_putv_AMPipeline_reqh,2,3,REQUEST,MEDIUM,0),   \
    gasneti_handler_tableentry_with_bits(gasnete_putvis_AMPipeline_reph,1,2,REPLY,SHORT,0),    \
    gasneti_handler_tableentry_with_bits(gasnete_getv_AMPipeline_reqh,2,3,REQUEST,MEDIUM,0),   \
    gasneti_handler_tableentry_with_bits(gasnete_getv_AMPipeline_reph,2,3,REPLY,MEDIUM,0),     \
    gasneti_handler_tableentry_with_bits(gasnete_puti_AMPipeline_reqh,5,6,REQUEST,MEDIUM,0),   \
    gasneti_handler_tableentry_with_bits(gasnete_geti_AMPipeline_reqh,5,6,REQUEST,MEDIUM,0),   \
    gasneti_handler_tableentry_with_bits(gasnete_geti_AMPipeline_reph,2,3,REPLY,MEDIUM,0),     \
    GASNETE_STRIDED_HANDLERS()
#else
  #define GASNETE_VIS_AMPIPELINE_HANDLERS()
#endif

MEDIUM_HANDLER_DECL(gasnete_vis_pcthunk_reqh,1,1);

#define GASNETE_REFVIS_HANDLERS()                                                              \
    gasneti_handler_tableentry_with_bits(gasnete_vis_pcthunk_reqh,1,1,REQUEST,MEDIUM,0),       \
    GASNETE_VIS_AMPIPELINE_HANDLERS()                        

/*---------------------------------------------------------------------------------*/

#endif
