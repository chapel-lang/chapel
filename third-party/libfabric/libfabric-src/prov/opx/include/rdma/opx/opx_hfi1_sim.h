/*
 * Copyright (C) 2023-2025 Cornelis Networks.
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
#ifndef _OPX_HFI1_SIM_H_
#define _OPX_HFI1_SIM_H_

/**********************************************************************
 * Macro and function support for the simulator as well as WFR and JKR.
 *
 * OPX_HFI1_BAR_STORE and OPX_HFI1_BAR_LOAD should be used for PCIe
 * device memory STORE and LOAD (scb, pio, pio_sop, and the uregs)
 *
 * Do not use these macros on regular memory maps. They will fail
 * on the simulator.
 *********************************************************************/

#ifdef OPX_SIM_ENABLED
/* Build simulator support */

__OPX_FORCE_INLINE__
void opx_sim_pio_store(uint64_t offset, uint64_t *value, const char *func, const int line)
{
	long ret, loffset = (long) offset;
	ret = lseek(fi_opx_global.hfi_local_info.sim_sctxt_fd, offset, SEEK_SET);
	if (ret != loffset) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s:%u FI_OPX_HFI1_BAR_STORE: offset %#16.16lX\n",
			     func, line, offset);
		perror("FI_OPX_HFI1_BAR_STORE: Unable to lseek BAR: ");
		abort();
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s:%u FI_OPX_HFI1_BAR_STORE: %#16.16lX value [%#16.16lX]\n",
		     func, line, offset, *value);
	if (write(fi_opx_global.hfi_local_info.sim_sctxt_fd, value, sizeof(*value)) < 0) {
		perror("FI_OPX_HFI1_BAR_STORE: Unable to write BAR: ");
		abort();
	}
	return;
}

__OPX_FORCE_INLINE__
void opx_sim_ureg_store(uint64_t offset, uint64_t *value, const char *func, const int line)
{
	long ret, loffset = (long) offset;
	ret = lseek(fi_opx_global.hfi_local_info.sim_rctxt_fd, offset, SEEK_SET);
	if (ret != loffset) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s:%u FI_OPX_HFI1_BAR_STORE: offset %#16.16lX\n",
			     func, line, offset);
		perror("FI_OPX_HFI1_BAR_STORE: Unable to lseek BAR: ");
		abort();
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s:%u FI_OPX_HFI1_BAR_STORE: %#16.16lX value [%#16.16lX]\n",
		     func, line, offset, *value);
	if (write(fi_opx_global.hfi_local_info.sim_rctxt_fd, value, sizeof(*value)) < 0) {
		perror("FI_OPX_HFI1_BAR_STORE: Unable to write BAR: ");
		abort();
	}
	return;
}

