/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gemini-conduit/gasnet_extended_help_extra.h $
 * Description: GASNet Extended gemini-specific Header
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_HELP_EXTRA_H
#define _GASNET_EXTENDED_HELP_EXTRA_H

/*
  Extensions:
  ==============================
*/

#if GASNETC_GNI_FETCHOP
/* Proof-of-concept GNI uint64_t fetch-and-op.
 * Not supported, and subject to change or removal.
 */

#define GASNETX_FETCHOP_DECLS(_name,_type)                        \
    extern void _gasnetX_fetch##_name(                            \
                _type *dest, gasnet_node_t node, _type *src,      \
                _type operand GASNETI_THREAD_FARG);               \
    extern gasnet_handle_t _gasnetX_fetch##_name##_nb(            \
                _type *dest, gasnet_node_t node, _type *src,      \
                _type operand GASNETI_THREAD_FARG)                \
                GASNETI_WARN_UNUSED_RESULT;                       \
    extern void _gasnetX_fetch##_name##_nbi(                      \
                _type *dest, gasnet_node_t node, _type *src,      \
                _type operand GASNETI_THREAD_FARG);               \
    extern _type _gasnetX_fetch##_name##_val(                     \
                gasnet_node_t node, _type *src,                   \
                _type operand GASNETI_THREAD_FARG);               \
    extern gasnet_valget_handle_t _gasnetX_fetch##_name##_nb_val( \
                gasnet_node_t node, _type *src,                   \
                _type operand GASNETI_THREAD_FARG)                \
                GASNETI_WARN_UNUSED_RESULT;

GASNETX_FETCHOP_DECLS(add_u64, uint64_t)
#define gasnetX_fetchadd_u64(dest,node,src,operand) \
        _gasnetX_fetchadd_u64(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchadd_u64_nb(dest,node,src,operand) \
        _gasnetX_fetchadd_u64_nb(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchadd_u64_nbi(dest,node,src,operand) \
        _gasnetX_fetchadd_u64_nbi(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchadd_u64_val(node,src,operand) \
        _gasnetX_fetchadd_u64_val(node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchadd_u64_nb_val(node,src,operand) \
        _gasnetX_fetchadd_u64_nb_val(node,src,operand GASNETI_THREAD_GET)

GASNETX_FETCHOP_DECLS(and_u64, uint64_t)
#define gasnetX_fetchand_u64(dest,node,src,operand) \
        _gasnetX_fetchand_u64(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchand_u64_nb(dest,node,src,operand) \
        _gasnetX_fetchand_u64_nb(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchand_u64_nbi(dest,node,src,operand) \
        _gasnetX_fetchand_u64_nbi(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchand_u64_val(node,src,operand) \
        _gasnetX_fetchand_u64_val(node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchand_u64_nb_val(node,src,operand) \
        _gasnetX_fetchand_u64_nb_val(node,src,operand GASNETI_THREAD_GET)

GASNETX_FETCHOP_DECLS(or_u64, uint64_t)
#define gasnetX_fetchor_u64(dest,node,src,operand) \
        _gasnetX_fetchor_u64(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchor_u64_nb(dest,node,src,operand) \
        _gasnetX_fetchor_u64_nb(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchor_u64_nbi(dest,node,src,operand) \
        _gasnetX_fetchor_u64_nbi(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchor_u64_val(node,src,operand) \
        _gasnetX_fetchor_u64_val(node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchor_u64_nb_val(node,src,operand) \
        _gasnetX_fetchor_u64_nb_val(node,src,operand GASNETI_THREAD_GET)

GASNETX_FETCHOP_DECLS(xor_u64, uint64_t)
#define gasnetX_fetchxor_u64(dest,node,src,operand) \
        _gasnetX_fetchxor_u64(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchxor_u64_nb(dest,node,src,operand) \
        _gasnetX_fetchxor_u64_nb(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchxor_u64_nbi(dest,node,src,operand) \
        _gasnetX_fetchxor_u64_nbi(dest,node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchxor_u64_val(node,src,operand) \
        _gasnetX_fetchxor_u64_val(node,src,operand GASNETI_THREAD_GET)
#define gasnetX_fetchxor_u64_nb_val(node,src,operand) \
        _gasnetX_fetchxor_u64_nb_val(node,src,operand GASNETI_THREAD_GET)

#endif

/* ------------------------------------------------------------------------------------ */
 

#endif
