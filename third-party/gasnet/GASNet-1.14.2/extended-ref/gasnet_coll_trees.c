/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_trees.c,v $
 *     $Date: 2009/10/22 20:14:56 $
 * $Revision: 1.13 $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_trees.h>
#define ALREADY_SEEN_TREES_C 1
#ifndef ALREADY_SEEN_GASNET_COLL_TREES_H
#error TREES_H MISSING!!
#endif

static gasneti_lifo_head_t gasnete_coll_tree_type_free_list = GASNETI_LIFO_INITIALIZER;
gasnete_coll_tree_type_t gasnete_coll_get_tree_type(void) {
  gasnete_coll_tree_type_t ret;
  
  ret = gasneti_lifo_pop(&gasnete_coll_tree_type_free_list);
  if(!ret) {
    ret = (gasnete_coll_tree_type_t) gasneti_malloc(sizeof(struct gasnete_coll_tree_type_t_));
  }
  memset(ret, 0, sizeof(struct gasnete_coll_tree_type_t_));
  return ret;
}

void gasnete_coll_free_tree_type(gasnete_coll_tree_type_t in){
  gasneti_free(in->params);
  if(in!=NULL) {
    gasneti_lifo_push(&gasnete_coll_tree_type_free_list, in);
  }
}


static int split_string(char ***split_strs, char *str, char *delim) {
  char *temp=NULL,*copy;
  int ret=0;
  size_t malloc_len = 8;
  int j=0;
  static gasneti_mutex_t lock= GASNETI_MUTEX_INITIALIZER;

  copy = gasneti_malloc(sizeof(char)*(strlen(str)+1));
  
  /*since the strtok function is desructive we have to
    create a copy of the string first to preserve the orignal*/
  GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(copy, str, sizeof(char)*(strlen(str)+1));
  gasneti_mutex_lock(&lock);
  *split_strs = (char **) gasneti_malloc(sizeof(char*) * malloc_len);
  temp = strtok(copy, delim);
  while(temp != NULL) {
    if(ret == malloc_len) {
      /*we've run out of space so grow the array by another factor*/
      malloc_len +=malloc_len;
      *split_strs = (char**) gasneti_realloc(*split_strs, sizeof(char*) * malloc_len);
      gasneti_fatalerror("more than 8 params not yet supported");

    }
    (*split_strs)[ret] = temp;
    
    ret++;
    temp=strtok(NULL, delim);
  }
  *split_strs = (char**) gasneti_realloc(*split_strs, sizeof(char*) * ret);
  gasneti_mutex_unlock(&lock);

  return ret;
}



static gasnete_coll_tree_type_t make_tree_type_str_helper(char *tree_name) {
  gasnete_coll_tree_type_t ret = gasnete_coll_get_tree_type();

  char **inner_split;
  int num_splits;
  int i;
  char inner_delim[]=",";
  num_splits = split_string(&inner_split, tree_name,inner_delim);
  if(strcmp(inner_split[0], "NARY_TREE")==0) {
    ret->tree_class = GASNETE_COLL_NARY_TREE;
  } else if(strcmp(inner_split[0], "KNOMIAL_TREE")==0) {
    ret->tree_class = GASNETE_COLL_KNOMIAL_TREE;
  } else if(strcmp(inner_split[0], "RECURSIVE_TREE")==0) {
    ret->tree_class = GASNETE_COLL_RECURSIVE_TREE;
  }  else if(strcmp(inner_split[0], "FORK_TREE")==0) {
    ret->tree_class = GASNETE_COLL_FORK_TREE;
  }else if(strcmp(inner_split[0], "FLAT_TREE")==0) {
    ret->tree_class = GASNETE_COLL_FLAT_TREE;
  } else {
    gasneti_fatalerror("Unknown Tree Type: %s\n", tree_name);
  }
  ret->params = gasneti_malloc(sizeof(int)*num_splits-1);
  ret->num_params = num_splits-1;
  for(i=0; i<ret->num_params; i++) {
    ret->params[i] = atoi(inner_split[i+1]);
  }
  gasneti_free(inner_split);
  return ret;
}
gasnete_coll_tree_type_t gasnete_coll_make_tree_type_str(char *tree_name_str) {
 
  char outter_delim[]=":";
  char inner_delim[]=",";
  char **outer_split;
  gasnete_coll_tree_type_t ret;
  /*first split the tree string on the ":"*/
  int num_levels = split_string(&outer_split, tree_name_str, outter_delim);
  if(num_levels > 1) {
    char **inner_split;
    int num_splits, num_params;
    int i;
     gasnete_coll_tree_type_t temp;
    ret = gasnete_coll_get_tree_type();
    
    num_splits = split_string(&inner_split, outer_split[0],inner_delim);
    num_params = num_splits-1;/*first split is the tree name*/
    gasneti_assert(strcmp(inner_split[0], "HIERARCHICAL_TREE")==0);
    ret->tree_class = GASNETE_COLL_HIERARCHICAL_TREE;
    if(num_params != num_levels-1){
      gasneti_fatalerror("badly formed hierarchical tree expect HIEARCHICAL_TREE,<numlevels>,<in level1>,<in level2>,..,<in level n-1>:TREE1,PARAMS1:TREE2,PARAMS2:(etc)\n");
    }
    /*NOT DONE*/
    ret->params = gasneti_malloc(sizeof(int)*(num_params));
    ret->num_params = num_params;
    for(i=0; i<num_params; i++) {
      ret->params[i] = atoi(inner_split[i+1]);
    }

    temp = ret;
    for(i=1; i<num_levels; i++) {
      temp->subtree = make_tree_type_str_helper(outer_split[i]);
      temp = temp->subtree;
    }
  } else {
    ret = make_tree_type_str_helper(tree_name_str);
  }

  gasneti_free(outer_split);
  return ret;
}

