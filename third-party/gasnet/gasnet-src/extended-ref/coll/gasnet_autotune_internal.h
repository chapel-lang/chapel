/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_autotune_internal.h $
 * Description: GASNet Autotuner Implementation
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>                              
 * Terms of use are as specified in license.txt
 */

/*During the collective initialization process all the algorithms are registered
 (including those defined by the conduit) with the auto-tuner
 
 given the capability requirements of the algorithms (i.e. synch modes, teh size, and whether or not the source and dest are inthe segment)
 the autotuner can returns a triple of (number of valid algorithms, number of tuning parameters, and the range of each tunign parameter)
 
 thus the query functions will return the number 
 */


#ifndef __GASNET_AUTOTUNE_INTERNAL_H__
#define __GASNET_AUTOTUNE_INTERNAL_H__ 1

#define GASNETE_COLL_DEFAULT_TREE_TYPE_STR "KNOMIAL_TREE,2"
#define GASNETE_COLL_DEFAULT_DISSEM_LIMIT_PER_THREAD 1024
#include <myxml/myxml.h>
#include <coll/gasnet_coll.h>

/*returns the implementation of the collectives including all the parameters to the algorithm*/
struct gasnete_coll_implementation_t_{
  struct gasnete_coll_implementation_t_ *next;
  gex_Event_t (*fn_ptr)();
  int fn_idx;
  gasnet_team_handle_t team;
  gasnet_coll_optype_t optype;
  uint32_t flags;
  int num_params;
  int need_to_free;
  gasnete_coll_tree_type_t tree_type;
  uint32_t param_list[GASNET_COLL_NUM_PARAM_TYPES]; /*declare an array that can take all the possible param types*/
};

typedef struct gasnete_coll_autotune_tree_node_t_ {
  struct gasnete_coll_autotune_tree_node_t_* parent;
  struct gasnete_coll_autotune_tree_node_t_** children;
  int num_children;
  char *field_name;
  unsigned int field_start;
  unsigned int field_end;
  gasnete_coll_implementation_t impl;
} gasnete_coll_autotune_tree_node_t;

typedef enum {GASNETE_COLL_NONO=0, GASNETE_COLL_NOMY, GASNETE_COLL_NOALL, 
  GASNETE_COLL_MYNO, GASNETE_COLL_MYMY, GASNETE_COLL_MYALL,
GASNETE_COLL_ALLNO, GASNETE_COLL_ALLMY, GASNETE_COLL_ALLALL, GASNETE_COLL_NUM_SYNCMODES}
  gasnete_coll_syncmode_t;

typedef enum {GASNETE_COLL_LOCAL_MODE,
  GASNETE_COLL_NUM_ADDRMODES} gasnete_coll_addr_mode_t;


  


typedef gex_Event_t 
(*gasnete_coll_bcast_fn_ptr_t)(gasnet_team_handle_t team,
                               void * dst,
                               gasnet_image_t srcimage, void *src,
                               size_t nbytes, int flags,
                               gasnete_coll_implementation_t coll_params,
                               uint32_t sequence
                               GASNETI_THREAD_FARG);

typedef gex_Event_t 
(*gasnete_coll_scatter_fn_ptr_t)(gasnet_team_handle_t team,
                                 void *dst,
                                 gasnet_image_t srcimage, void *src,
                                 size_t nbytes, size_t dist, int flags,
                                 gasnete_coll_implementation_t coll_params,
                                 uint32_t sequence
                                 GASNETI_THREAD_FARG);


typedef gex_Event_t 
(*gasnete_coll_gather_fn_ptr_t)(gasnet_team_handle_t team,
                                gasnet_image_t dstimage, void *dst,
                                void *src,
                                size_t nbytes, size_t dist, int flags, 
                                gasnete_coll_implementation_t coll_params,
                                uint32_t sequence
                                GASNETI_THREAD_FARG);

typedef gex_Event_t 
(*gasnete_coll_gather_all_fn_ptr_t)(gasnet_team_handle_t team,
                                    void *dst, void *src,
                                    size_t nbytes, int flags, 
                                    gasnete_coll_implementation_t coll_params,
                                    uint32_t sequence
                                    GASNETI_THREAD_FARG);

typedef gex_Event_t
(*gasnete_coll_exchange_fn_ptr_t)(gasnet_team_handle_t team,
                                    void *dst, void *src,
                                    size_t nbytes, int flags, 
                                    gasnete_coll_implementation_t coll_params,
                                    uint32_t sequence
                                    GASNETI_THREAD_FARG);

