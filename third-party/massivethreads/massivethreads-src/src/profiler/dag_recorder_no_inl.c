/* 
 * dag_recorder_api.c
 */

#define DAG_RECORDER 2
#define DAG_RECORDER_INLINE_INSTRUMENTATION 0
#include <dag_recorder.h>
#undef static_if_inline
#define static_if_inline
#include <dag_recorder_inl.h>