char* gasnete_coll_tree_type_to_str(char *buffer, gasnete_coll_tree_type_t tree_type) {
  int i;
  if(!tree_type) {memset(buffer, 0, 10); return buffer;}
  switch (tree_type->tree_class) {
    case GASNETE_COLL_NARY_TREE:
      sprintf(buffer, "%s", "NARY_TREE");
      break;
    case GASNETE_COLL_KNOMIAL_TREE:
      sprintf(buffer, "%s", "KNOMIAL_TREE");
      break;
    case GASNETE_COLL_RECURSIVE_TREE:
      sprintf(buffer, "%s", "RECURSIVE_TREE");
      break;
    case GASNETE_COLL_FORK_TREE:
      sprintf(buffer, "%s", "FORK_TREE");
      break;
    case GASNETE_COLL_FLAT_TREE:
      sprintf(buffer, "%s", "FLAT_TREE");
      break;
    default:
      gasneti_fatalerror("Unknown tree class: %d", tree_type->tree_class);
  }
  for(i=0; i<tree_type->num_params; i++ ) {
    gasneti_assert(strlen(buffer) < GASNETE_COLL_MAX_TREE_TYPE_STRLEN);
    sprintf(buffer, "%s,%d", buffer, tree_type->params[i]);
  }
  return buffer;
}
gasnete_coll_tree_type_t gasnete_coll_make_tree_type(int tree_class,  int *params, int num_params) {
  gasnete_coll_tree_type_t ret= gasnete_coll_get_tree_type();
  
  
#if GASNET_DEBUG
  if(tree_class >= GASNETE_COLL_NUM_TREE_CLASSES) {
    gasneti_fatalerror("Unknown Tree Class: %d\n", tree_class);
  }
#endif  
  ret->tree_class = (int) tree_class;
  ret->params = (int*) gasneti_malloc(sizeof(int)*num_params);
  GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(ret->params, params, num_params*sizeof(int));
  ret->num_params = num_params;
  
  return ret;
}


void gasnete_coll_print_tree(gasnete_coll_local_tree_geom_t *geom, int gasnete_coll_tree_mynode) {
  int i;
  
  for(i=0; i<geom->child_count; i++) {
    fprintf(stdout, "%d> child %d: %d, subtree for that child: %d (offset: %d)\n", gasnete_coll_tree_mynode, i, (int)geom->child_list[i], (int)geom->subtree_sizes[i], (int)geom->child_offset[i]);
  }
  if(gasnete_coll_tree_mynode == geom->root) {
    /* for(i=0; i<geom->total_size; i++) { */
/*       fprintf(stdout, "%d> dfs order %d: %d\n", (int)gasnete_coll_tree_mynode, i, (int)geom->dfs_order[i]); */
/*     } */
  } else {
    fprintf(stdout, "%d> parent: %d\n", (int)gasnete_coll_tree_mynode, (int)geom->parent);
  }
  fprintf(stdout, "%d> mysubtree size: %d\n", (int)gasnete_coll_tree_mynode, (int)geom->mysubtree_size);
#if 1
  fprintf(stdout, "%d> My sibling info: (id: %d, offset %d)\n", (int)gasnete_coll_tree_mynode, (int)geom->sibling_id, (int)geom->sibling_offset);
#endif
}

void gasnete_coll_set_dissemination_order(gasnete_coll_local_tree_geom_t *geom, int gasnete_coll_tree_mynode, int gasnete_coll_tree_nodes) {
  
  int i = gasnete_coll_tree_nodes;
  int k;
  int factor;
  int lognp;
  gasnet_node_t *proc_list;
  
  int j;
  
  
  lognp = 0;
  i = gasnete_coll_tree_nodes;
  while(i > 1) {
    lognp ++;
    i = i/2;
  }
  
  proc_list = (gasnet_node_t*)gasneti_malloc(sizeof(gasnet_node_t)*lognp);
  
  
  k=0;
  factor = 2;
  for(i=0; i<lognp; i++) {
    j = (gasnete_coll_tree_mynode + (factor/2))%factor;
    j += (gasnete_coll_tree_mynode / factor) * factor;
    proc_list[i] = j;
    factor = factor * 2;
  }
  
  geom->dissem_order = proc_list;
  geom->dissem_count = lognp;
}


struct tree_node_t_ {
  gasnet_node_t id;
  struct tree_node_t_ *parent;
  int num_children;
  uint8_t children_reversed;
  struct tree_node_t_ **children;
};
typedef struct tree_node_t_* tree_node_t;

#ifndef MIN
#define MIN(A,B) ((A) < (B) ? (A) : (B))
#endif

#ifndef MAX
#define MAX(A,B) ((A) > (B) ? (A) : (B))
#endif

#define MYABS(A) ((A) < 0 ? (-1)*(A) : (A))
#define MYCEIL(A, B) (((A) % (B)) !=0 ? ((A) / (B))+1 : (A)/(B)) 

#define GET_PARENT_ID(TREE_NODE) ((TREE_NODE)->parent==NULL ? -1 : (TREE_NODE)->parent->id)
#define GET_NODE_ID(TREE_NODE) ((TREE_NODE)->id)
#define GET_NUM_CHILDREN(TREE_NODE) ((TREE_NODE)->num_children)
#define GET_CHILD_IDX(TREE_NODE, IDX) ((TREE_NODE)->children[IDX])

