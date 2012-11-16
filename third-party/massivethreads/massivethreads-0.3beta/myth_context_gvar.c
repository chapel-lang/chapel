#include "myth_config.h"
#include "myth_context.h"

#if defined MYTH_CONTEXT_ARCH_UNIVERSAL || defined MYTH_CONTEXT_ARCH_sparc

volatile __thread myth_ctx_withcall_param g_ctx_withcall_params;

#endif
