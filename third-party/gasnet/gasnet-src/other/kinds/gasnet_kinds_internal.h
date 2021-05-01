/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/kinds/gasnet_kinds_internal.h $
 * Description: GASNet Memory Kinds Internal Header
 * Copyright (c) 2020, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_KINDS_INTERNAL_H
#define _GASNET_KINDS_INTERNAL_H

/*---------------------------------------------------------------------------------*/
//
// Common logic for use by memory kinds implementatons
//

#define GASNETI_MK_MAGIC           GASNETI_MAKE_MAGIC('M','K','_','t')
#define GASNETI_MK_BAD_MAGIC       GASNETI_MAKE_BAD_MAGIC('M','K','_','t')

// Optional default allocation
gasneti_MK_t gasneti_alloc_mk(
            gasneti_Client_t                 i_client,
            gasneti_mk_impl_t               *mk_impl,
            gex_Flags_t                      flags);

// Optional default free
void gasneti_free_mk(gasneti_MK_t mk);

/*---------------------------------------------------------------------------------*/
//
// Declarations for class-specific create functions
//

#define GASNETI_MK_CLASS_DECL(kind) \
  extern int gasneti_MK_Create_##kind(                  \
            gasneti_MK_t                     *i_mk_p,   \
            gasneti_Client_t                 i_client,  \
            const gex_MK_Create_args_t       *args,     \
            gex_Flags_t                      flags);

#if GASNET_HAVE_MK_CLASS_CUDA_UVA
GASNETI_MK_CLASS_DECL(cuda_uva)
#endif


/*---------------------------------------------------------------------------------*/
//
// Dispatch table for class-specific operations
//

struct gasneti_mk_impl_s {
  // GEX_MK_CLASS_* enum value
  gex_MK_Class_t       mk_class;

  // The characters after GEX_MK_CLASS_.  Used in tracing and other messages
  const char          *mk_name;

  // Size of class-specific gasneti_MK_t.
  // Must include the GASNETI_MK_COMMON fields (required as prefix)
  // If zero, then only space for the COMMON fields is allocated
  size_t               mk_sizeof;

  // Hook for gex_MK_Destroy()
  // If NULL, the default is to call gasneti_free_mk()
  void (*mk_destroy)(gasneti_MK_t, gex_Flags_t);

  // Hook for gex_Segment_Create()
  // If NULL, the default is failure of gex_Segment_Create() with GASNET_ERR_BAD_ARG
  int (*mk_segment_create)(gasneti_Segment_t *, gasneti_MK_t, void *, uintptr_t, gex_Flags_t);
};

#endif