static tree_node_t *allocate_nodes(tree_node_t **curr_nodes, gasnet_team_handle_t team, int rootrank) {
  gasnet_node_t i;
  int new_allocation=0;

  if(!(*curr_nodes)) {
    *curr_nodes = (tree_node_t*) gasneti_malloc(sizeof(tree_node_t)*team->total_ranks);
    new_allocation=1;
  }
  for(i=0; i<team->total_ranks; i++) {
    if(new_allocation) {
      (*curr_nodes)[i] = (struct tree_node_t_*) gasneti_calloc(1,sizeof(struct tree_node_t_));
    } else {
      gasneti_free((*curr_nodes)[i]->children);
      (*curr_nodes)[i]->children = NULL;
      (*curr_nodes)[i]->num_children = 0;
      (*curr_nodes)[i]->children_reversed = 0;
    }
    (*curr_nodes)[i]->id = (i+rootrank)%team->total_ranks;
    (*curr_nodes)[i]->parent = NULL;
  }

  return *curr_nodes;
}

/*preappend a list of children*/
static tree_node_t preappend_children(tree_node_t main_node, tree_node_t *child_nodes, int num_nodes) {
  if(num_nodes > 0) {
    if(main_node->num_children == 0) {
      main_node->children = gasneti_malloc(num_nodes * sizeof(tree_node_t));
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(main_node->children, child_nodes, sizeof(tree_node_t)*num_nodes);
    } else {
      tree_node_t *new_children = gasneti_malloc(sizeof(tree_node_t)*
                                         (main_node->num_children+num_nodes));
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(new_children, child_nodes, num_nodes*sizeof(tree_node_t));
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(new_children+num_nodes, main_node->children, 
             main_node->num_children*(sizeof(tree_node_t)));
      
      gasneti_free(main_node->children);
      main_node->children = new_children;
    }
    main_node->num_children = main_node->num_children+num_nodes;
  }
  return main_node;
}

static tree_node_t make_chain_tree(tree_node_t *nodes, int num_nodes) {
  gasnet_node_t i;
  for(i=0; i<num_nodes-1; i++) {
    /*put each successive node as the child of the previous*/
    preappend_children(nodes[i], &nodes[i+1], 1);
  }
  return nodes[0];
}



static gasnet_node_t multarr(int *arr, int nelem){ 
  int ret=1; int i;
  for(i=0; i<nelem; i++) {
    ret*=arr[i];
  }
  return ret;
}


/*need to worry about corner cases*/
static tree_node_t make_fork_tree(tree_node_t *nodes, int num_nodes, 
                           int *dims, int ndims) {
  int i;
  int stride;
  tree_node_t *temp_nodes;
  gasneti_assert(ndims > 0);
  gasneti_assert(multarr(dims, ndims)==num_nodes);
  
  if(ndims > 1) {
    temp_nodes = gasneti_malloc(sizeof(tree_node_t)*dims[0]);
    stride = multarr(dims+1,ndims-1);
    for(i=dims[0]-1; i>=0; i--) {
      temp_nodes[i] = make_fork_tree(nodes+stride*i, stride,
                                     dims+1, ndims-1);
    }
    
    make_chain_tree(temp_nodes, dims[0]);
    gasneti_free(temp_nodes);
  } else {
    make_chain_tree(nodes, dims[0]);
  }
  nodes[0]->children_reversed=1;
  return nodes[0];
}

static tree_node_t make_knomial_tree(tree_node_t *nodes, int num_nodes, int radix) {
  int i,j;
  int num_children=0;
  
  gasneti_assert(radix>1);
  if(num_nodes > 1) {
    int r;
    int stride = 1;
    int num_proc = 1;
    tree_node_t *children;
    while(num_proc < num_nodes) {
      for(r=stride; r<stride*radix; r+=stride) {
        num_proc += MIN(stride, num_nodes - num_proc);
        num_children++;
        if(num_proc == num_nodes) break;
      }
      stride*=radix;
    }
    children = (tree_node_t*) gasneti_malloc(num_children*sizeof(tree_node_t));
    
    num_proc = 1; i=1; stride = 1;
    
    while(num_proc<num_nodes) {
      for(r=stride; r<stride*radix; r+=stride) {
        gasneti_assert(i<=num_children);
        children[num_children-i] = make_knomial_tree(nodes+r,MIN(stride, num_nodes - num_proc), radix);
        num_proc += MIN(stride, num_nodes - num_proc);
        if(num_proc == num_nodes) break;
        i++;
      }
      stride*=radix;
    }
    nodes[0]->children_reversed=1;
    preappend_children(nodes[0], children, num_children);
    gasneti_free(children);
  }
  return nodes[0];
}

static tree_node_t make_recursive_tree(tree_node_t *nodes, gasnet_node_t num_nodes, int radix) {
  gasnet_node_t i,j;
  int num_children=0;

  if(num_nodes > 1) {
    tree_node_t *children;
    gasneti_assert(radix > 1);
    for(i=1; i<num_nodes; i*=radix) {
      num_children++;
    }
    children = (tree_node_t*) gasneti_malloc(num_children*sizeof(tree_node_t));
    /*reverse the order of hte children as specified by the binomial tree construction*/
    for(i=1,j=num_children-1; i<num_nodes; i*=radix,j--) {
      children[j] = make_recursive_tree(nodes+i,
                                      (MIN(num_nodes, (i*radix)) - i),
                                      radix);
    }
    nodes[0]->children_reversed=1;
    preappend_children(nodes[0], children, num_children);
    gasneti_free(children);
  }
  
  return nodes[0];
}

