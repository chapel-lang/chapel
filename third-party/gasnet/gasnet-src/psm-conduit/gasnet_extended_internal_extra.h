/*
 * Description: PSM extended API Op management
 * Copyright (c) 2015 Intel Corporation. All rights reserved.
 * Terms of use are as specified in license.txt
 */


#ifndef _GASNET_EXTENDED_EXTRA_H
#define _GASNET_EXTENDED_EXTRA_H

/* -------------------------------------------------------------------------- */

#if GASNETE_EOP_COUNTED && !GASNETE_AMREF_USE_MARKDONE
  #define PSM_PACK_EOP_DONE(_eop)         (&(_eop)->completed_cnt)
  #define PSM_PACK_IOP_DONE(_iop,_getput) (&(_iop)->completed_##_getput##_cnt)
  #define PSM_MARK_DONE(_ptr,_isget)      gasneti_weakatomic_increment((gasneti_weakatomic_t *)(_ptr), \
                                                                   (_isget) ? GASNETI_ATOMIC_REL : 0)
#else
  #define PSM_PACK_EOP_DONE(_eop)         (_eop)
  #define PSM_PACK_IOP_DONE(_iop,_getput) (_iop)
  #define PSM_MARK_DONE(_ptr,_isget)      do {                                                      \
                                         if (_isget) gasneti_sync_writes();                     \
                                         gasnete_op_markdone((gasnete_op_t *)(_ptr), (_isget)); \
                                      } while (0)
#endif

/*
 * Internal CPUID detection
 */
#define CPUID_FAMILY_MASK       0x00000f00
#define CPUID_MODEL_MASK        0x000000f0
#define CPUID_EXMODEL_MASK      0x000f0000

/*
 * CPUID return values
 */
#define CPUID_FAMILY_XEON       0x00000600
#define CPUID_MODEL_PHI_GEN2    0x57
/*
 * cpuid function 0, returns "GeniuneIntel" in EBX,ECX,EDX
 * due to Little Endian and Hex it is not so obvious
 */
#define CPUID_GENUINE_INTEL_EBX 0x756e6547 /* "uneG" - Little Endian "Genu" */
#define CPUID_GENUINE_INTEL_ECX 0x6c65746e /* "Ieni" - Little Endian "ineI" */
#define CPUID_GENUINE_INTEL_EDX 0x49656e69 /* "letn" - Little Endian "ntel" */

/*
 * These values are internal only, not real register values
 */
#define CPUID_GENUINE_INTEL     0xf0000000
#define CPUID_MODEL_UNDEFINED   -1
#define DEFAULT_PSM2_MQ_RNDV_HFI_WINDOW_SIZE 2097152

typedef struct cpuid {
    unsigned eax, ebx, ecx, edx;
} cpuid_t;

GASNETI_INLINE(get_cpuid)
void
get_cpuid(const unsigned func, const unsigned subfunc, cpuid_t *id)
{
    unsigned a = 0;
    unsigned b = 0;
    unsigned c = 0;
    unsigned d = 0;

#if GASNETI_HAVE_GCC_ASM
    __asm__ (" \
                mov %4, %%eax \n\
                mov %5, %%ecx \n\
                cpuid \n\
                mov %%eax, %0 \n\
                mov %%ebx, %1 \n\
                mov %%ecx, %2 \n\
                mov %%edx, %3 \n\
                " : "=g" (a), "=g" (b), "=g" (c), "=g" (d)
                : "g" (func), "g" (subfunc)
                : "%eax", "%ebx", "%ecx", "%edx"
            );
#endif

    id->eax = a;
    id->ebx = b;
    id->ecx = c;
    id->edx = d;
}


void gasnete_op_markdone(gasnete_op_t *op, int isget);

#endif
