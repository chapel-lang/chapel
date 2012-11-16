/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_extended_refvis.h,v $
 *     $Date: 2007/10/15 20:00:19 $
 * $Revision: 1.7 $
 * Description: GASNet Vector, Indexed & Strided conduit header
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_EXTENDED_REFVIS_H
#define _GASNET_EXTENDED_REFVIS_H

#include <gasnet_handler.h>

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

#if PLATFORM_COMPILER_HP && GASNETE_LOOPING_DIMS > 7
  /* avoid bugs in HP C preprocessor */
  #undef GASNETE_LOOPING_DIMS
  #define GASNETE_LOOPING_DIMS 7  
#elif PLATFORM_COMPILER_CRAY && GASNETE_LOOPING_DIMS > 4
  /* avoid bugs in Cray C compiler */
  #undef GASNETE_LOOPING_DIMS
  #define GASNETE_LOOPING_DIMS 4  
#elif PLATFORM_COMPILER_COMPAQ && GASNETE_LOOPING_DIMS > 4
  /* avoid bugs in Compaq C optimizer */
  #undef GASNETE_LOOPING_DIMS
  #define GASNETE_LOOPING_DIMS 4 
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

#ifndef GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
  #if GASNETI_HAVE_EOP_INTERFACE
    #define GASNETE_USE_REMOTECONTIG_GATHER_SCATTER 1
  #else
    #define GASNETE_USE_REMOTECONTIG_GATHER_SCATTER 0
  #endif
#endif
#define GASNETE_USE_REMOTECONTIG_GATHER_SCATTER_DEFAULT 0

#ifndef GASNETE_USE_AMPIPELINE
  #if GASNETI_HAVE_EOP_INTERFACE
    #define GASNETE_USE_AMPIPELINE 1
  #else
    #define GASNETE_USE_AMPIPELINE 0
  #endif
#endif
#define GASNETE_USE_AMPIPELINE_DEFAULT 0

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
#define _hidx_gasnete_gets_AMPipeline_reqh    (GASNETE_VIS_HANDLER_BASE+8)
#define _hidx_gasnete_gets_AMPipeline_reph    (GASNETE_VIS_HANDLER_BASE+9)

/*---------------------------------------------------------------------------------*/

#if GASNETE_USE_AMPIPELINE
  MEDIUM_HANDLER_DECL(gasnete_putv_AMPipeline_reqh,2,3);
  SHORT_HANDLER_DECL(gasnete_putvis_AMPipeline_reph,1,2);
  MEDIUM_HANDLER_DECL(gasnete_getv_AMPipeline_reqh,2,3);
  MEDIUM_HANDLER_DECL(gasnete_getv_AMPipeline_reph,2,3);
  MEDIUM_HANDLER_DECL(gasnete_puti_AMPipeline_reqh,5,6);
  MEDIUM_HANDLER_DECL(gasnete_geti_AMPipeline_reqh,5,6);
  MEDIUM_HANDLER_DECL(gasnete_geti_AMPipeline_reph,2,3);
  MEDIUM_HANDLER_DECL(gasnete_puts_AMPipeline_reqh,5,7);
  MEDIUM_HANDLER_DECL(gasnete_gets_AMPipeline_reqh,6,8);
  MEDIUM_HANDLER_DECL(gasnete_gets_AMPipeline_reph,4,5);

  #define GASNETE_VIS_AMPIPELINE_HANDLERS()                               \
    gasneti_handler_tableentry_with_bits(gasnete_putv_AMPipeline_reqh),   \
    gasneti_handler_tableentry_with_bits(gasnete_putvis_AMPipeline_reph), \
    gasneti_handler_tableentry_with_bits(gasnete_getv_AMPipeline_reqh),   \
    gasneti_handler_tableentry_with_bits(gasnete_getv_AMPipeline_reph),   \
    gasneti_handler_tableentry_with_bits(gasnete_puti_AMPipeline_reqh),   \
    gasneti_handler_tableentry_with_bits(gasnete_geti_AMPipeline_reqh),   \
    gasneti_handler_tableentry_with_bits(gasnete_geti_AMPipeline_reph),   \
    gasneti_handler_tableentry_with_bits(gasnete_puts_AMPipeline_reqh),   \
    gasneti_handler_tableentry_with_bits(gasnete_gets_AMPipeline_reqh),   \
    gasneti_handler_tableentry_with_bits(gasnete_gets_AMPipeline_reph),     
#else
  #define GASNETE_VIS_AMPIPELINE_HANDLERS()
#endif

#if GASNETE_USE_AMPIPELINE
  #define GASNETE_REFVIS_HANDLERS()                            \
    /* ptr-width independent handlers */                       \
    /*  gasneti_handler_tableentry_no_bits(gasnete__reqh) */   \
                                                               \
    /* ptr-width dependent handlers */                         \
    /*  gasneti_handler_tableentry_with_bits(gasnete__reqh) */ \
                                                               \
    GASNETE_VIS_AMPIPELINE_HANDLERS()                        
#endif

/*---------------------------------------------------------------------------------*/

#endif