static tree_node_t make_nary_tree(tree_node_t *nodes, gasnet_node_t num_nodes, int radix) {
  gasnet_node_t num_children=0;
  int i,j;

  if(num_nodes > 1) {
    tree_node_t *children;
    for(j=0; j<radix; j++){
      int start,end;
      start = (j==0 ? 1 : MIN(num_nodes, j*(MYCEIL(num_nodes, radix))));
      end = MIN(num_nodes, (j+1)*MYCEIL(num_nodes, radix)); 
      if(start == end) continue;
      num_children++;
    }
    if(num_children > 0) {
      children = (tree_node_t*) gasneti_malloc(num_children*sizeof(tree_node_t));

      for(j=0, i=num_children-1; j<radix; j++) {
        int start,end;
        start = (j==0 ? 1 : MIN(num_nodes, j*(MYCEIL(num_nodes, radix))));
        end = MIN(num_nodes, (j+1)*MYCEIL(num_nodes, radix)); 
        if(start == end) continue;        
        children[i] = make_nary_tree(nodes+start, end-start, radix);
        i--;
      }
    
      nodes[0]->children_reversed=1;
      preappend_children(nodes[0], children, num_children);
      gasneti_free(children);
    }
  } 
  return nodes[0];
}

static tree_node_t make_flat_tree(tree_node_t *nodes, int num_nodes) {
  /*attach all the nodes to one nodes[0]*/
  preappend_children(nodes[0], nodes+1, num_nodes-1);
  return nodes[0];
}

static tree_node_t make_hiearchical_tree_helper(gasnete_coll_tree_type_t tree_type, int level, int final_level, tree_node_t *allnodes, int num_nodes, int *node_counts) {
  tree_node_t rootnode;
  tree_node_t *temp;
  gasneti_assert(tree_type !=NULL);
  if(level == final_level) {
    switch (tree_type->tree_class) {
      case GASNETE_COLL_NARY_TREE:
        rootnode = make_nary_tree(allnodes, num_nodes, tree_type->params[0]);
        break;
      case GASNETE_COLL_FLAT_TREE:
        rootnode = make_flat_tree(allnodes, num_nodes);
        break;
      case GASNETE_COLL_KNOMIAL_TREE:
        rootnode = make_knomial_tree(allnodes, num_nodes, tree_type->params[0]);
        break;
      case GASNETE_COLL_RECURSIVE_TREE:
        rootnode = make_recursive_tree(allnodes, num_nodes, tree_type->params[0]);
        break;
      case GASNETE_COLL_FORK_TREE:
        rootnode = make_fork_tree(allnodes, num_nodes, tree_type->params, tree_type->num_params);
        break;
      default:
        gasneti_fatalerror("unknown tree type");
    }
  
  } else {
    int i,j=0,num_processed=0;
    int level_nodes = MYCEIL(num_nodes, node_counts[0]);
    temp = gasneti_malloc(sizeof(tree_node_t) * level_nodes);
    for(i=0; i<level_nodes-1; i++) {
      temp[j]=make_hiearchical_tree_helper(tree_type->subtree, level+1, final_level, allnodes+i*node_counts[0], node_counts[0], node_counts+1);  
      j++;
      num_processed += node_counts[0];
    }
    temp[j]=make_hiearchical_tree_helper(tree_type->subtree, level+1, final_level, allnodes+i*node_counts[0], num_nodes - num_processed, node_counts+1); 
    j++;
    switch (tree_type->tree_class) {
      case GASNETE_COLL_NARY_TREE:
        rootnode = make_nary_tree(temp, j, tree_type->params[0]);
        break;
      case GASNETE_COLL_FLAT_TREE:
        rootnode = make_flat_tree(temp,j);
        break;
      case GASNETE_COLL_KNOMIAL_TREE:
        rootnode = make_knomial_tree(temp, j, tree_type->params[0]);
        break;
      case GASNETE_COLL_RECURSIVE_TREE:
        rootnode = make_recursive_tree(temp, j, tree_type->params[0]);
        break;
      case GASNETE_COLL_FORK_TREE:
        rootnode = make_fork_tree(temp, j, tree_type->params, tree_type->num_params);
        break;
      default:
        gasneti_fatalerror("unknown tree type");
    }
    gasneti_free(temp);
  }
  return rootnode;
}

static tree_node_t make_hiearchical_tree(gasnete_coll_tree_type_t tree_type, tree_node_t *allnodes, int num_nodes) {
  /*first param tells us how many tree levels there are going to be*/
  /*the second contains the number at teh lowest level grouping*/
  /*each tree level contains a triple (tree shape, <tree args>*/
  /*so a 64 node run with 8 flat trees grouped into a binomial tree w/ 8 ndoes would have
    2, 8*/
  int num_levels = tree_type->params[0];
  int curr_idx = 2;
  int i;
  gasneti_assert(tree_type->num_params >= 2);
  return make_hiearchical_tree_helper(tree_type->subtree, 0, num_levels-1, allnodes, num_nodes, tree_type->params+1);
}
       
static tree_node_t setparentshelper(tree_node_t main_node, tree_node_t parent) {
  gasnet_node_t i;
  main_node->parent = parent;
  for(i=0; i<GET_NUM_CHILDREN(main_node); i++) {
    /*set myself as the parent for the children*/
    setparentshelper(GET_CHILD_IDX(main_node, i), main_node);
  }
  return main_node;
}

static tree_node_t setparents(tree_node_t main_node){
  return setparentshelper(main_node, NULL);
}

static int treesize(tree_node_t node) {
  gasnet_node_t i;
  int ret = 1;
  if(node == NULL) return 0;
  for(i=0; i<GET_NUM_CHILDREN(node); i++) {
    ret+=treesize(GET_CHILD_IDX(node, i));
  }
  return ret;
}

