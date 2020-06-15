/*
 * Copyright (c) 2015 Cray Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GNIX_PRIV_H_
#define GNIX_PRIV_H_

#include "gnix.h"

extern uint8_t precomputed_crc_results[256];

/*
 * Start of code pulled from gni_priv.h
 */
#define gni_crc_bits(data) precomputed_crc_results[(data)]

inline static uint8_t gni_memhndl_calc_crc(gni_mem_handle_t *memhndl)
{
        uint64_t qw1 = memhndl->qword1;
        uint64_t qw2 = memhndl->qword2;
        uint8_t  crc = 0;
        crc  = gni_crc_bits((qw1 ^ crc)&0xff);
        crc  = gni_crc_bits(((qw1 >> 8) ^ crc)&0xff);
        crc  = gni_crc_bits(((qw1 >> 16) ^ crc)&0xff);
        crc  = gni_crc_bits(((qw1 >> 24) ^ crc)&0xff);
        crc  = gni_crc_bits(((qw1 >> 32) ^ crc)&0xff);
        crc  = gni_crc_bits(((qw1 >> 40) ^ crc)&0xff);
        crc  = gni_crc_bits(((qw1 >> 48) ^ crc)&0xff);
        crc  = gni_crc_bits(((qw1 >> 56) ^ crc)&0xff);
        crc  = gni_crc_bits((qw2 ^ crc)&0xff);
        crc  = gni_crc_bits(((qw2 >> 8) ^ crc)&0xff);
        crc  = gni_crc_bits(((qw2 >> 16) ^ crc)&0xff);
        crc  = gni_crc_bits(((qw2 >> 24) ^ crc)&0xff);
        crc  = gni_crc_bits(((qw2 >> 32) ^ crc)&0xff);
        crc  = gni_crc_bits(((qw2 >> 40) ^ crc)&0xff);
        crc  = gni_crc_bits(((qw2 >> 48) ^ crc)&0xff);

        return crc;
}

typedef struct gni_mem_hndl_v1 {
        struct {
                uint64_t va: 52;
                uint64_t mdh: 12;
        };
        struct {
                uint64_t npages: 28;
                uint64_t pgsize: 6;
                uint64_t flags: 8;
                uint64_t unused: 14;
                uint64_t crc: 8;
        };
} gni_mem_hndl_v1_t;
typedef struct gni_mem_hndl_v2 {
        union {
                struct {
                        uint64_t va: 52;
                        uint64_t entropy: 12;
                };
                uint64_t id;
        };
        struct {
                uint64_t npages: 28;
                uint64_t pgsize: 6;
                uint64_t flags: 8;
                uint64_t mdh: 12;
                uint64_t unused: 2;
                uint64_t crc: 8;
        };
} gni_mem_hndl_v2_t;

/*************** Memory Handle ****************/
/* Flags (8 bits)*/
#define GNI_MEMHNDL_FLAG_READONLY       0x01UL /* Memory is not writable */
#define GNI_MEMHNDL_FLAG_VMDH           0x02UL /* Mapped via virtual MDH table */
#define GNI_MEMHNDL_FLAG_MRT            0x04UL /* MRT was used for mapping */
#define GNI_MEMHNDL_FLAG_GART           0x08UL /* GART was used for mapping */
#define GNI_MEMHNDL_FLAG_IOMMU          0x10UL /* IOMMU was used for mapping */
#define GNI_MEMHNDL_FLAG_PCI_IOMMU      0x20UL /* PCI IOMMU was used for mapping */
#define GNI_MEMHNDL_FLAG_CLONE          0x40UL /* Registration cloned from a master MDD */
#define GNI_MEMHNDL_FLAG_NEW_FRMT       0x80UL /* Used to support MDD sharing */
/* Memory Handle manipulations  */
#define GNI_MEMHNDL_INIT(memhndl) do {memhndl.qword1 = 0; memhndl.qword2 = 0;} while(0)
/* Support macros, 34 is the offset of the flags value */
#define GNI_MEMHNDL_NEW_FRMT(memhndl) ((memhndl.qword2 >> 34) & GNI_MEMHNDL_FLAG_NEW_FRMT)
#define GNI_MEMHNDL_FRMT_SET(memhndl, val, value)           \
        if (GNI_MEMHNDL_NEW_FRMT(memhndl)) {                \
                uint64_t tmp = value;                       \
                ((gni_mem_hndl_v2_t *)&memhndl)->val = tmp; \
        } else {                                            \
                uint64_t tmp = value;                       \
                ((gni_mem_hndl_v1_t *)&memhndl)->val = tmp; \
        }

#define GNI_MEMHNDL_FRMT_GET(memhndl, val) \
        ((uint64_t)(GNI_MEMHNDL_NEW_FRMT(memhndl) ? ((gni_mem_hndl_v2_t *)&memhndl)->val : ((gni_mem_hndl_v1_t *)&memhndl)->val))

/* Differing locations for V1 and V2 mem handles */
#define GNI_MEMHNDL_SET_VA(memhndl, value)  GNI_MEMHNDL_FRMT_SET(memhndl, va, (value) >> 12)
#define GNI_MEMHNDL_GET_VA(memhndl)         (GNI_MEMHNDL_FRMT_GET(memhndl, va) << 12)
#define GNI_MEMHNDL_SET_MDH(memhndl, value) GNI_MEMHNDL_FRMT_SET(memhndl, mdh, value)
#define GNI_MEMHNDL_GET_MDH(memhndl)        GNI_MEMHNDL_FRMT_GET(memhndl, mdh)


/* The MDH field size is the same, and there is no other define to
 * limit max MDHs in uGNI. */

#define GNI_MEMHNDL_MDH_MASK    0xFFFUL

/* From this point forward, there is no difference. We don't need the
 * inlined conditionals */

/* Number of Registered pages (1TB for 4kB pages): QWORD2[27:0] */
#define GNI_MEMHNDL_NPGS_MASK   0xFFFFFFFUL
#define GNI_MEMHNDL_SET_NPAGES(memhndl, value) memhndl.qword2 |= (value & GNI_MEMHNDL_NPGS_MASK)
/* Page size that was used to calculate the total number of pages : QWORD2[33:28] */
#define GNI_MEMHNDL_PSIZE_MASK  0x3FUL
#define GNI_MEMHNDL_SET_PAGESIZE(memhndl, value) memhndl.qword2 |= (((uint64_t)value & GNI_MEMHNDL_PSIZE_MASK) << 28)
/* Flags: QWORD2[41:34] */
#define GNI_MEMHNDL_FLAGS_MASK  0xFFUL
#define GNI_MEMHNDL_SET_FLAGS(memhndl, value) memhndl.qword2 |= ((value & GNI_MEMHNDL_FLAGS_MASK) << 34)
#define GNI_MEMHNDL_GET_FLAGS(memhndl) ((memhndl.qword2 >> 34) & GNI_MEMHNDL_FLAGS_MASK)
/* QWORD2[55:54] left blank */
/* CRC to verify integrity of the handle: QWORD2[63:56] ( Call this only after all other field are set!)*/
#define GNI_MEMHNDL_CRC_MASK 0xFFUL
#define GNI_MEMHNDL_SET_CRC(memhndl) (memhndl.qword2 |= ((uint64_t)gni_memhndl_calc_crc(&memhndl)<<56))

/*
 * End of code pulled from gni_priv.h
 */

#endif /* GNIX_PRIV_H_ */