typedef enum {
  GASNETE_COLL_BROADCAST_TREE_PUT_SCRATCH,
  GASNETE_COLL_BROADCAST_TREE_PUT_SEG,
  GASNETE_COLL_BROADCAST_SCATTERALLGATHER,
  GASNETE_COLL_BROADCAST_EAGER,
  GASNETE_COLL_BROADCAST_TREE_EAGER,
  GASNETE_COLL_BROADCAST_RVOUS,
  GASNETE_COLL_BROADCAST_RVGET,
  GASNETE_COLL_BROADCAST_TREE_RVGET,
#ifdef GASNETE_COLL_CONDUIT_BROADCAST_OPS
  /*check to see if the conduits have defined any new ops*/
  GASNETE_COLL_CONDUIT_BROADCAST_OPS ,
#endif
  GASNETE_COLL_BROADCAST_NUM_ALGS} gasnete_coll_broadcast_alg_types_t;

typedef enum {
  GASNETE_COLL_SCATTER_TREE_PUT,
  GASNETE_COLL_SCATTER_TREE_PUT_NO_COPY,
  GASNETE_COLL_SCATTER_TREE_PUT_SEG,
  GASNETE_COLL_SCATTER_TREE_EAGER,
  GASNETE_COLL_SCATTER_EAGER,
  GASNETE_COLL_SCATTER_RVGET,
  GASNETE_COLL_SCATTER_RVOUS,
#ifdef GASNETE_COLL_CONDUIT_SCATTER_OPS
  /*check to see if the conduits have defined any new ops*/
  GASNETE_COLL_CONDUIT_SCATTER_OPS ,
#endif
  GASNETE_COLL_SCATTER_NUM_ALGS} gasnete_coll_scatter_alg_types_t;

typedef enum {
  GASNETE_COLL_GATHER_TREE_PUT,
  GASNETE_COLL_GATHER_TREE_PUT_NO_COPY,
  GASNETE_COLL_GATHER_TREE_PUT_SEG,
  GASNETE_COLL_GATHER_TREE_EAGER,
  GASNETE_COLL_GATHER_EAGER,
  GASNETE_COLL_GATHER_RVPUT,
  GASNETE_COLL_GATHER_RVOUS,
#ifdef GASNETE_COLL_CONDUIT_GATHER_OPS
  GASNETE_COLL_CONDUIT_GATHER_OPS ,
#endif
  GASNETE_COLL_GATHER_NUM_ALGS} gasnete_coll_gather_alg_types_t;

typedef enum {
  GASNETE_COLL_GATHER_ALL_DISSEM_EAGER=0,
  GASNETE_COLL_GATHER_ALL_DISSEM,
  GASNETE_COLL_GATHER_ALL_FLAT_PUT_EAGER,
  GASNETE_COLL_GATHER_ALL_GATH,
#ifdef GASNETE_COLL_CONDUIT_GATHER_ALL_OPS
  GASNETE_COLL_CONDUIT_GATHER_ALL_OPS ,
#endif
  GASNETE_COLL_GATHER_ALL_NUM_ALGS} gasnete_coll_gather_all_alg_types_t;

typedef enum {
  GASNETE_COLL_EXCHANGE_DISSEM2=0,
  GASNETE_COLL_EXCHANGE_DISSEM3,
  GASNETE_COLL_EXCHANGE_DISSEM4,
  GASNETE_COLL_EXCHANGE_DISSEM8,
  GASNETE_COLL_EXCHANGE_FLAT_SCRATCH,
  GASNETE_COLL_EXCHANGE_RVPUT,
  GASNETE_COLL_EXCHANGE_GATH,
#ifdef GASNETE_COLL_CONDUIT_EXCHANGE_OPS
  GASNETE_COLL_CONDUIT_EXCHANGE_OPS ,
#endif
  GASNETE_COLL_EXCHANGE_NUM_ALGS} gasnete_coll_exchange_alg_types_t;


#ifndef GASNET_COLL_MIN_PIPE_SEG_SIZE
#define GASNET_COLL_MIN_PIPE_SEG_SIZE 8192
#endif

#ifndef GASNET_COLL_MAX_PIPE_SEG_SIZE
#define GASNET_COLL_MAX_PIPE_SEG_SIZE gex_AM_LUBRequestLong()
#endif