static tree_node_t find_node(tree_node_t tree, gasnet_node_t id) {
  gasnet_node_t i;
  if(GET_NODE_ID(tree)==id) return tree;
  for(i=0; i<GET_NUM_CHILDREN(tree); i++) {
    tree_node_t temp = find_node(GET_CHILD_IDX(tree,i), id);
    /*found a match in the subtree return it*/
    if(temp!=NULL) {
      return temp;
    }
  }
  /*no match found */
  return NULL;
}

static void print_tree_node(tree_node_t main_node, int id) {
  int i;
  printf("%d> %d num_children: %d\n", id, (int)GET_NODE_ID(main_node), GET_NUM_CHILDREN(main_node));
  for(i=0; i<GET_NUM_CHILDREN(main_node); i++) {
    printf("%d> %d child: %d %d\n", id, (int)GET_NODE_ID(main_node), i, (int)GET_NODE_ID(GET_CHILD_IDX(main_node,i)));
  }
  for(i=0; i<GET_NUM_CHILDREN(main_node); i++) {
    print_tree_node(GET_CHILD_IDX(main_node, i), id);
  }
  return;
}
/*this fucntion is already serialized in the function that calls this 
  so from here on out there is no worry about locking*/
gasnete_coll_local_tree_geom_t *gasnete_coll_tree_geom_create_local(gasnete_coll_tree_type_t in_type, int rootrank, gasnete_coll_team_t team)  {
  gasnete_coll_local_tree_geom_t *geom;
  int i,j;
  gasnete_coll_tree_type_t intype_copy;
  tree_node_t *allnodes = (tree_node_t*) team->tree_construction_scratch;
  tree_node_t rootnode,mynode;
  gasneti_assert(rootrank<team->total_ranks && rootrank >=0);
  gasneti_assert_always(in_type);
  intype_copy = in_type;
  geom = (gasnete_coll_local_tree_geom_t*)gasneti_malloc(sizeof(gasnete_coll_local_tree_geom_t));
  gasneti_assert_always(in_type==intype_copy);
  
  switch (in_type->tree_class) {
#if 1
  case GASNETE_COLL_NARY_TREE:
      gasneti_assert(in_type->num_params ==1);
      allnodes =  allocate_nodes((tree_node_t**) &team->tree_construction_scratch, team, rootrank);
      rootnode = make_nary_tree(allnodes, team->total_ranks, in_type->params[0]);
       geom->rotation_points = (int*) gasneti_malloc(sizeof(int)*1);
       geom->num_rotations = 1;
       geom->rotation_points[0] = rootrank;
      break;
    case GASNETE_COLL_KNOMIAL_TREE:
      gasneti_assert(in_type->num_params ==1);
       allnodes = allocate_nodes((tree_node_t**) &team->tree_construction_scratch, team, rootrank);
      rootnode = make_knomial_tree(allnodes, team->total_ranks, in_type->params[0]);
       geom->rotation_points = (int*) gasneti_malloc(sizeof(int)*1);
       geom->num_rotations = 1;
       geom->rotation_points[0] = rootrank;
      break;
    case GASNETE_COLL_FLAT_TREE:
      allocate_nodes((tree_node_t**) &team->tree_construction_scratch , team, rootrank);
      rootnode = make_flat_tree(team->tree_construction_scratch, team->total_ranks);
      geom->rotation_points = (int*) gasneti_malloc(sizeof(int)*1);
      geom->num_rotations = 1;
      geom->rotation_points[0] = rootrank;
      break;
    case GASNETE_COLL_RECURSIVE_TREE:
      gasneti_assert(in_type->num_params ==1);
      allnodes = allocate_nodes((tree_node_t**) &team->tree_construction_scratch, team, rootrank);
      rootnode = make_recursive_tree(allnodes, team->total_ranks, in_type->params[0]);
      geom->rotation_points = (int*) gasneti_malloc(sizeof(int)*1);
      geom->num_rotations = 1;
      geom->rotation_points[0] = rootrank;
      break;
    case GASNETE_COLL_FORK_TREE:
      allnodes = allocate_nodes((tree_node_t**) &team->tree_construction_scratch, team, rootrank);
      rootnode = make_fork_tree(allnodes, team->total_ranks, in_type->params, in_type->num_params);
       geom->rotation_points = (int*) gasneti_malloc(sizeof(int)*1);
       geom->num_rotations = 1;
       geom->rotation_points[0] = rootrank;
      break;
    case GASNETE_COLL_HIERARCHICAL_TREE:
#if 0
      allnodes = team->tree_construction_scratch = allocate_nodes(allnodes, team, 0);
      rootnode = make_hiearchical_tree(in_type, allnodes, team->total_ranks);
       /* XXX ADD CODE TO GET ROTATION POINTS*/
      break;
#else
       gasneti_fatalerror("HIERARCHICAL_TREE not yet fully supported");
#endif
#endif
    default:
       gasneti_fatalerror("unknown tree type");
      break;
  }
  
  rootnode = setparents(rootnode);
  mynode = find_node(rootnode, team->myrank);

  geom->root = rootrank;
  geom->tree_type = in_type;
  geom->total_size = team->total_ranks;
  geom->parent = GET_PARENT_ID(mynode);
  geom->child_count = GET_NUM_CHILDREN(mynode);
  geom->mysubtree_size = treesize(mynode);
  geom->parent_subtree_size = treesize(mynode->parent);
  geom->children_reversed = mynode->children_reversed;
  if(rootrank != team->myrank) {
    geom->num_siblings = GET_NUM_CHILDREN(mynode->parent);
    geom->sibling_id = -1;
    geom->sibling_offset = 0;
    for(i=0; i<geom->num_siblings; i++) {
      int tmp_id;
      if(mynode->parent->children_reversed==1) {
        tmp_id = geom->num_siblings-1-i;
      } else {
        tmp_id =i;
      }
      if(GET_NODE_ID(GET_CHILD_IDX(mynode->parent, tmp_id))==team->myrank) {
        geom->sibling_id = tmp_id;
        break;
      } else {
        geom->sibling_offset += treesize(GET_CHILD_IDX(mynode->parent, tmp_id));
      }
    }
  } else {
    geom->num_siblings = 0;
    geom->sibling_id = 0;
    geom->sibling_offset = 0;
    /***** THIS NEEDS TO BE TAKEN OUT
      The DFS ordering that we impose on the trees will mean that this no longer needs to be kept around
      but it's in here for now for backward compatability sake until we make the neccessary changes to all the other collective algorithms
      ****/
    geom->dfs_order = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*team->total_ranks);
    for(i=0; i<team->total_ranks; i++) {
      geom->dfs_order[i] = (i+rootrank)%team->total_ranks;
    }
  }
  geom->seq_dfs_order = 1;
  geom->child_list = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*geom->child_count);
  geom->subtree_sizes = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*geom->child_count);
  geom->child_offset = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*geom->child_count);
  geom->grand_children = (gasnet_node_t*)gasneti_malloc(sizeof(gasnet_node_t)*geom->child_count); 
  geom->num_non_leaf_children=0;
  geom->num_leaf_children=0;
  geom->child_contains_wrap = 0;
  for(i=0; i<geom->child_count; i++) {
    geom->child_list[i] = GET_NODE_ID(GET_CHILD_IDX(mynode,i));
    geom->subtree_sizes[i] = treesize(GET_CHILD_IDX(mynode,i));
    geom->grand_children[i] = GET_NUM_CHILDREN(GET_CHILD_IDX(mynode, i));
    if(geom->subtree_sizes[i] > 1) {
      geom->num_non_leaf_children++;
    } else {
      geom->num_leaf_children++;
    }
    if(geom->child_list[i]+geom->subtree_sizes[i] > geom->total_size) {
      geom->child_contains_wrap = 1;
    }
    
  }
  gasneti_assert((geom->num_leaf_children+geom->num_non_leaf_children) == geom->child_count);
  
  if(mynode->children_reversed==1) {
    size_t temp_offset = 0;
    for(i=geom->child_count-1; i>=0; i--) {
      geom->child_offset[i] = temp_offset; 
      temp_offset+=geom->subtree_sizes[i];
    }
  } else {
    size_t temp_offset = 0;
    for(i=0; i<geom->child_count; i++) {
      geom->child_offset[i] = temp_offset; 
      temp_offset+=geom->subtree_sizes[i];
    }
  }
