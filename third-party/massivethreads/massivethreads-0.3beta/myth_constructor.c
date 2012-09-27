#include "myth_original_lib.h"
#include "myth_init.h"
#include "myth_sched.h"
#include "myth_worker.h"
#include "myth_io.h"
#include "myth_tls.h"

#include "myth_sched_proto.h"
#include "myth_io_proto.h"
#include "myth_tls_proto.h"

#include "myth_worker_func.h"
#include "myth_io_func.h"
#include "myth_sync_func.h"
#include "myth_sched_func.h"
#include "myth_tls_func.h"

__attribute__((constructor)) static void myth_to_pthread_init(void)
{
	myth_get_original_funcs();
	myth_log_start_body();
	myth_init_body(0,0);
	myth_sched_prof_start_body();
}

__attribute__((destructor)) static void myth_to_pthread_fini(void)
{
	myth_fini_body();
	myth_free_original_funcs();
}
