#define LOG_SPAWN_SEQUENTIAL_YAML(num_tasks, total_time) \
    fprintf(stdout, "Sequential spawn:\n"); \
    fprintf(stdout, "    Number of tasks: %lu\n", (unsigned long)num_tasks); \
    fprintf(stdout, "    Total time: %f\n", total_time);

#define LOG_SPAWN_PARALLEL_YAML(num_tasks, total_time) \
    fprintf(stdout, "Parallel spawn:\n"); \
    fprintf(stdout, "    Number of tasks: %lu\n", (unsigned long)num_tasks); \
    fprintf(stdout, "    Total time: %f\n", total_time);

#define LOG_FIB_YAML(n_value, result, total_time) \
    fprintf(stdout, "Fib:\n"); \
    fprintf(stdout, "    N: %lu\n", (unsigned long)n_value); \
    fprintf(stdout, "    Result: %lu\n", (unsigned long)result); \
    fprintf(stdout, "    Total time: %f\n", total_time);

#define LOG_UTS_PARAMS_YAML() \
    fprintf(stdout, "UTS:\n"); \
    fprintf(stdout, "    Parameters:\n"); \
    fprintf(stdout, "        Tree type: %d\n", tree_type); \
    fprintf(stdout, "        Tree type name: %s\n", type_names[tree_type]); \
    fprintf(stdout, "        Root bf: %.1f\n", bf_0); \
    fprintf(stdout, "        Root seed: %d\n", root_seed); \
    if ((tree_type == GEO) || (tree_type == HYBRID)) { \
        fprintf(stdout, "        Gen mx: %d\n", tree_depth); \
        fprintf(stdout, "        Shape fn: %d\n", shape_fn); \
        fprintf(stdout, "        Shape fn name: %s\n", shape_names[shape_fn]); \
    } \
    if ((tree_type == BIN) || (tree_type == HYBRID)) { \
        double q  = non_leaf_prob; \
        int    m  = non_leaf_bf; \
        double es = (1.0 / (1.0 - q * m)); \
        fprintf(stdout, "        q: %f\n", q); \
        fprintf(stdout, "        m: %d\n", m); \
        fprintf(stdout, "        E(n): %f\n", q * m); \
        fprintf(stdout, "        E(s): %.2f\n", es); \
    } \
    if (tree_type == HYBRID) { \
        fprintf(stdout, "        Root-to-depth: %d\n", (int)ceil(shift_depth * tree_depth)); \
    } \
    if (tree_type == BALANCED) { \
        fprintf(stdout, "        Gen mx: %d\n", tree_depth); \
        fprintf(stdout, "        Expected num nodes: %llu\n", (unsigned long long)((pow(bf_0, tree_depth + 1) - 1.0) / (bf_0 - 1.0))); \
        fprintf(stdout, "        Expected num leaves: %llu\n", (unsigned long long)pow(bf_0, tree_depth)); \
    } \
    fprintf(stdout, "        Max num children: %d\n", MAXNUMCHILDREN); \
    fprintf(stdout, "        Non-leaf branching factor: %d\n", non_leaf_bf); \
    fprintf(stdout, "        Non-leaf probability: %f\n", non_leaf_prob); \
    fprintf(stdout, "        Compute granularity: %d\n", num_samples);

#define LOG_UTS_RESULTS_YAML(total_num_nodes, total_time) \
    fprintf(stdout, "UTS:\n"); \
    fprintf(stdout, "    Results:\n"); \
    fprintf(stdout, "        Tree size: %lu\n", (unsigned long)total_num_nodes); \
    fprintf(stdout, "        Tree depth: %d\n", (int)tree_height); \
    fprintf(stdout, "        Num leaves: %llu\n", (unsigned long long)num_leaves); \
    fprintf(stdout, "        Exec time: %.3f\n", total_time);

#define LOG_ENV_QTHREADS_YAML() \
    fprintf(stdout, "Qthreads:\n"); \
    fprintf(stdout, "    Number of shepherds: %d\n", qthread_num_shepherds()); \
    fprintf(stdout, "    Number of workers: %d\n", qthread_num_workers()); \
    fprintf(stdout, "    Stack size: %lu\n", (unsigned long)qthread_readstate(STACK_SIZE));

#define LOG_ENV_CILK_YAML() \
    fprintf(stdout, "Cilk:\n"); \
    fprintf(stdout, "    Number of threads: %lu\n", (unsigned long)__cilkrts_get_nworkers());

#define LOG_ENV_OMP_YAML(num_threads) \
    fprintf(stdout, "OpenMP:\n"); \
    fprintf(stdout, "    Number of threads: %lu\n", (unsigned long)num_threads);

#define LOG_ENV_TBB_YAML(num_threads) \
    fprintf(stdout, "TBB:\n"); \
    fprintf(stdout, "    Number of threads: %lu\n", (unsigned long)num_threads);

