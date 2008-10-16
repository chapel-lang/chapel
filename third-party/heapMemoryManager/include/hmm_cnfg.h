/* This code is in the public domain.
** Version: 1.1  Author: Walt Karas
*/

/* Configure Heap Memory Manager for processor architecture, compiler,
** and desired performance characteristics.  This file is included
** by heapmm.h, so these definitions can be used by code external to
** HMM.  You can change the default configuration, and/or create alternate
** configuration(s).
*/

/* To allow for multiple configurations of HMM to be used in the same
** compilation unit, undefine all preprocessor symbols that will be
** defined below.
*/
#undef HMM_ADDR_ALIGN_UNIT
#undef HMM_BLOCK_ALIGN_UNIT
#undef HMM_UNIQUE
#undef HMM_DESC_PARAM
#undef HMM__SYM_TO_STRING
#undef HMM_SYM_TO_STRING
#undef HMM_AUDIT_FAIL

/* Turn X into a string after one macro expansion pass of X.  This trick
** works with both GCC and Visual C++. */
#define HMM_SYM_TO_STRING(X) HMM__SYM_TO_STRING(X)
#define HMM__SYM_TO_STRING(X) #X

#ifndef HMM_CNFG_NUM

/* Default configuration. */

/* Use hmm_ prefix to avoid identifier conflicts. */
#define HMM_UNIQUE(BASE) hmm_ ## BASE

/* Number of bytes in an Address Alignment Unit (AAU). */
#define HMM_ADDR_ALIGN_UNIT sizeof(int) 

/* Number of AAUs in a Block Alignment Unit (BAU). */
#define HMM_BLOCK_ALIGN_UNIT 1

/* Type of unsigned integer big enough to hold the size of a Block in AAUs. */
typedef unsigned long HMM_UNIQUE(size_aau);

/* Type of unsigned integer big enough to hold the size of a Block/Chunk
** in BAUs.  The high bit will be robbed. */
typedef unsigned long HMM_UNIQUE(size_bau);

void HMM_dflt_abort(const char *, const char *);

/* Actions upon a self-audit failure.  Must expand to a single complete
** statement.  If you remove the definition of this macro, no self-auditing
** will be performed. */
#define HMM_AUDIT_FAIL \
  HMM_dflt_abort(__FILE__, HMM_SYM_TO_STRING(__LINE__));

#elif HMM_CNFG_NUM == 0

/* Definitions for testing. */

#define HMM_UNIQUE(BASE) thmm_ ## BASE

#define HMM_ADDR_ALIGN_UNIT sizeof(int) 

#define HMM_BLOCK_ALIGN_UNIT 3

typedef unsigned HMM_UNIQUE(size_aau);

typedef unsigned short HMM_UNIQUE(size_bau);

/* Under this test setup, a long jump is done if there is a self-audit
** failure.
*/

extern jmp_buf HMM_UNIQUE(jmp_buf);
extern const char * HMM_UNIQUE(fail_file);
extern unsigned HMM_UNIQUE(fail_line);

#define HMM_AUDIT_FAIL \
   { HMM_UNIQUE(fail_file) = __FILE__; HMM_UNIQUE(fail_line) = __LINE__; \
     longjmp(HMM_UNIQUE(jmp_buf), 1); }

#elif HMM_CNFG_NUM == 1

/* Put configuration 1 definitions here (if there is a configuration 1). */

#elif HMM_CNFG_NUM == 2

/* Put configuration 2 definitions here. */

#elif HMM_CNFG_NUM == 3

/* Put configuration 3 definitions here. */

#elif HMM_CNFG_NUM == 4

/* Put configuration 4 definitions here. */

#elif HMM_CNFG_NUM == 5

/* Put configuration 5 definitions here. */

#endif