#if 0  
  gasnete_coll_print_tree(geom, gasneti_mynode);
#endif
  return geom;
}


#if 0
void gasnete_coll_tree_type_to_str(char *outbuf, gasnete_coll_tree_type_t in) {
  gasneti_assert(in->subtree == NULL);
  gasneti_assert(in->num_params <= 1);
  switch(in->tree_class) {
    
  case GASNETE_COLL_NARY_TREE:
      sprintf(outbuf, "NARY_TREE,%d", in->params[0]);
      break;
  case GASNETE_COLL_FLAT_TREE:
    sprintf(outbuf, "FLAT_TREE");
    break;
  case GASNETE_COLL_KNOMIAL_TREE:
    sprintf(outbuf, "KNOMIAL_TREE,%d", in->params[0]);
    break;
  case GASNETE_COLL_RECURSIVE_TREE:
    sprintf(outbuf, "RECURSIVE_TREE,%d", in->params[0]);
    break;
  default:
    gasneti_fatalerror("unknown tree type");
  }
  
}
#endif
/*---------------------------------------------------------------------------------*/
/* Operations to access the tree geometry cache */

/*
 Just keep track of the number of refs to an object for debug reasons
 However according to our design we will never free a geometry that is created
 It will be leaked away once the GASNet program finishes.
 */
#if 0
static void gasnete_coll_tree_geom_release(gasnete_coll_tree_geom_t *geom) {
	gasneti_weakatomic_decrement(&(geom->ref_count), 0);
}
#endif

/* the helper function goes through the cache and then either returns the appropriate geometry
 or returns NULL indicating that the tree needs to be appended to the end of the cache 
 */

int gasnete_coll_compare_tree_types(gasnete_coll_tree_type_t a, gasnete_coll_tree_type_t b) {
  
  if(a==NULL && b==NULL) {
    /*if they are both null then tehy are trivially equal*/
    return 0;
  } else if(a==NULL || b==NULL){
    /*if one is null and the other is non-null then we have to reutnr a nonzero*/
    return 0;
  } else if(a->tree_class == b->tree_class) {
    /*both tree types are non null so check the rest of the tree*/
    if(a->num_params == b->num_params) {
      int i;
      for(i=0; i<a->num_params; i++) {
        if(a->params[i]!=b->params[i]) return 0;
      }
      return 1;
    }
  } 
  return 0;
  
}
static gasnete_coll_tree_geom_t *gasnete_coll_tree_geom_fetch_helper(gasnete_coll_tree_type_t in_type, gasnete_coll_tree_geom_t *geom_cache) {
  gasnete_coll_tree_geom_t *curr_geom = geom_cache;
  while(curr_geom != NULL) {
    if(gasnete_coll_compare_tree_types(in_type, curr_geom->tree_type)){
      return curr_geom;
    } else
      curr_geom = curr_geom->next;
  }
  /*we've reached the end of the list without finding a match*/
  return NULL;
  
}
/* XXX: should per-team */

