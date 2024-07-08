/*
 * Copyright (C) 2023 Cornelis Networks.
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

#ifdef OPX_SIM_ENABLED
/* Build simulator support */

#ifndef NDEBUG
static inline void OPX_SIM_DEBUG_FPRINTF(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	return;
}
#else
static inline void OPX_SIM_DEBUG_FPRINTF(const char *format, ...)
{
	return;
}
#endif

__OPX_FORCE_INLINE__
void opx_sim_store(uint64_t  offset, uint64_t *value) {
	long ret, loffset = (long) offset;
	OPX_SIM_DEBUG_FPRINTF("FI_OPX_HFI1_BAR_STORE: offset %#16.16lX\n", loffset);
	ret = lseek(fi_opx_global.hfi_local_info.sim_fd, offset, SEEK_SET);
	if (ret != loffset) {
		perror("FI_OPX_HFI1_BAR_STORE: Unable to lseek BAR: ");
		sleep(5); abort();
	}
	OPX_SIM_DEBUG_FPRINTF("FI_OPX_HFI1_BAR_STORE: value %#16.16lX\n", *value);
	if (write(fi_opx_global.hfi_local_info.sim_fd, value, sizeof(*value)) < 0) {
		perror("FI_OPX_HFI1_BAR_STORE: Unable to write BAR: ");
		sleep(5); abort();
	}
	return;
}

__OPX_FORCE_INLINE__
uint64_t opx_sim_load(uint64_t  offset) {
	uint64_t value;
	long ret, loffset = (long) offset;
	OPX_SIM_DEBUG_FPRINTF("FI_OPX_HFI1_BAR_LOAD: offset %#16.16lX\n", loffset);
	ret = lseek(fi_opx_global.hfi_local_info.sim_fd, offset, SEEK_SET);
	if (ret != loffset) {
		perror("FI_OPX_HFI1_BAR_LOAD: Unable to lseek BAR: ");
		sleep(5); abort();
	}
	if (read(fi_opx_global.hfi_local_info.sim_fd, &value, sizeof(value)) < 0) {
		perror("FI_OPX_HFI1_BAR_LOAD: Unable to read BAR: ");
		sleep(5); abort();
	}
	OPX_SIM_DEBUG_FPRINTF("FI_OPX_HFI1_BAR_LOAD: value %#16.16lX\n", value);
	return value;
}

#define OPX_OPEN_BAR(unit) opx_open_sim_bar(unit)

__OPX_FORCE_INLINE__
void opx_open_sim_bar (unsigned unit)
{
	const char *sim_barfiles[2] = {
		/* Typical sim bar files */
		"/sys/devices/pcif00f:00/f00f:00:00.0/resource0", /* hfi_0 */
		"/sys/devices/pcif00f:00/f00f:00:01.0/resource0", /* hfi_1 */
	};
	assert(unit < 2);

	const char* filename = sim_barfiles[unit];
#if (!defined(OPX_WFR) && !defined(OPX_JKR))
	fprintf(stderr, "Simulator MUST be built with OPX_WFR or OPX_JKR\n");
	abort();
#endif
	fi_opx_global.hfi_local_info.sim_fd = open(filename, O_RDWR);
	if (fi_opx_global.hfi_local_info.sim_fd < 0) {
		perror("fi_opx_sim_open_bar Unable to open BAR\n");
		sleep(5); abort();
	}
}

#define OPX_HFI1_BAR_STORE(bar, value)                                    \
do {                                                                      \
        uint64_t __uint64_value = value;                                  \
        opx_sim_store((uint64_t)bar, &__uint64_value);                    \
} while (false)

#define OPX_HFI1_BAR_LOAD(bar) opx_sim_load((uint64_t)bar)

#define OPX_TXE_PIO_SEND ((uint64_t)0x2000000)

#define OPX_JKR_RXE_PER_CONTEXT_OFFSET ((uint64_t)0x1600000)
#define OPX_WFR_RXE_PER_CONTEXT_OFFSET ((uint64_t)0x1300000)

#define OPX_JKR_RXE_UCTX_STRIDE ((uint64_t)8*1024)
#define OPX_WFR_RXE_UCTX_STRIDE ((uint64_t)4*1024)

#define OPX_HFI1_INIT_PIO_SOP(context, input) ({		          \
volatile uint64_t  * __pio_sop;                                           \
do {                                                                      \
	      if(OPX_HFI1_TYPE == OPX_HFI1_WFR) {    	                  \
                       __pio_sop = (uint64_t  *)		          \
	                           (OPX_TXE_PIO_SEND +                    \
				    (context * (64*1024L)) +              \
				    (16*1024*1024L));   	          \
              } else {  					          \
		      __pio_sop = (uint64_t  *) 		          \
				  (OPX_TXE_PIO_SEND +                     \
				   (context * (64*1024L)) +               \
				   (16*1024*1024L));    	          \
              } 						          \
} while (false);                                                          \
__pio_sop;                                                                \
})

#define OPX_HFI1_INIT_PIO(context, input) ({		                  \
volatile uint64_t  * __pio;                                               \
do {                                                                      \
        if(OPX_HFI1_TYPE == OPX_HFI1_WFR) {    	                          \
                __pio = (uint64_t  *)(OPX_TXE_PIO_SEND +                  \
				     (context * (64*1024L)));   	  \
        } else {  					                  \
		__pio = (uint64_t  *)(OPX_TXE_PIO_SEND +                  \
				     (context * (64*1024L)));    	  \
        } 						                  \
} while (false);                                                          \
__pio;                                                                    \
})


#define OPX_HFI1_INIT_UREGS(context, input) ({		                  \
volatile uint64_t  * __uregs;                                             \
do {                                                                      \
        if(OPX_HFI1_TYPE == OPX_HFI1_WFR) {    	                          \
                __uregs = (uint64_t*)(OPX_WFR_RXE_PER_CONTEXT_OFFSET +    \
				((context) * OPX_WFR_RXE_UCTX_STRIDE));   \
        } else {  					                  \
		__uregs = (uint64_t*)(OPX_JKR_RXE_PER_CONTEXT_OFFSET +    \
				 ((context) * OPX_JKR_RXE_UCTX_STRIDE));  \
        } 						                  \
} while (false);                                                          \
__uregs;                                                                  \
})

#else
/* Build only "real" HFI1 support (default) */

#define OPX_OPEN_BAR(unit) fi_opx_global.hfi_local_info.sim_fd = -1

#define OPX_HFI1_BAR_STORE(bar, _value) *bar = _value

#define OPX_HFI1_BAR_LOAD(bar)  *(volatile uint64_t *)bar

#define OPX_TXE_PIO_SEND ((uint64_t)0x2000000)

#define OPX_JKR_RXE_PER_CONTEXT_OFFSET ((uint64_t)0x1600000)
#define OPX_WFR_RXE_PER_CONTEXT_OFFSET ((uint64_t)0x1300000)

#define OPX_JKR_RXE_UCTX_STRIDE ((uint64_t)8*1024)
#define OPX_WFR_RXE_UCTX_STRIDE ((uint64_t)4*1024)

#define OPX_HFI1_INIT_PIO_SOP(context, input) input

#define OPX_HFI1_INIT_PIO(context, input)  input


#define OPX_HFI1_INIT_UREGS(context, input)  input

#endif

#endif
