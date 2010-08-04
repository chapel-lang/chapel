/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_trees.h,v $
 *     $Date: 2009/10/22 20:14:56 $
 * $Revision: 1.5 $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef ALREADY_SEEN_GASNET_COLL_TREES_H 
#define ALREADY_SEEN_GASNET_COLL_TREES_H 1

#if 0
#include <gasnet_coll.h>
#include <gasnet_coll_internal.h>
#include <gasnet_extended_refcoll.h>
#endif


#define  GASNETE_COLL_DEFAULT_FANOUT 2
#define  GASNETE_COLL_DEFAULT_RADIX 2

/*first four are more "traditional"
 */
typedef enum {GASNETE_COLL_FLAT_TREE=0, GASNETE_COLL_KNOMIAL_TREE, 
              GASNETE_COLL_NARY_TREE, 
              /*insert PLATFORM INDEPENDANT Tree classes here*/
              /*the most generic searcher will go through these classes first*/
              GASNETE_COLL_NUM_PLATFORM_INDEP_TREE_CLASSES,
              /*insert more specialized trees here*/
              GASNETE_COLL_RECURSIVE_TREE,
              GASNETE_COLL_FORK_TREE, 
              GASNETE_COLL_HIERARCHICAL_TREE, GASNETE_COLL_NUM_TREE_CLASSES_TOTAL} gasnete_coll_tree_class_t;

#define GASNETE_COLL_NUM_TREE_CLASSES (GASNETE_COLL_NUM_TREE_CLASSES_TOTAL - 1)

typedef enum  {
  GASNETE_COLL_TREE_RADIX, GASNETE_COLL_TREE_DIMS
} gasnete_coll_tree_tuning_param_type_t;

struct gasnete_coll_tree_type_t_ {
  gasnete_coll_tree_class_t tree_class;
  int *params;
  int num_params;
  struct gasnete_coll_tree_type_t_ *subtree;
};


/*returns 1 if they are equal or 0 otherwise*/
int gasnete_coll_compare_tree_types(gasnete_coll_tree_type_t a, gasnete_coll_tree_type_t b);

#define GASNETE_COLL_MAX_TREE_TYPE_STRLEN 100
gasnete_coll_tree_type_t gasnete_coll_make_tree_type_str(char *tree_name_str);
gasnete_coll_tree_type_t gasnete_coll_make_tree_type(int tree_type, int *params, int num_params);
char* gasnete_coll_tree_type_to_str(char *buffer, gasnete_coll_tree_type_t tree_type);

/*ACCESSOR MACROS (all take a gasnete_coll_local_tree_geom_t)*/
#define GASNETE_COLL_TREE_GEOM_ROOT(GEOM) ((GEOM)->root)
#define GASNETE_COLL_TREE_GEOM_PARENT(GEOM) ((GEOM)->parent)
#define GASNETE_COLL_TREE_GEOM_CHILD_COUNT(GEOM) ((GEOM)->child_count)
#define GASNETE_COLL_TREE_GEOM_CHILDREN(GEOM) ((GEOM)->child_list)
#define GASNETE_COLL_TREE_GEOM_SIBLING_ID(GEOM) ((GEOM)->sibling_id)
#define GASNETE_COLL_TREE_GEOM_KIND(GEOM) ((GEOM)->kind)
#define GASNETE_COLL_TREE_GEOM_FANOUT(GEOM) ((GEOM)->fanout)

/* a local view of the tree goemetry */
struct gasnete_coll_local_tree_geom_t_ {
  int allocated;
  /** tree geometry**/
  gasnet_node_t root;
  gasnete_coll_tree_type_t tree_type;
  gasnet_node_t total_size; /*total number of nodes of this geometry*/
  gasnet_node_t parent; /*parent of this node*/
  gasnet_node_t child_count; /*number of children*/
  gasnet_node_t *child_list; /*list of children*/
  gasnet_node_t *subtree_sizes; /* the size of the subtrees under each of our children */
  gasnet_node_t *child_offset;
  gasnet_node_t *grand_children; /*contians the number of children under each of our children*/
  gasnet_node_t mysubtree_size;
  uint8_t children_reversed;
  gasnet_node_t parent_subtree_size; /* size of the subtree under our parent*/
  
  /** sibling information**/
  gasnet_node_t num_siblings;

  gasnet_node_t sibling_id; /*my sibling number*/
  
  /* if the subtree of the parent of this node were to be listed linearly in DFS order, this number indicates
    the position in the parent's list where this node's subtree starts */
  gasnet_node_t sibling_offset;
  
  /* DFS Order of the tree, only assigned at the root node */
  gasnet_node_t *dfs_order;
  
  /*in order to reorder the array this indidcates where the data needs to be reordered*/
  int *rotation_points;
  int num_rotations;

  /* A boolean variable that is set if the dfs_order of the tree is sequential*/
  /* I.E. No Reordering will be needed for scatter and gathers */
  uint8_t seq_dfs_order;
  
  /*set to true if the contiguous numbering wraps around in a subtree of the root rather than as a direct child*/
  uint8_t child_contains_wrap;
  