/*
 this routine will initially just return a pointer into a localview and create one if needed. 
 it will do the simple thing and not create new views and just keep reusing old views as needed
 */
    
gasnete_coll_local_tree_geom_t *gasnete_coll_local_tree_geom_fetch(gasnete_coll_tree_type_t type, gasnet_node_t root,  gasnete_coll_team_t team) {
  gasnete_coll_tree_geom_t *geom_cache_head = team->tree_geom_cache_head;
  gasnete_coll_tree_geom_t *geom_cache_tail = team->tree_geom_cache_tail;
  gasnete_coll_local_tree_geom_t *ret;
  gasnete_coll_tree_geom_t *curr_geom;
  
  
  /*lock here so that only one multiple threads don't try to build it*/
  gasneti_mutex_lock(&team->tree_geom_cache_lock);
  curr_geom = gasnete_coll_tree_geom_fetch_helper(type, geom_cache_head);
  if(curr_geom == NULL) {
    int i;
#if 0
    if(gasneti_mynode ==0) fprintf(stderr, "%d> new tree: %d type %d fanout\n",gasneti_mynode, type.tree_class, type.prams[0]);
#endif
    /* allocate new geometry */
    curr_geom = (gasnete_coll_tree_geom_t *) gasneti_malloc(sizeof(gasnete_coll_tree_geom_t));
    curr_geom->local_views = (gasnete_coll_local_tree_geom_t**) 
    gasneti_malloc(sizeof(gasnete_coll_local_tree_geom_t*)*team->total_ranks);
    for(i=0; i<team->total_ranks; i++) {
      curr_geom->local_views[i] = NULL;
    }
    curr_geom->next = NULL;
    curr_geom->tree_type = type;
    /*	curr_geom->root = root; */
    /* link it into the cache*/
    if(geom_cache_head == NULL) {
      /*cache is empty*/
      curr_geom->prev = NULL;
      team->tree_geom_cache_head = curr_geom;
      team->tree_geom_cache_tail = curr_geom;
    } else {
      team->tree_geom_cache_tail->next = curr_geom;
      curr_geom->prev = team->tree_geom_cache_tail;
      team->tree_geom_cache_tail = curr_geom;
    }
    curr_geom->local_views[root] = gasnete_coll_tree_geom_create_local(type, root, team);
    
    ret = curr_geom->local_views[root];
    
    /* create local view for the root that we request */
  } else {
    /* if it is already allocated for root go ahead and return it ... this should be the fast path*/    
    if(curr_geom->local_views[root] == NULL) {
      curr_geom->local_views[root] = gasnete_coll_tree_geom_create_local(type, root, team);
    }
    ret = curr_geom->local_views[root];
  } 

#ifdef GASNETC_HAVE_AMRDMA  
  /*at the time of this writing no conduits support this yet*/
  if(team->myrank != ret->root) {
    int count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(ret);
    gasnet_node_t *tmp = gasneti_calloc(1+count, sizeof(gasnet_node_t));
    memcpy(tmp, GASNETE_COLL_TREE_GEOM_CHILDREN(ret), count*sizeof(gasnet_node_t));
    tmp[count] = GASNETE_COLL_TREE_GEOM_PARENT(ret);
    gasnetc_amrdma_init(1+count, tmp);
    gasneti_free(tmp);
  } else {
    gasnetc_amrdma_init(GASNETE_COLL_TREE_GEOM_CHILD_COUNT(ret),
                        GASNETE_COLL_TREE_GEOM_CHILDREN(ret));
  }
#endif
  
  gasneti_mutex_unlock(&team->tree_geom_cache_lock);
  return ret;
}

void gasnete_coll_local_tree_geom_release(gasnete_coll_local_tree_geom_t *geom) {
	
	/* for now don't do anything since we will reuse all our geometries*/
	
}

/**** Dissemination Stuff ****/

/*figure out if given number is a power of 2*/
int gasnete_coll_is_power_of_two(int num) {
  /*keep shifting right until the 0th digit is not zero*/
  while((num & 0x1) == 0) num = num >> 1;
  
  /* once we hit a stopping point, shift that digit out*/
  /*if the remaining number is 0 then it is a power of 2 */
  return ((num >> 1)==0);
  
}

#if 0
static
gasnete_coll_dissem_info_t *gasnete_coll_build_dissemination(int r, gasnete_coll_team_t team) {
  gasnete_coll_dissem_info_t *ret;  
  int h,w,i,j,distance,x,numpeers,destproc;
  int num_out_peers, num_in_peers;
  ret = (gasnete_coll_dissem_info_t*) gasneti_malloc(sizeof(gasnete_coll_dissem_info_t));
  
  w = gasnete_coll_build_tree_mylogn(team->total_ranks, r);
  ret->dissemination_radix = r;
  ret->dissemination_phases = w;
  
  
  
  distance = 1;
  /* phase 2: communication in log_r(team->total_ranks) steps*/
  for(i=0; i<w; i++) {
    if(i==(w-1)) {
      /*h = ceil(team->total_ranks/DIST);*/
      h = team->total_ranks/distance;
      if(team->total_ranks % distance != 0) 
        h++;
    } else {
      h = r;
    }
    
    ret->exchange_order[i].n = h-1;
    ret->exchange_order[i].elem_list = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*(h-1));
    for(j=1; j<h; j++) {
      ret->barrier_order[i].elem_list[j-1] = (team->myrank + j*distance) % team->total_ranks;
    }
    /*scale the distance by the radix*/
    distance *= r;
  }
  
  
  
  /*simulate the packing step and figure out what is the maxiumum number of blocks that come in across all the nodes*/
  ret->max_dissem_blocks =MAX(1,(team->total_ranks/ret->dissemination_radix));
  for(i=0; i<w; i++) {
    int curr_count = 0;
    for(j=0; j<team->total_ranks; j++) {
      if( ((j / gasnete_coll_build_tree_mypow(ret->dissemination_radix, i)) % ret->dissemination_radix) 
         == 1) curr_count++; 
    }
    ret->max_dissem_blocks=MAX(ret->max_dissem_blocks, curr_count);
  }
  
  return ret;
}
#endif

