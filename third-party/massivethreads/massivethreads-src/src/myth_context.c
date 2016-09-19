/* 
 * myth_context.c
 */
#include "myth_config.h"
#include "myth_context.h"

#if MYTH_CONTEXT == MYTH_CONTEXT_UCONTEXT

volatile __thread myth_ctx_withcall_param g_ctx_withcall_params;

#endif
