#include <hwloc.h>
#include <stdio.h>

#include "qt_affinity.h"
#include "qt_envariables.h"

hwloc_topology_t topology;

// Shepherd affinity 
struct {
  hwloc_cpuset_t* binds;
  int num;
} sheps;

// Worker affinity bindings, indexed by packed_worker_id
struct {
  hwloc_cpuset_t* binds;
  int num;
} workers;

int INTERNAL qt_affinity_get_unique_id(int i){
  return i;
}

void INTERNAL qt_affinity_balanced(int num_workers, 
                                   int start, 
                                   hwloc_obj_t obj)
{
  if (num_workers > 0){
    hwloc_obj_t child = obj->first_child;
    if (child) {
      // Have a child, not a leaf
      int arity = obj->arity;
      int div = num_workers / arity;
      int rem = num_workers % arity;
      int n = start;
      while (child) {
        int m = div;
        if (rem > 0) {
          m++;
          rem--;
        }
        qt_affinity_balanced(m, n, child); 
        child = child->next_sibling;
        n += m;
      }
    } else {
      // No children
      workers.binds[start] = hwloc_bitmap_alloc();
      hwloc_bitmap_copy(workers.binds[start], obj->cpuset);
      if (num_workers > 1) {
        printf("warning: PU oversubscribed\n");
      }
      if (hwloc_bitmap_weight(obj->cpuset) != 1) {
        printf("error: expected pu, got weight %d\n", hwloc_bitmap_weight(obj->cpuset));
        exit(-1);
      } 
    }
  }
}

int INTERNAL qt_affinity_compact(int num_workers, 
                                 hwloc_obj_t obj)
{
  int n = num_workers;
  hwloc_obj_t child = obj->first_child;
  if (child) {
    // Have a child, not a leaf
    while(child && n > 0){
      n = qt_affinity_compact(n, child);
      child = child->next_sibling;
    }
  } else {
    // No children, should be PU
    workers.binds[workers.num - n] = hwloc_bitmap_alloc();
    hwloc_bitmap_copy(workers.binds[workers.num - n], obj->cpuset);
    if (hwloc_bitmap_weight(obj->cpuset) != 1){
      printf("error: expected pu, got weight %d\n", hwloc_bitmap_weight(obj->cpuset));
      exit(-1);
    } 
    return n - 1;
  }
  return n;
}
  