  /*number of children that aren't leaves of the tree*/
  gasnet_node_t num_non_leaf_children;
  /*number of children that are leaves of the tree*/
  gasnet_node_t num_leaf_children;
  gasnet_node_t *dissem_order;
  int dissem_count;
  
  
} ;

/*for now i will only assume that one gasnet thread will be involved in the tree communication 
 and thus assume no locks are needed since only one given thread in a node will ever access the tree*/
 
struct gasnete_coll_tree_geom_t_ {
   /* linked list pointers 
	  used in the caching of tree geometries
   */
   gasnete_coll_tree_geom_t *next;
   gasnete_coll_tree_geom_t *prev;
 /* gasneti_weakatomic_t	ref_count; */
   
   /*an array of local views that represents the global view*/
   gasnete_coll_local_tree_geom_t **local_views; 
   int local_views_allocated;
   
   /*** tree structure metadata*****/
   gasnete_coll_tree_type_t tree_type;
  /* don't need a root argument here since local_views[i] gives a tree rooted at i*/
  gasnete_coll_team_t *team; /*a pointer back to the associated team*/
 };


/* 
   build a full tree with the tree type root and fanout
   the fanout is only applicable for nary trees
 
   This routine first checks the cache for the object
       If the tree type fanout pair exists look in the localviews array and return the appropriate pointer
	   Else 
			if none of the local views are alloc create exactly one geometry at local_views[root]
			else create them all 
			
  (implementation note ... we might change this so that we allocate all the local views)
  (we might need to construct the intermediary views as we run the DFS for some of hte trees so 
  (may as well save the time and do it at one shot)
*/



gasnete_coll_local_tree_geom_t *gasnete_coll_local_tree_geom_fetch(gasnete_coll_tree_type_t type, gasnet_node_t root, gasnete_coll_team_t team);
void gasnete_coll_local_tree_geom_release(gasnete_coll_local_tree_geom_t *geom);
gasnete_coll_tree_type_t gasnete_coll_get_tree_type(void);
void gasnete_coll_free_tree_type(gasnete_coll_tree_type_t in);
char* gasnete_coll_tree_type_to_str(char *outbuf, gasnete_coll_tree_type_t in);

/******** Dissemination Ordering **********/
#define GASNETE_COLL_DISSEM_GET_TOTAL_PHASES(DISSEM_INFO) ((DISSEM_INFO)->dissemination_phases)
#define GASNETE_COLL_DISSEM_GET_RADIX(DISSEM_INFO) ((DISSEM_INFO)->dissemination_radix)
#define GASNETE_COLL_DISSEM_MAX_BLOCKS(DISSEM_INFO) ((DISSEM_INFO)->max_dissem_blocks)
#define GASNETE_COLL_DISSEM_NBLOCKS(DISSEM_INFO) ((DISSEM_INFO)->n_blocks)

#define GASNETE_COLL_DISSEM_GET_FRONT_PEERS(DISSEM_INFO) ((DISSEM_INFO)->exchange_out_order)
#define GASNETE_COLL_DISSEM_GET_BEHIND_PEERS(DISSEM_INFO) ((DISSEM_INFO)->exchange_in_order)
#define GASNETE_COLL_DISSEM_GET_PEER_COUNT(DISSEM_INFO) ((DISSEM_INFO)->ptr_vec[(DISSEM_INFO)->dissemination_phases])

#define GASNETE_COLL_DISSEM_GET_FRONT_PEERS_PHASE(DISSEM_INFO, PHASE) ((DISSEM_INFO)->exchange_out_order+(DISSEM_INFO)->ptr_vec[(PHASE)])
#define GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(DISSEM_INFO, PHASE) ((DISSEM_INFO)->exchange_in_order+(DISSEM_INFO)->ptr_vec[(PHASE)])

#define GASNETE_COLL_DISSEM_GET_PEER_COUNT_PHASE(DISSEM_INFO, PHASE) ((DISSEM_INFO)->ptr_vec[(PHASE)+1]-(DISSEM_INFO)->ptr_vec[(PHASE)])


struct gasnete_coll_dissem_info_t_ {
  gasnete_coll_dissem_info_t *prev;
  gasnete_coll_dissem_info_t *next;
  gasnet_node_t *exchange_out_order;
  gasnet_node_t *exchange_in_order;
  gasnet_node_t *ptr_vec;
  int dissemination_phases; /*log_radix(THREADS)*/
  int dissemination_radix;
  int max_dissem_blocks;
 
  /*an array that holds the number of blocks we send in each phase of the 
    dissemination all to all. Used to capture when nblocks is 1 to avoid copies*/
  int *n_blocks; 
  /*whether this dissem obj is designed to run the all_reduce*/
  /*only true when power of two proc count AND radix is 2*/
  int all_reduce_ok; 
};

gasnete_coll_dissem_info_t *gasnete_coll_fetch_dissemination(int radix, gasnete_coll_team_t team);
void gasnete_coll_release_dissemination(gasnete_coll_dissem_info_t* obj, gasnete_coll_team_t team);
/*****************************************/

#endif
