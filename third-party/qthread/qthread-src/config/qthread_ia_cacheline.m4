AC_DEFUN([QTHREAD_IA_CACHELINE],
[AC_REQUIRE([AC_PROG_CC])
AC_LANG_PUSH([C])
AC_CACHE_CHECK([for x86 cache line size],
	[qt_cv_cacheline],
	[AC_RUN_IFELSE([AC_LANG_PROGRAM([
#include <stdio.h>
/* Architecture defines */
#define QTHREAD_UNSUPPORTED 0
#define QTHREAD_IA32        1
#define QTHREAD_AMD64       2
#define QTHREAD_POWERPC32   6
#define QTHREAD_POWERPC64   7
#define QTHREAD_ARM         12
#define QTHREAD_ARMV8_A64   13
],[
int op = 1, eax, ebx, ecx, edx, cachelinesize;
FILE *f;
#if QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32
# ifdef __PIC__
__asm__("push %%ebx\n\t"
"cpuid\n\t"
"mov %%ebx, %1\n\t"
"pop %%ebx"
:"=a"(eax), "=m"(ebx), "=c"(ecx), "=d"(edx)
:"a"(op));
# else
__asm__("cpuid"
:"=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
:"a"(op));
# endif
#elif QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64
__asm__("cpuid"
:"=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
:"a"(op));
#endif
cachelinesize = 8*((ebx>>8)&0xff);
if (cachelinesize == 0) {
	op = 2;
#if QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32
__asm__("push %%ebx\n\t"
"cpuid\n\t"
"mov %%ebx, %1\n\t"
"pop %%ebx"
:"=a"(eax), "=m"(ebx), "=c"(ecx), "=d"(edx)
:"a"(op));
#elif QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64
__asm__("cpuid"
:"=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
:"a"(op));
#endif
}
f = fopen("conftest_cacheline", "w"); if (!f) return 1;
fprintf(f, "%i\n", cachelinesize);
fclose(f);
return 0;
])],
	[qt_cv_cacheline=`cat conftest_cacheline`; rm -f conftest_cacheline],
	[qt_cv_cacheline=0; rm -f conftest_cacheline],
	[qt_cv_cacheline=0])])
AC_LANG_POP([C])
])