// Checks affinity environment variables using the following precedence:
//   1. QT_CPUBIND
//   2. QT_NUM_SHEPHERDS -- QT_NUM_WORKERS_PER_SHEPHERD -- QT_HWPAR
//      -- QT_BALANCED || QT_COMPACT
void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers,
                               size_t                *hw_par)
{                           
  hwloc_topology_init(&topology);
  hwloc_topology_load(topology);
  const char *bindstr = qt_internal_get_env_str("CPUBIND", "NOT_SET");
  if (!bindstr || strcmp("NOT_SET", bindstr) == 0) {
    size_t num_sheps = qt_internal_get_env_num("NUM_SHEPHERDS", 1, 0);
    size_t num_workers_per_shepherd = qt_internal_get_env_num("NUM_WORKERS_PER_SHEPHERD", 0, 0);
    size_t num_workers = qt_internal_get_env_num("HWPAR", 0, 0);
    const char * affinity = qt_internal_get_env_str("LAYOUT", "NOT_SET");
    hwloc_const_bitmap_t allowed = hwloc_bitmap_dup(hwloc_topology_get_allowed_cpuset(topology));
    if (!allowed) {
      printf("hwloc detection of allowed cpus failed\n");
      exit(-1);
    }

    // Determine number of workers
    if (num_workers) {
      workers.num = num_workers;
    } else if (num_workers_per_shepherd && num_sheps) {
      workers.num = num_workers_per_shepherd * num_sheps;
    } else {
      workers.num = hwloc_bitmap_weight(allowed);
    } 

    // Determine number of shepherds 
    if (num_sheps) {
      sheps.num = num_sheps;
    } else {
      sheps.num = 1;
    }

    workers.binds = qt_malloc(sizeof(hwloc_cpuset_t) * workers.num);
    for (int i = 0; i< workers.num; i++) {
      workers.binds[i] = hwloc_bitmap_alloc();  
      workers.binds[i] = hwloc_bitmap_dup(allowed);
    }
    hwloc_obj_t obj = hwloc_get_obj_inside_cpuset_by_depth(topology, allowed, 0, 0);
    if (affinity && strcmp("COMPACT", affinity) == 0) {
      qt_affinity_compact(workers.num, obj);
    }
    if (affinity && strcmp("BALANCED", affinity) == 0) {
      qt_affinity_balanced(workers.num, 0, obj);
    }

  } else {
    char *bstr = qt_malloc(strlen(bindstr));
    strcpy(bstr,bindstr);
    int i,j;
    sheps.num = 1; 
    i = 0;
    while (bstr[i] != 0) {
      if (bstr[i] == ':') {
        sheps.num ++;
      }
      i++;
    }
    char ** ranges = qt_malloc(sizeof(char **) * sheps.num);
    j = 0;
    ranges[j++] = bstr; 
    for (i=0; bstr[i] != 0; i++) {
      if (bstr[i] == ':') {
        bstr[i] = 0;
        ranges[j++] = bstr+i+1;
      }
    }

    sheps.binds = qt_malloc(sizeof(hwloc_cpuset_t) * sheps.num);
    workers.num = 0;
    for (i=0; i<sheps.num; i++) {
      char tmp[256];
      sheps.binds[i] = hwloc_bitmap_alloc();
      hwloc_bitmap_list_sscanf(sheps.binds[i], ranges[i]);
      //hwloc_bitmap_list_snprintf(tmp, 256, sheps.binds[i]);
      workers.num += hwloc_bitmap_weight(sheps.binds[i]);
    }
    j = 0;
    int id;
    workers.binds = qt_malloc(sizeof(hwloc_cpuset_t) * workers.num);
    for (i=0; i<sheps.num; i++) {
      char tmp[256];
      hwloc_bitmap_foreach_begin(id, sheps.binds[i])
        workers.binds[j] = hwloc_bitmap_alloc();
        hwloc_bitmap_set(workers.binds[j], id);
        //hwloc_bitmap_list_snprintf(tmp, 256, workers.binds[j]);
        j++;
      hwloc_bitmap_foreach_end();
    }
  }
  *nbshepherds = sheps.num;
  *nbworkers = workers.num / sheps.num;
  *hw_par = workers.num; 

  // Sanity check
  if (workers.num % sheps.num != 0) {
    printf("error: non-uniform number of workers per shepherd\n");
    exit(-1);
  }
}

void INTERNAL qt_affinity_set(qthread_worker_t *me,
                              unsigned int      nworkerspershep)
{                                                                                             
  hwloc_set_cpubind(topology, workers.binds[me->packed_worker_id], HWLOC_CPUBIND_THREAD);
}                                     

int INTERNAL qt_affinity_gendists(qthread_shepherd_t   *sheps,
                                  qthread_shepherd_id_t nshepherds)
{                                                                     

  for (size_t i = 0; i < nshepherds; ++i) {
      sheps[i].node            = i;
      sheps[i].sorted_sheplist = qt_calloc(nshepherds - 1,
                                           sizeof(qthread_shepherd_id_t));
      sheps[i].shep_dists      = qt_calloc(nshepherds, sizeof(unsigned int));
  }
  for (size_t i = 0; i < nshepherds; ++i) {
    for (size_t j = 0, k = 0; j < nshepherds; ++j) {
      if (j != i) {
        sheps[i].shep_dists[j] = 10;
        sheps[i].sorted_sheplist[k++] = j;
      }
    }
  }

  return QTHREAD_SUCCESS;
}                             

/* vim:set expandtab: */
