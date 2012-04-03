#ifndef _CODELET_STARPU_H_
#define _CODELET_STARPU_H_

#include <starpu.h>

#define _codelet_init starpu_init
#define _codelet_shutdown starpu_shutdown
#define _codelet_create starpu_task_create
#define _codelet_submit_task starpu_task_submit
#define _codelet_declare_deps starpu_tag_declare_deps

#define CHPL_GPU STARPU_GPU
#define CHPL_CPU STARPU_CPU

/* File to map codelet types to their equivalent starpu types */
typedef struct starpu_conf _codelet_config;
typedef struct starpu_task _codelet_task;
typedef struct starpu_codelet _codelet;
typedef starpu_tag_t _codelet_tag_t;

#endif