__OPX_FORCE_INLINE__
uint64_t opx_sim_ureg_load(uint64_t offset)
{
	uint64_t value;
	long	 ret, loffset = (long) offset;
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_HFI1_BAR_LOAD: offset %#16.16lX\n", loffset);
	ret = lseek(fi_opx_global.hfi_local_info.sim_rctxt_fd, offset, SEEK_SET);
	if (ret != loffset) {
		perror("FI_OPX_HFI1_BAR_LOAD: Unable to lseek BAR: ");
		abort();
	}
	if (read(fi_opx_global.hfi_local_info.sim_rctxt_fd, &value, sizeof(value)) < 0) {
		perror("FI_OPX_HFI1_BAR_LOAD: Unable to read BAR: ");
		abort();
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_HFI1_BAR_LOAD: value %#16.16lX\n", value);
	return value;
}

#define OPX_OPEN_BAR(unit, rcontext, scontext) opx_open_sim_bar(unit, rcontext, scontext)

__OPX_FORCE_INLINE__
void opx_open_sim_bar(unsigned unit, unsigned short int rcontext, unsigned short int scontext)
{
	static const char *sim_barfiles[] = {
		/* Typical sim bar files */
		"/sys/devices/pcif00f:00/f00f:00:00.0/resource0", /* hfi_0 */
		"/sys/devices/pcif00f:00/f00f:00:01.0/resource0", /* hfi_1 */
		"/sys/devices/f00f:01:00.0/resource0",		  /* hfi_0 updated simpci */
		"/sys/devices/f00f:02:00.0/resource0"		  /* hfi_1 updated simpci */
	};
	static bool once_only = false;
	if (once_only) {
		/* Move globals to per-endpoint if we need to test > 1 endpoint in simulation */
		fprintf(stderr, "SIMULATION only supports one endpoint\n");
		abort();
	}
	once_only = true;

	/* CN6000 contexts are split across BARs depending on their value:
	   0-255 in BAR0/resource0, 256-511 in BAR1/resource2, 512-767 in BAR2/resource4*/

	char *BAR[3] = {"resource0", "resource2", "resource4"};

	{ /* Send context BAR (PIO) */
		const char *resource = BAR[(scontext >> 8) & 0x3];
		const char *filename = NULL;
		static char filename_storage[256];

		if (getenv("HFI_FNAME")) {
			/* Arbitrary user specified file name*/
			filename = getenv("HFI_FNAME");
		} else if (getenv("FI_OPX_SIMPCI_V")) {
			/* Old "standard" file names */
			assert(unit < 2); /* simulation limit for this option */
			int v = atoi(getenv("FI_OPX_SIMPCI_V"));
			assert((v == 0) || (v == 1));
			if (v) {
				unit += 2;
			}
			filename = sim_barfiles[unit];
		} else {
			/* Calculate new expected path/filename */
			snprintf(filename_storage, sizeof(filename_storage), "%s_%d/device/%s", OPX_CLASS_PATH, unit,
				 resource);
			filename = filename_storage;
		}

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Open (context %u) %s\n", scontext, filename);

		fi_opx_global.hfi_local_info.sim_sctxt_fd = open(filename, O_RDWR);
		if (fi_opx_global.hfi_local_info.sim_sctxt_fd < 0) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "HFI_FNAME %s: filename %s\n", getenv("HFI_FNAME"),
				filename);
			perror("fi_opx_sim_open_bar Unable to open BAR\n");
			abort();
		}
	}

	{ /* Receive context BAR (UREGS) */
		const char *resource = BAR[(rcontext >> 8) & 0x3];
		const char *filename = NULL;
		static char filename_storage[256];

		if (getenv("HFI_FNAME")) {
			/* Arbitrary user specified file name*/
			filename = getenv("HFI_FNAME");
		} else if (getenv("FI_OPX_SIMPCI_V")) {
			/* Old "standard" file names */
			assert(unit < 2); /* simulation limit for this option */
			int v = atoi(getenv("FI_OPX_SIMPCI_V"));
			assert((v == 0) || (v == 1));
			if (v) {
				unit += 2;
			}
			filename = sim_barfiles[unit];
		} else {
			/* Calculate new expected path/filename */
			snprintf(filename_storage, sizeof(filename_storage), "/sys/class/infiniband/hfi1_%d/device/%s",
				 unit, resource);
			filename = filename_storage;
		}

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Open (context %u) %s\n", rcontext, filename);

		fi_opx_global.hfi_local_info.sim_rctxt_fd = open(filename, O_RDWR);
		if (fi_opx_global.hfi_local_info.sim_rctxt_fd < 0) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "HFI_FNAME %s: filename %s\n", getenv("HFI_FNAME"),
				filename);
			perror("fi_opx_sim_open_bar Unable to open BAR\n");
			abort();
		}
	}
}

#define OPX_HFI1_BAR_PIO_STORE(bar, value)                                      \
	do {                                                                    \
		uint64_t _value = value;                                        \
		opx_sim_pio_store((uint64_t) bar, &_value, __func__, __LINE__); \
	} while (false)

#define OPX_HFI1_BAR_UREG_STORE(bar, value)                                      \
	do {                                                                     \
		uint64_t _value = value;                                         \
		opx_sim_ureg_store((uint64_t) bar, &_value, __func__, __LINE__); \
	} while (false)

#define OPX_HFI1_BAR_UREG_LOAD(bar) opx_sim_ureg_load((uint64_t) bar)

#define OPX_TXE_PIO_SEND ((uint64_t) 0x2000000)

#define OPX_JKR_RXE_PER_CONTEXT_OFFSET ((uint64_t) 0x1600000)
#define OPX_WFR_RXE_PER_CONTEXT_OFFSET ((uint64_t) 0x1300000)

#define OPX_JKR_RXE_UCTX_STRIDE ((uint64_t) 8 * 1024)
#define OPX_WFR_RXE_UCTX_STRIDE ((uint64_t) 4 * 1024)

/* CYR constants match JKR but they are per BAR (opx_open_sim_bar) and should be masked to 8 bits.
   WFR and JKR are already <= 8 bits */
#define OPX_HFI1_INIT_PIO_SOP(context, input)                                                                   \
	({                                                                                                      \
		volatile uint64_t *__pio_sop;                                                                   \
		const short int	   per_bar_context = context & 0xFF;                                            \
		do {                                                                                            \
			if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {                                                     \
				__pio_sop = (uint64_t *) (OPX_TXE_PIO_SEND + (context * (64 * 1024L)) +         \
							  (16 * 1024 * 1024L));                                 \
			} else {                                                                                \
				__pio_sop = (uint64_t *) (OPX_TXE_PIO_SEND + (per_bar_context * (64 * 1024L)) + \
							  (16 * 1024 * 1024L));                                 \
			}                                                                                       \
		} while (false);                                                                                \
		__pio_sop;                                                                                      \
	})