/*flags to control how the search space looks like*/
#define GASNET_COLL_TUNING_STRIDE_ADD (1<<0)
#define GASNET_COLL_TUNING_STRIDE_MULTIPLY (1<<1)
#define GASNET_COLL_TUNING_SIZE_PARAM (1<<2)
#define GASNET_COLL_TUNING_TREE_SHAPE (1<<3)

struct gasnet_coll_tuning_parameter_t {
  gasnet_coll_tuning_param_type_t tuning_param;
  uint32_t start;
  uint32_t end;
  uint32_t stride;
  int flags;
};

/* Macro to initialize a 1-element array of gasnet_coll_tuning_parameter_t.
 * GASNet coding standards prohibit the use of non-const struct initializers
   (which we may need for start and end members).
*/
#define GASNETE_COLL_TUNING_PARAMETER(_name,_param,_start,_end,_stride,_flags) \
    struct gasnet_coll_tuning_parameter_t _name[1] = \
       { { _param,0,0,_stride,_flags } };\
    _name[0].start = _start; \
    _name[0].end   = _end /* no semicolon */

/*contains an entry in the function table*/
typedef struct gasnete_coll_allgorithm_t_ {
  struct gasnete_coll_allgorithm_t_ *next;
  
  /*what kind of op is this*/
  gasnet_coll_optype_t optype;
  
  /*for what synch flags does this algorithm work*/
  uint32_t syncflags;
  
  /*what other input flags are required for this algorithm to work*/
  /*thus if the input flags and the requirements are anded together
   and the result is equal to requirements then the collective will work
   for those flags*/
  uint32_t requirements;
  
  /*A list of flags (if present) the algorithm will not work for
   */
  uint32_t n_requirements;
  
  /*the maximum number of bytes as an argument that this algorithm can handle*/
  /*probably will be based on maximum AM lengths or lenghts of largest transfers*/
  /* a size of 0 indicates that it will work for all sizes*/
  size_t max_num_bytes;
  
  /*some algorithms only work above a certain threshold*/
  size_t min_num_bytes;
  
  /*what are the parameters to the algorithm*/
  uint32_t num_parameters;
  
  /*set if this is a tree-based algorithm*/
  uint32_t tree_alg;
  
  struct gasnet_coll_tuning_parameter_t *parameter_list;
  
  union {
    gex_Event_t (*generic_coll_fn_ptr)();
    gasnete_coll_bcast_fn_ptr_t bcast_fn;
    gasnete_coll_scatter_fn_ptr_t scatter_fn;
    gasnete_coll_gather_fn_ptr_t gather_fn;
    gasnete_coll_gather_all_fn_ptr_t gather_all_fn;
    gasnete_coll_exchange_fn_ptr_t exchange_fn;
  } fn_ptr;
  
  const char *name_str;
} gasnete_coll_algorithm_t;

#define GASNETE_COLL_AUTOTUNE_RADIX_ARR_LEN 20
#define GASNETE_COLL_LOG2_AUTOTUNE_MAX_SIZE 20
#define GASNETE_COLL_ALG_INDEX_SIZE (GASNET_COLL_NUM_COLL_OPTYPES*GASNETE_COLL_NUM_SYNCMODES*GASNETE_COLL_NUM_ADDRTYPES*GASNETE_COLL_LOG2_AUTOTUNE_MAX_SIZE)

struct gasnete_coll_autotune_index_entry_t_ {
  struct gasnete_coll_autotune_index_entry_t_ *subtree;
  struct gasnete_coll_autotune_index_entry_t_ *next_interval;
  
  const char* node_type;
  int start;
  int end;
  
  /*when added to the profiling information the num_calls field will be updated*/
  int num_calls;
  
  /*when added to the actual index the impl is the relavent field*/
  gasnete_coll_implementation_t impl;
};

typedef struct gasnete_coll_autotune_index_entry_t_ gasnete_coll_autotune_index_entry_t;

struct gasnete_coll_autotune_info_t_ {
  gasnete_coll_tree_type_t bcast_tree_type;
  gasnete_coll_tree_type_t scatter_tree_type;
  gasnete_coll_tree_type_t gather_tree_type;
  
  size_t gather_all_dissem_limit;
  size_t exchange_dissem_limit;
  int exchange_dissem_radix;
  size_t pipe_seg_size;
  
