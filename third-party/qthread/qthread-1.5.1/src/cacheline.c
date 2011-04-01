#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <qthread/cacheline.h>
#include <qthread/common.h>
//#define DEBUG_CPUID 1

#ifdef DEBUG_CPUID
#include <stdio.h>
#endif

enum vendor
{ AMD, Intel, Unknown };
static int cacheline_bytes = 0;

#define MAX(a,b) (((a)>(b))?(a):(b))

#if ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32) || \
     (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)) && \
    defined(HAVE_GCC_INLINE_ASSEMBLY)
static void cpuid(const unsigned int op, unsigned int *eax_ptr, unsigned int *ebx_ptr,
		  unsigned int *ecx_ptr, unsigned int *edx_ptr)
{				       /*{{{ */
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32) && defined(__PIC__)
    unsigned int eax, ebx, ecx, edx;
    unsigned int pic_ebx = 0;
    __asm__ __volatile__("mov %%ebx, %4\n\t"
			 "cpuid\n\t"
			 "mov %%ebx, %1\n\t"
			 "mov %4, %%ebx"
			 :"=a"(eax), "=m"(ebx), "=c"(ecx),
			 "=d"(edx), "=m"(pic_ebx)
			 :"a"(op), "m"(pic_ebx));
    *eax_ptr = eax;
    *ebx_ptr = ebx;
    *ecx_ptr = ecx;
    *edx_ptr = edx;
# else
    __asm__ __volatile__("cpuid":"=a"(*eax_ptr), "=b"(*ebx_ptr), "=c"(*ecx_ptr), "=d"(*edx_ptr)
			 :"a"    (op));
# endif
}				       /*}}} */

static void cpuid4(const int cache, unsigned int *eax_ptr, unsigned int *ebx_ptr, unsigned int *ecx_ptr,
		   unsigned int *edx_ptr)
{				       /*{{{ */
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32) && defined(__PIC__)
    unsigned int eax, ebx, ecx, edx;
    unsigned int pic_ebx = 0;
    __asm__ __volatile__("mov %%ebx, %4\n\t"
			 "cpuid\n\t"
			 "mov %%ebx, %1\n\t"
			 "mov %4, %%ebx"
			 :"=a"(eax), "=m"(ebx), "=c"(ecx),
			 "=d"(edx), "=m"(pic_ebx)
			 :"a"    (4), "c"(cache),
			  "m"(pic_ebx));
    *eax_ptr = eax;
    *ebx_ptr = ebx;
    *ecx_ptr = ecx;
    *edx_ptr = edx;
# else
    __asm__ __volatile__("cpuid":"=a"(*eax_ptr), "=b"(*ebx_ptr),
			 "=c"(*ecx_ptr), "=d"(*edx_ptr)
			 :"a"    (4), "c"(cache));
# endif
}				       /*}}} */

static void descriptor(unsigned int d)
{				       /*{{{ */
    switch (d) {
	case 0x00:
	    return;
	case 0x0a:
	case 0x0c:
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x45:
	case 0x82:
	case 0x83:
	case 0x84:
	case 0x85:
#ifdef DEBUG_CPUID
	    printf("\top 2: code %02x: 32\n", d);
#endif
	    cacheline_bytes = MAX(cacheline_bytes, 32);
	    return;
	case 0x0d:
	case 0x21:
	case 0x22:
	case 0x23:
	case 0x25:
	case 0x29:
	case 0x2c:
	case 0x39:
	case 0x3a:
	case 0x3b:
	case 0x3c:
	case 0x3d:
	case 0x3e:
	case 0x46:
	case 0x47:
	case 0x48:
	case 0x49:
	case 0x4a:
	case 0x4b:
	case 0x4c:
	case 0x4d:
	case 0x4e:
	case 0x60:
	case 0x66:
	case 0x67:
	case 0x68:
	case 0x78:
	case 0x79:
	case 0x7a:
	case 0x7b:
	case 0x7c:
	case 0x7d:
	case 0x7f:
	case 0x86:
	case 0x87:
	case 0xd0:
	case 0xd1:
	case 0xd2:
	case 0xd6:
	case 0xd7:
	case 0xd8:
	case 0xdc:
	case 0xdd:
	case 0xde:
	case 0xe2:
	case 0xe3:
	case 0xe4:
	case 0xea:
	case 0xeb:
	case 0xec:
#ifdef DEBUG_CPUID
	    printf("\top 2: code %02x: 64\n", d);
#endif
	    cacheline_bytes = MAX(cacheline_bytes, 64);
	    return;
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x06:
	case 0x08:
	case 0x09:
	case 0x30:
	case 0x40:
	case 0x50:
	case 0x51:
	case 0x52:
	case 0x55:
	case 0x56:
	case 0x57:
	case 0x5a:
	case 0x5b:
	case 0x5c:
	case 0x5d:
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
	case 0xb0:
	case 0xb1:
	case 0xb2:
	case 0xb3:
	case 0xb4:
	case 0xca:
	case 0xf0:
	case 0xf1:
	    return;
	default:		       /*printf("no idea: %02x\n", d); */
	    return;
    }
}				       /*}}} */

static void examine(unsigned int r)
{				       /*{{{ */
    if ((r & 0x40000000) == 0) {
	descriptor((r >> 0) & 0xff);
	descriptor((r >> 8) & 0xff);
	descriptor((r >> 16) & 0xff);
	descriptor((r >> 24) & 0xff);
    }
}				       /*}}} */
#endif

