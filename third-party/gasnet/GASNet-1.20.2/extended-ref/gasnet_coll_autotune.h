/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_autotune.h,v $                                                                                     
 *     $Date: 2009/10/22 20:14:56 $                                                                                      
 * $Revision: 1.6 $                                                                                     
 * Description: GASNet Autotuner Implementation                                                     
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt                                                     
*/

/* This is intended as a stub for the autotuner routines*/

#ifndef __GASNET_COLL_AUTOTUNE_H__
#define __GASNET_COLL_AUTOTUNE_H__ 1

#define GASNETE_COLL_DEFAULT_TREE_TYPE_STR "BINOMIAL_TREE"
#define GASNETE_COLL_DEFAULT_TREE_FANOUT 2
#define GASNETE_COLL_DEFAULT_DISSEM_LIMIT_PER_THREAD 1024


typedef enum {GASNETE_COLL_BROADCAST_OP=0, 
              GASNETE_COLL_SCATTER_OP, 
              GASNETE_COLL_GATHER_OP, 
              GASNETE_COLL_GATHER_ALL_OP,
              GASNETE_COLL_EXCHANGE_OP} gasnete_coll_autotune_optype_t;


gasnete_coll_autotune_info_t* gasnete_coll_autotune_init(gasnet_node_t mynode, gasnet_node_t total_nodes, 
                                                         gasnet_image_t my_images, gasnet_image_t total_images, 
                                                         size_t min_scratch_size);
/*testing functions*/
void gasnete_coll_set_tree_class(char *treestr, gasnete_coll_autotune_optype_t op_type);
#define gasnet_coll_set_tree_class gasnete_coll_set_tree_class

void gasnete_coll_set_fanout(int fanout, gasnete_coll_autotune_optype_t op_type);
#define gasnet_coll_set_fanout gasnete_coll_set_fanout;

gasnete_coll_tree_type_t gasnete_coll_autotune_get_tree_type(gasnete_coll_autotune_info_t* autotune_info, 
                                                             gasnete_coll_autotune_optype_t op_type, 
                                                             gasnet_node_t root, size_t nbytes, int flags) ;

size_t gasnete_coll_get_dissem_limit(gasnete_coll_autotune_info_t* autotune_info, gasnete_coll_autotune_optype_t op_type, int flags);

int gasnete_coll_get_dissem_radix(gasnete_coll_autotune_info_t* autotune_info, gasnete_coll_autotune_optype_t op_type, int flags);

size_t gasnete_coll_get_pipe_seg_size(gasnete_coll_autotune_info_t* autotune_info, gasnete_coll_autotune_optype_t op_type, int flags);

#endif