  int warm_iters;
  int perf_iters;
	
  int allow_flat_tree;
  
	/*array index i tells you what the tree fanout should be for 2^(i-1) < nbytes <= 2^(i) bytes*/
	int bcast_tree_radix_limits[GASNETE_COLL_AUTOTUNE_RADIX_ARR_LEN];
  
  gasnete_coll_algorithm_t *collective_algorithms[GASNET_COLL_NUM_COLL_OPTYPES];
  gasnete_coll_autotune_index_entry_t *autotuner_defaults;
  gasnete_coll_autotune_index_entry_t *collective_profile;
  gasnete_coll_team_t team;
  int search_enabled;
  int profile_enabled;
};




gasnete_coll_autotune_info_t* gasnete_coll_autotune_init(gasnet_team_handle_t team
                                                         GASNETI_THREAD_FARG);
void gasnete_coll_autotune_free(gasnet_team_handle_t team);
/*testing functions*/

gasnete_coll_tree_type_t gasnete_coll_autotune_get_tree_type(gasnete_coll_autotune_info_t* autotune_info, 
                                                             gasnet_coll_optype_t op_type, 
                                                             gex_Rank_t root, size_t nbytes, int flags);


#ifdef GASNETE_COLL_CONDUIT_COLLECTIVES
void gasnete_coll_register_conduit_collectives(gasnete_coll_autotune_info_t* info);
#endif

gasnete_coll_algorithm_t gasnete_coll_autotune_register_algorithm(gasnet_team_handle_t team,
                                                                  gasnet_coll_optype_t optype, 
                                                                  uint32_t syncflags,
                                                                  uint32_t requirements,
                                                                  uint32_t n_requirements,
                                                                  size_t max_size, size_t min_size,
                                                                  uint32_t tree_alg,
                                                                  uint32_t num_params,
                                                                  struct gasnet_coll_tuning_parameter_t *param_list,
                                                                  gex_Event_t (*coll_fnptr)(),
                                                                  const char *name_str);

size_t gasnete_coll_get_dissem_limit(gasnete_coll_autotune_info_t* autotune_info, gasnet_coll_optype_t op_type, int flags);

size_t gasnete_coll_get_pipe_seg_size(gasnete_coll_autotune_info_t* autotune_info, gasnet_coll_optype_t op_type, int flags);
int gasnete_coll_get_dissem_radix(gasnete_coll_autotune_info_t* autotune_info, gasnet_coll_optype_t op_type, int flags);

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_bcast_algorithm(gasnet_team_handle_t team, void *dst, gasnet_image_t srcimage, void *src, 
                                          size_t nbytes, uint32_t flags  GASNETI_THREAD_FARG);

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_scatter_algorithm(gasnet_team_handle_t team, void *dst, gasnet_image_t srcimage, void *src, size_t nbytes, size_t dist, uint32_t flags  GASNETI_THREAD_FARG);

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_gather_algorithm(gasnet_team_handle_t team,gasnet_image_t dstimage, void *dst, void *src, 
                                           size_t nbytes, size_t dist, uint32_t flags  GASNETI_THREAD_FARG);

gasnete_coll_implementation_t 
gasnete_coll_autotune_get_gather_all_algorithm(gasnet_team_handle_t team, void *dst, void *src, 
                                                size_t nbytes, uint32_t flags  GASNETI_THREAD_FARG);


gasnete_coll_implementation_t 
gasnete_coll_autotune_get_exchange_algorithm(gasnet_team_handle_t team, void *dst, void *src, 
                                             size_t nbytes, uint32_t flags  GASNETI_THREAD_FARG);


gasnete_coll_implementation_t gasnete_coll_lookup_implementation(gasnete_coll_autotune_info_t* autotune_info, 
                                                                 gasnet_coll_optype_t optype, gasnete_coll_syncmode_t syncmode, gasnete_coll_addr_mode_t, size_t nbytes);
gasnete_coll_autotune_index_entry_t *gasnete_coll_load_autotuner_defaults(gasnete_coll_autotune_info_t* autotune_info, myxml_node_t *tuning_data);

gasnete_coll_implementation_t gasnete_coll_get_implementation(void);
void gasnete_coll_free_implementation(gasnete_coll_implementation_t in);

void gasnete_coll_implementation_print(gasnete_coll_implementation_t impl, FILE *fp);

#endif
