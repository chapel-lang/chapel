
int qthread_internal_net_driver_runtime_init(void);
int qthread_internal_net_driver_runtime_fini(void);
ptl_process_t* qthread_internal_net_driver_runtime_get_mapping(void);
void qthread_internal_net_driver_runtime_barrier(void);

int qthread_internal_net_driver_get_rank(void);
int qthread_internal_net_driver_get_size(void);