static void figure_out_cacheline_size(void)
{				       /*{{{ */
#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    if (sizeof(long) == 4) {
	cacheline_bytes = 32;	       // G4
    } else {
	cacheline_bytes = 128;	       // G5
    }
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32) || \
      (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    cacheline_bytes = 128;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
# ifdef DEBUG_CPUID
    printf("IA64 does not support CPUID; but is usually 128\n");
# endif
    cacheline_bytes = 128;	       // Itanium L2/L3 are 128, L1 is 64
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_TILE)
    cacheline_bytes = 64;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32) || \
      (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
# if !defined(HAVE_GCC_INLINE_ASSEMBLY)
    cacheline_bytes = 128;
# else
    unsigned int eax, ebx, ecx, edx;
    enum vendor v;
    int tmp = 0;
    int largest_ext = 0;
    int largest_std = 0;

    cpuid(0, &eax, &ebx, &ecx, &edx);
    if (ebx == 0x756e6547 && edx == 0x49656e69 && ecx == 0x6c65746e) {
	largest_std = eax;
	v = Intel;
#ifdef DEBUG_CPUID
	printf("GenuineIntel (%i max)\n", largest_std);
#endif
    } else if (ebx == 0x68747541 && ecx == 0x444d4163 && edx == 0x69746e65) {
	largest_std = eax;
	v = AMD;
#ifdef DEBUG_CPUID
	printf("AuthenticAMD (%i max)\n", largest_std);
#endif
    } else {
	v = Unknown;
	largest_std = eax;
#ifdef DEBUG_CPUID
	printf("Unknown Vendor: %x %x %x %x\n", eax, ebx, ecx, edx);
	printf("\tvendor string: %c%c%c%c%c%c%c%c%c%c%c%c\n",
		ebx&0xff, (ebx>>8)&0xff, (ebx>>16)&0xff, (ebx>>24)&0xff,
		edx&0xff, (edx>>8)&0xff, (edx>>16)&0xff, (edx>>24)&0xff,
		ecx&0xff, (ecx>>8)&0xff, (ecx>>16)&0xff, (ecx>>24)&0xff);
#endif
    }

    if (v == AMD && largest_std >= 1) {
	cpuid(1, &eax, &ebx, &ecx, &edx);
	tmp = 8 * ((ebx >> 8) & 0xff); // The clflush width
#ifdef DEBUG_CPUID
	printf("clflush width: %i\n", tmp);
#endif
	cacheline_bytes = MAX(cacheline_bytes, tmp);
    }
    if (v == Intel) {
	if (largest_std >= 2) {
	    int i = 1;
	    int limit;

	    do {
		cpuid(2, &eax, &ebx, &ecx, &edx);
		limit = eax & 0xf;
		i++;
		examine(eax);
		examine(ebx);
		examine(ecx);
		examine(edx);
	    } while (i < limit);
	}

	if (largest_std >= 4) {
	    int cache = 0;

	    // Deterministic cache parameters
	    cpuid4(cache, &eax, &ebx, &ecx, &edx);
	    while ((eax & 0x1f) != 0) {
		tmp = (ebx & 0xfff) + 1;
#ifdef DEBUG_CPUID
		printf("L%i %s System Coherency Line Size: %i\n",
		       (eax >> 5) & 0x7, ((eax&0x1f)==1)?"DCache":(((eax&0x1f)==2)?"ICache":(((eax&0x1f)==3)?"UCache":"NULL")), tmp);
		if (ebx == 0) {
		    printf("\teax:%x ebx:%x ecx:%x edx:%x\n", eax, ebx, ecx, edx);
		    printf("\tI suspect this is a VirtualBox bug.\n");
		}
#endif
		cacheline_bytes = MAX(cacheline_bytes, tmp);
		cpuid4(++cache, &eax, &ebx, &ecx, &edx);
	    }
	    if (cache > 0 && cacheline_bytes > 0)
		return;
	}
    }
    cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
#ifdef DEBUG_CPUID
    printf("largest ext = %x\n", eax);
#endif
    largest_ext = eax;
    if (v == AMD && largest_ext >= 0x80000005) {
	cpuid(0x80000005, &eax, &ebx, &ecx, &edx);
	tmp = (ecx >> 8) & 0xff;
#ifdef DEBUG_CPUID
	printf("L1 cache line size: %i\n", tmp);
#endif
	cacheline_bytes = MAX(cacheline_bytes, tmp);
    }
    if ((v == AMD || v == Intel) && largest_ext >= 0x80000006) {
	cpuid(0x80000006, &eax, &ebx, &ecx, &edx);
	tmp = ecx & 0xff;
#ifdef DEBUG_CPUID
	printf("L2 cache line size: %i\n", tmp);
#endif
	cacheline_bytes = MAX(cacheline_bytes, tmp);
	if (v == AMD) {
	    tmp = edx & 0xff;
#ifdef DEBUG_CPUID
	    printf("L3 cache line size: %i\n", tmp);
#endif
	    cacheline_bytes = MAX(cacheline_bytes, tmp);
	}
    }
# endif
#else
    cacheline_bytes = 128;	       // safe default, probably not accurate
#endif
}				       /*}}} */

/* returns the cache line size */
int qthread_cacheline(void)
{				       /*{{{ */
    if (cacheline_bytes == 0) {
	figure_out_cacheline_size();
	if (cacheline_bytes == 0) {    /* to cache errors in cacheline detection */
	    cacheline_bytes = 128;
	}
    }
    return cacheline_bytes;
}				       /*}}} */

#ifdef DEBUG_CPUID
int main()
{
    int cl = qthread_cacheline();

    printf("Hello! Cacheline: %i bytes\n", cl);

    return 0;
}
#endif