#define OPX_HFI1_INIT_PIO(context, input)                                                                   \
	({                                                                                                  \
		volatile uint64_t *__pio;                                                                   \
		const short int	   per_bar_context = context & 0xFF;                                        \
		do {                                                                                        \
			if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {                                                 \
				__pio = (uint64_t *) (OPX_TXE_PIO_SEND + (context * (64 * 1024L)));         \
			} else {                                                                            \
				__pio = (uint64_t *) (OPX_TXE_PIO_SEND + (per_bar_context * (64 * 1024L))); \
			}                                                                                   \
		} while (false);                                                                            \
		__pio;                                                                                      \
	})

#define OPX_HFI1_INIT_UREGS(context, input)                                                             \
	({                                                                                              \
		volatile uint64_t *__uregs;                                                             \
		const short int	   per_bar_context = context & 0xFF;                                    \
		do {                                                                                    \
			if (OPX_HFI1_TYPE & OPX_HFI1_WFR) {                                             \
				__uregs = (uint64_t *) (OPX_WFR_RXE_PER_CONTEXT_OFFSET +                \
							((context) * OPX_WFR_RXE_UCTX_STRIDE));         \
			} else {                                                                        \
				__uregs = (uint64_t *) (OPX_JKR_RXE_PER_CONTEXT_OFFSET +                \
							((per_bar_context) * OPX_JKR_RXE_UCTX_STRIDE)); \
			}                                                                               \
		} while (false);                                                                        \
		__uregs;                                                                                \
	})

#else
/* Build only "real" HFI1 support (default) */

#define OPX_OPEN_BAR(unit, rcontext, scontext) \
	fi_opx_global.hfi_local_info.sim_rctxt_fd = fi_opx_global.hfi_local_info.sim_sctxt_fd = -1

#if !defined(NDEBUG) && defined(OPX_DEBUG_VERBOSE)
#define OPX_HFI1_BAR_UREG_LOAD(bar)                                                                                  \
	({                                                                                                           \
		volatile uint64_t _value = *(volatile uint64_t *) bar;                                               \
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s:%u FI_OPX_HFI1_BAR_LOAD: offset %#16.16lX\n",   \
			     __func__, __LINE__, (uint64_t) (bar));                                                  \
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_HFI1_BAR_LOAD: value %#16.16lX\n", _value); \
		_value;                                                                                              \
	})

#define OPX_HFI1_BAR_PIO_STORE(bar, value)                                                                          \
	do {                                                                                                        \
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s:%u FI_OPX_HFI1_BAR_STORE: offset %#16.16lX\n", \
			     __func__, __LINE__, (uint64_t) (bar));                                                 \
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_HFI1_BAR_STORE: value %#16.16lX\n",        \
			     (uint64_t) value);                                                                     \
		*(volatile uint64_t *) bar = (uint64_t) value;                                                      \
	} while (false)

#define OPX_HFI1_BAR_UREG_STORE(bar, value)                                                                         \
	do {                                                                                                        \
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s:%u FI_OPX_HFI1_BAR_STORE: offset %#16.16lX\n", \
			     __func__, __LINE__, (uint64_t) (bar));                                                 \
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_HFI1_BAR_STORE: value %#16.16lX\n",        \
			     (uint64_t) value);                                                                     \
		*(volatile uint64_t *) bar = (uint64_t) value;                                                      \
	} while (false)

#else

#define OPX_HFI1_BAR_PIO_STORE(bar, value)  *(volatile uint64_t *) bar = (uint64_t) value;
#define OPX_HFI1_BAR_UREG_STORE(bar, value) *(volatile uint64_t *) bar = (uint64_t) value;

#define OPX_HFI1_BAR_UREG_LOAD(bar) *(volatile uint64_t *) bar

#endif

#define OPX_TXE_PIO_SEND ((uint64_t) 0x2000000)

#define OPX_JKR_RXE_PER_CONTEXT_OFFSET ((uint64_t) 0x1600000)
#define OPX_WFR_RXE_PER_CONTEXT_OFFSET ((uint64_t) 0x1300000)

#define OPX_JKR_RXE_UCTX_STRIDE ((uint64_t) 8 * 1024)
#define OPX_WFR_RXE_UCTX_STRIDE ((uint64_t) 4 * 1024)

#define OPX_HFI1_INIT_PIO_SOP(context, input) input

#define OPX_HFI1_INIT_PIO(context, input) input

#define OPX_HFI1_INIT_UREGS(context, input) input

#endif

#endif