/* external code to force a tree type (for testing purposes only)*/
/* tree building code*/
static int gasnete_coll_build_tree_mypow(gasnet_node_t base, int pow) {
  int ret = 1;
  while(pow!=0) {
    ret *=base;
    pow--;
  }
  return ret;
}
static int gasnete_coll_build_tree_mylog2(gasnet_node_t num) {
  unsigned int ret=0;
  while (num >= 1) {
    ret++;
    num = num >> 1;
  }
  return MAX(1,ret);
}

static int gasnete_coll_build_tree_mylogn(gasnet_node_t num, int base) {
  int ret=1;
  int mult = base;
  while (num > mult) {
    ret++;
    mult*=base;
  }
  return ret;
  
}


static
gasnete_coll_dissem_info_t *gasnete_coll_build_dissemination(int r, gasnete_coll_team_t team) {
  gasnete_coll_dissem_info_t *ret;  
  int h,w,i,j,distance,k,numpeers,destproc;
  int num_out_peers, num_in_peers;
  ret = (gasnete_coll_dissem_info_t*) gasneti_malloc(sizeof(gasnete_coll_dissem_info_t));
  
  w = gasnete_coll_build_tree_mylogn(team->total_ranks, r);
  
  ret->dissemination_radix = r;
  ret->dissemination_phases = (team->total_ranks>1 ? w : 0);
  
  
  
  ret->ptr_vec = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*(w+1));
  ret->ptr_vec[0] = 0;
  
  distance = 1;
  /* phase 2: communication in log_r(team->total_ranks) steps*/
  for(i=0; i<w; i++) {
    if(i==(w-1)) {
      /*h = ceil(team->total_ranks/DIST);*/
      h = team->total_ranks/distance;
      if(team->total_ranks % distance != 0) 
        h++;
    } else {
      h = r;
    }
    ret->ptr_vec[i+1] = ret->ptr_vec[i]+(h-1);
    /*scale the distance by the radix*/
    distance *= r;
  }
  
  ret->exchange_out_order = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*(ret->ptr_vec[w]));
  ret->exchange_in_order = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*(ret->ptr_vec[w]));
  
  distance = 1;
  /* phase 2: communication in log_r(team->total_ranks) steps*/
  for(k=0, i=0; i<w; i++) {
    if(i==(w-1)) {
      /*h = ceil(team->total_ranks/DIST);*/
      h = team->total_ranks/distance;
      if(team->total_ranks % distance != 0) 
        h++;
    } else {
      h = r;
    }
    
    for(j=1; j<h; j++, k++) {
      ret->exchange_out_order[k] = (team->myrank + j*distance) % team->total_ranks;
      ret->exchange_in_order[k]  = ((team->myrank < j*distance) ? team->total_ranks + (team->myrank - j*distance) : (team->myrank - j*distance));
      
    }
    /*scale the distance by the radix*/
    distance *= r;
  }
  
  
  
  /*simulate the packing step and figure out what is the maxiumum number of blocks that come in across all the nodes*/
  ret->max_dissem_blocks =MAX(1,(team->total_ranks/ret->dissemination_radix));
  for(i=0; i<w; i++) {
    int curr_count = 0;
    for(j=0; j<team->total_ranks; j++) {
      if( ((j / gasnete_coll_build_tree_mypow(ret->dissemination_radix, i)) % ret->dissemination_radix) 
         == 1) curr_count++; 
    }
    ret->max_dissem_blocks=MAX(ret->max_dissem_blocks, curr_count);
  }
  
  return ret;
}

gasnete_coll_dissem_info_t *gasnete_coll_fetch_dissemination(int radix, gasnete_coll_team_t team) {
  /* look through the existing cache for our dissemination order*/
  gasnete_coll_dissem_info_t *temp;
  
  if((team->dissem_cache_head == NULL) &&
     (team->dissem_cache_tail == NULL)) {
    temp = gasnete_coll_build_dissemination(radix, team);
    team->dissem_cache_head = team->dissem_cache_tail = temp;
    
    team->dissem_cache_head->next = NULL;
    team->dissem_cache_tail->prev = NULL;
    
  } else {
    temp = team->dissem_cache_head;
    while(temp!=NULL) {
	    if(temp->dissemination_radix == radix) {
        return temp;
	    } else {
        temp = temp->next;
	    }
    }
    /*we've reached the end without finding it */
    temp = gasnete_coll_build_dissemination(radix, team);
    temp->next = NULL;
    temp->prev = team->dissem_cache_tail;
    team->dissem_cache_tail->next = temp;
    team->dissem_cache_tail = temp;  
  }
  return temp;
}

void gasnete_coll_release_dissemination(gasnete_coll_dissem_info_t *obj, gasnete_coll_team_t team) {
  /* do nothing for now */
}
