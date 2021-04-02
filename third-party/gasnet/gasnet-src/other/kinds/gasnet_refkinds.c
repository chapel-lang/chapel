/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/kinds/gasnet_refkinds.c $
 * Description: GASNet Memory Kinds Implementation
 * Copyright (c) 2020, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#define GASNETI_NEED_GASNET_MK_H 1
#include <gasnet_internal.h>
#include <gasnet_kinds_internal.h>


#ifndef gasneti_import_mk
gasneti_MK_t gasneti_import_mk(gex_MK_t _mk) {
  const gasneti_MK_t _real_mk = GASNETI_IMPORT_POINTER(gasneti_MK_t,_mk);
  GASNETI_IMPORT_MAGIC(_real_mk, MK);
  return _real_mk;
}
#endif

#ifndef gasneti_import_mk_nonhost
gasneti_MK_t gasneti_import_mk_nonhost(gex_MK_t _mk) {
  if (_mk == GEX_MK_HOST) {
    gasneti_fatalerror("Invalid use of GEX_MK_HOST where prohibited");
  }
  return gasneti_import_mk(_mk);
}
#endif

#ifndef gasneti_export_mk
gex_MK_t gasneti_export_mk(gasneti_MK_t _real_mk) {
  GASNETI_CHECK_MAGIC(_real_mk, GASNETI_MK_MAGIC);
  return GASNETI_EXPORT_POINTER(gex_MK_t, _real_mk);
}
#endif

// TODO: what to do about conduit-spcific extension?
gasneti_MK_t gasneti_alloc_mk(
            gasneti_Client_t                 i_client,
            gasneti_mk_impl_t               *mk_impl,
            gex_Flags_t                      flags)
{
  gasneti_assert(mk_impl);

  gasneti_MK_t mk;
  size_t alloc_size = mk_impl->mk_sizeof ? mk_impl->mk_sizeof : sizeof(*mk);
  gasneti_assert(alloc_size >= sizeof(*mk));
  mk = gasneti_calloc(1, alloc_size);

  GASNETI_INIT_MAGIC(mk, GASNETI_MK_MAGIC);
  mk->_cdata = NULL;
  mk->_flags = flags;
  mk->_client = i_client;
  mk->_mk_class = mk_impl->mk_class;
  mk->_mk_impl = mk_impl;
  gasneti_weakatomic32_set(&mk->_ref_count, 0, 0);
  return mk;
}

void gasneti_free_mk(gasneti_MK_t mk)
{
  GASNETI_INIT_MAGIC(mk, GASNETI_MK_BAD_MAGIC);
  gasneti_free(mk);
}

// Convenience macro
#define MK_IMPL(i_mk,short_field) ((i_mk)->_mk_impl->mk_##short_field)

int gex_MK_Create(
            gex_MK_t                         *memkind_p,
            gex_Client_t                     e_client,
            const gex_MK_Create_args_t       *args,
            gex_Flags_t                      flags)
{
  gasneti_Client_t client = gasneti_import_client(e_client);
  gasneti_MK_t result = NULL;
  int rc = GASNET_ERR_BAD_ARG;

  GASNETI_TRACE_PRINTF(O,("gex_MK_Create: client='%s' flags=%d",
                          client ? client->_name : "(NULL)", flags));

  if (! client) {
    gasneti_fatalerror("Invalid call to gex_MK_Create with NULL client");
  }
  if (!memkind_p) {
    gasneti_fatalerror("Invalid call to gex_MK_Create with NULL memkind_p");
  }
  if (!args) {
    gasneti_fatalerror("Invalid call to gex_MK_Create with NULL args");
  }
  if (flags) {
    gasneti_fatalerror("Invalid call to gex_MK_Create with non-zero flags");
  }
  if (args->gex_flags) {
    gasneti_fatalerror("Invalid call to gex_MK_Create with non-zero args->gex_flags");
  }

  switch (args->gex_class) {
    case GEX_MK_CLASS_HOST:
      gasneti_fatalerror("Invalid call to gex_MK_Create with GEX_MK_CLASS_HOST");
      break;

    case GEX_MK_CLASS_CUDA_UVA:
    #if GASNET_HAVE_MK_CLASS_CUDA_UVA
      rc = gasneti_MK_Create_cuda_uva(&result, client, args, flags);
    #else
      GASNETI_RETURN_ERRR(BAD_ARG,"This build lacks support for GEX_MK_CLASS_CUDA_UVA");
    #endif
      break;

    default: gasneti_unreachable_error(("Unknown MK class: %i",(int)args->gex_class));
  }

  if (! rc) {
    // Sanity checks on per-class initialization
    gasneti_assert(result->_mk_class == args->gex_class);
    gasneti_assert(result->_mk_impl);
    gasneti_assert(MK_IMPL(result,class) == args->gex_class);
    gasneti_assert(MK_IMPL(result,name));
    gasneti_assert(strlen(MK_IMPL(result,name)));

    *memkind_p = gasneti_export_mk(result);
  }

  return rc;
}

void gex_MK_Destroy(
            gex_MK_t                    e_mk,
            gex_Flags_t                 flags)
{
  if (e_mk == GEX_MK_INVALID) {
    gasneti_fatalerror("Invalid call to gex_MK_Destroy(GEX_MK_INVALID)");
  }
  if (e_mk == GEX_MK_HOST) {
    gasneti_fatalerror("Invalid call to gex_MK_Destroy(GEX_MK_HOST)");
  }

  gasneti_MK_t i_mk = gasneti_import_mk(e_mk); // "this"
  gasneti_assert(i_mk->_mk_impl);
  gasneti_assert(MK_IMPL(i_mk,name));

  GASNETI_TRACE_PRINTF(O,("gex_MK_Destroy: memkind=%p, class='%s' flags=%d",
                          (void*)e_mk, MK_IMPL(i_mk,name), flags));

  if (flags) {
    gasneti_fatalerror("Invalid call to gex_MK_Destroy with non-zero flags");
  }

  uint32_t ref_count = gasneti_weakatomic32_read(&i_mk->_ref_count, 0);
  if (ref_count) {
    gasneti_fatalerror("Invalid call to gex_MK_Destroy with ref_count=%u",
                       (unsigned int)ref_count);
  }

  // Class-specific hook or default if none
  if (MK_IMPL(i_mk,destroy)) MK_IMPL(i_mk,destroy)(i_mk, flags);
  else                       gasneti_free_mk(i_mk);
}

int gasneti_MK_Segment_Create(
            gasneti_Segment_t *i_segment_p,
            gasneti_Client_t  i_client,
            void              *addr,
            uintptr_t         size,
            gex_MK_t          e_mk,
            gex_Flags_t       flags)
{
  gasneti_assert(e_mk != GEX_MK_INVALID); // Caller should have already checked user args

  gasneti_MK_t i_mk = gasneti_import_mk_nonhost(e_mk);

  if (i_mk->_client != i_client) {
    gasneti_fatalerror("Invalid call to gex_Segment_Create with a gex_MK_t from a different client");
  }

  // Class-specific hook, if any
  if (MK_IMPL(i_mk,segment_create)) {
    int rc = MK_IMPL(i_mk,segment_create)(i_segment_p, i_mk, addr, size, flags);
    if (rc) return rc;
  } else {
    GASNETI_RETURN_ERRR(BAD_ARG,"gex_Segment_Create() called on unsupported memory kind");
  }

  gasneti_weakatomic32_increment(&i_mk->_ref_count, 0);
  return GASNET_OK;
}
