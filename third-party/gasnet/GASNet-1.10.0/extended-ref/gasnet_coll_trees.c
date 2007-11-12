#include "gasnet_coll_trees.h"

/* external code to force a tree type (for testing purposes only)*/
/* tree building code*/
int gasnete_coll_build_tree_mypow(gasnet_node_t base, int pow) {
  int ret = 1;
  while(pow!=0) {
    ret *=base;
    pow--;
  }
    return ret;
}
int gasnete_coll_build_tree_mylog2(gasnet_node_t num) {
  unsigned int ret=0;
  while (num >= 1) {
    ret++;
    num = num >> 1;
  }
  return MAX(1,ret);
}

int gasnete_coll_build_tree_mylogn(gasnet_node_t num, int base) {
  int ret=1;
  int mult = base;
  while (num > mult) {
    ret++;
    mult*=base;
  }
  return ret;

}


gasnete_coll_tree_type_t gasnete_coll_make_tree_type(char *tree_name_str, gasnet_node_t fanout) {
  gasnete_coll_tree_type_t ret;
  if(strcmp(tree_name_str, "GASNET_COLL_BINOMIAL_TREE")==0) {
    ret.tree_class = GASNETE_COLL_BINOMIAL_TREE;
  } else if(strcmp(tree_name_str, "GASNET_COLL_NARY_TREE")==0) {
    ret.tree_class = GASNETE_COLL_NARY_TREE;
  } else if(strcmp(tree_name_str, "GASNET_COLL_DFS_RECURSIVE_TREE")==0) {
    ret.tree_class = GASNETE_COLL_DFS_RECURSIVE_TREE;
  } else if(strcmp(tree_name_str, "GASNET_COLL_REV_RECURSIVE_TREE")==0) {
    ret.tree_class = GASNETE_COLL_REV_RECURSIVE_TREE;
  } else {
    gasneti_fatalerror("Unknown Tree Type: %s\n", tree_name_str);
  }
  ret.fanout = fanout;
  return ret;
}


void gasnete_coll_print_tree(gasnete_coll_local_tree_geom_t *geom, int gasnete_coll_tree_mynode) {
  int i;
  
  for(i=0; i<geom->child_count; i++) {
    fprintf(stderr, "%d> child %d: %d, subtree for that child: %d\n", gasnete_coll_tree_mynode, i, (int)geom->child_list[i], (int)geom->subtree_sizes[i]);
  }
  if(gasneti_mynode == geom->root) {
    for(i=0; i<geom->total_size; i++) {
      fprintf(stderr, "%d> dfs order %d: %d\n", (int)gasneti_mynode, i, (int)geom->dfs_order[i]);
    }
  } else {
     fprintf(stderr, "%d> parent: %d\n", (int)gasnete_coll_tree_mynode, (int)geom->parent);
  }
  fprintf(stderr, "%d> mysubtree size: %d\n", (int)gasneti_mynode, (int)geom->mysubtree_size);
#if 1
  fprintf(stderr, "%d> My sibling info: (id: %d, offset %d)\n", (int)gasnete_coll_tree_mynode, (int)geom->sibling_id, (int)geom->sibling_offset);
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


int gasnete_coll_build_tree_START(int level, int fanout) {
  int i;
  int ret=0;
  for(i=0; i<level; i++) {
    ret +=gasnete_coll_build_tree_mypow(fanout,i);
  }
  return ret;
}

gasnet_node_t gasnete_coll_get_jth_digit(gasnet_node_t num, int digit_id, int radix) {
  if(radix == 2) {
    num = num >> digit_id;
    return (num & 0x1);
  } else if (radix == 4) {
    num = num >> (digit_id*2);
    return (num & (0x3));
  } else {
    while(digit_id > 0) {
      num = num/radix;
      digit_id--;
    }
    return num%radix;
  }
}

gasnet_node_t gasnete_coll_get_upper_j_digits(gasnet_node_t num, int digit_id, int radix) {
  if(radix == 2) {
    return num >> digit_id;
  } else if (radix == 4) {
    return num >> (digit_id*2);
  } else {
    while(digit_id > 0) {
      num = num/radix;
      digit_id--;
    }
    return num;
  }
}


gasnet_node_t gasnete_coll_replace_jth_digit(gasnet_node_t num, int in_digit_id, int radix, int new_digit) {
  int i=new_digit;
  int pow=1;
  int digit_id = in_digit_id;
  while(digit_id > 0) {
    i*=radix;
    pow*=radix;
    digit_id--;
  }
  
  if(gasnete_coll_get_jth_digit(num,in_digit_id,radix)==0) {
    return num+i;
  } else {
    int ret;
    int bot=0;
    int j,mult=1;
    ret = gasnete_coll_get_upper_j_digits(num,in_digit_id+1,radix)*pow*radix;
    for(j=0,mult=1;j<in_digit_id;j++,mult*=radix) {
      bot += gasnete_coll_get_jth_digit(num, j, radix)*mult;
    }
    return ret+bot;
  }
}
#define GASNETE_COLL_ACT2REL(actrank, root, nodes) ( ((actrank) >= (root)) ? (actrank) - (root) : (actrank) \
                                              - (root) + (nodes) )
#define GASNETE_COLL_REL2ACT(relrank, root, nodes) ( (relrank) < (gasneti_nodes-root) ? \
                                                     (relrank) + (root) : (relrank) + (root) - (nodes))

#define GASNETE_COLL_DFS_RECURSIVE (1<<2)
#define GASNETE_COLL_REV_RECURSIVE (1<<3)


void gasnete_coll_build_recursive_tree(int fanout, gasnet_node_t mynode, gasnet_node_t nodes, gasnet_node_t root, gasnet_node_t *parent, gasnet_node_t **children, gasnet_node_t *child_count, int options) {
  gasnet_node_t relrank;
  gasnet_node_t *temp_dest_list;
  int num_child=0;
  int i,j;
  int d = gasnete_coll_build_tree_mylogn(nodes, fanout); /* the number of digits required to represent the largest node number */
  
  
  relrank = GASNETE_COLL_ACT2REL(mynode, root,nodes);
  
  
  /*assume that the number of GASNET_NODES will fit into an unsigned 32-bit int as specified
    in gasnet.h*/
  /*thus i assume there will be a max of 2^32 = 4,294,967,296 GASNET_NODES*/
  temp_dest_list = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*gasnete_coll_build_tree_mylogn(nodes,fanout)*(fanout-1));
  /* All the processors will work first transform their number into the base specified by fanout*/
  
  /* Let us first assume that it will take d digits to represent the rank of any node numbered 0:d-1*/
  
  
  /* if digit j is 0 that menas we are are responsible for sending to those threads that have 
    the same digits as me with their j^th digit replaced with 1,2,...,k
    
    Once i find a digit j that is not 0, then my parent is simply the node with the j^th digit 
    replaced with 0. From here on out, i will have no more children and i can simply return
    */
  
  /*find children*/
  *parent = (gasnet_node_t)(-1);
  if(options & GASNETE_COLL_DFS_RECURSIVE) {
    for(j=0; j<d; j++) {
      if(gasnete_coll_get_jth_digit(relrank,j,fanout) == 0) {
        for(i=1; i<fanout; i++) {
          int temp = gasnete_coll_replace_jth_digit(relrank,j,fanout,i);
          if(temp < nodes) {
            temp_dest_list[num_child] = GASNETE_COLL_REL2ACT(temp,root,nodes);  
            num_child++;
          }
        }
      } else {
        /* I am not looking for anymore children and i am done*/
        /* Find the parent and break*/
        *parent = GASNETE_COLL_REL2ACT(gasnete_coll_replace_jth_digit(relrank, j, fanout, 0), root,nodes);
        break;
      }
    }
  } else if(options & GASNETE_COLL_REV_RECURSIVE){
    for(j=d-1; j>=0; j--) {
      if(gasnete_coll_get_jth_digit(relrank,j,fanout) == 0) {
        for(i=fanout-1; i>0; i--) {
          int temp = gasnete_coll_replace_jth_digit(relrank,j,fanout,i);
          if(temp < nodes) {
            temp_dest_list[num_child] = GASNETE_COLL_REL2ACT(temp,root,nodes);  
            num_child++;
          }
        }
      } else {
        /* I am not looking for anymore children and i am done*/
        /* Find the parent and break*/
        *parent = GASNETE_COLL_REL2ACT(gasnete_coll_replace_jth_digit(relrank, j, fanout, 0), root,nodes);
        break;
      }
    }
  } else {
    gasneti_fatalerror("BAD RECURSIVE TREE TYPE\n");
  }
  *child_count = num_child;
  *children = gasneti_malloc(sizeof(gasnet_node_t)*num_child);
  /*reverse the order of the children so that the heavier children are first*/
  for(i=0; i<num_child; i++) {
    (*children)[i] = temp_dest_list[num_child-i-1];
  }
/*  GASNETE_FAST_UNALIGNED_MEMCPY(*children, temp_dest_list, sizeof(gasnet_node_t)*num_child); */
  gasneti_free(temp_dest_list);
}


gasnete_coll_local_tree_geom_t*  gasnete_coll_build_tree(gasnete_coll_tree_class_t class, int fanout, int root, int gasnete_coll_tree_mynode, int gasnete_coll_tree_nodes, int threads_per_node) {
  
  int relrank = GASNETE_COLL_ACT2REL(gasnete_coll_tree_mynode, root,gasnete_coll_tree_nodes);
  gasnete_coll_local_tree_geom_t *geom = NULL;
  int numnodes = gasnete_coll_tree_nodes / threads_per_node;
  int mynode = relrank / threads_per_node;
  if(root%threads_per_node!=0 && gasnete_coll_tree_mynode==0) {
    fprintf(stderr, "TREE WARNING: trees are not properly optimized for the case when root%%threads_per_node (i.e. %d %% %d) !=0\n",  root, threads_per_node);
    fprintf(stderr, "TREE WARNING: use threads_per_node = 1 instead\n");
  
  }
  geom = (gasnete_coll_local_tree_geom_t*)gasneti_malloc(sizeof(gasnete_coll_local_tree_geom_t));
  gasneti_assert(gasnete_coll_tree_nodes > gasnete_coll_tree_mynode);

  geom->parent = (gasnet_node_t)(-1);
  /*initialize num_sibllings to zero so it can be set externally if need be*/
 
  geom->num_siblings = 0;
  switch(class) {
  case GASNETE_COLL_NARY_TREE:
    {
      /*if we are the root of a node then we have to build the tree
	relative to others
      */
      if(relrank%threads_per_node == 0) {
	int level;
	gasnet_node_t *tchild;
	int i,j;
	level = 0;
	tchild = (gasnet_node_t*)gasneti_malloc(sizeof(gasnet_node_t)*fanout);
	while(1) { 
	  /* has to terminate because of the semantics of the loop  */
	  if (mynode >= gasnete_coll_build_tree_START(level,fanout) && 
	      mynode < gasnete_coll_build_tree_START(level+1,fanout)) {
	    break;
	  } else {
	    level++;
	  }
	}

	if (relrank!=0) {
	  /* we expect to recieve from some one */
	  int relparent = (mynode-gasnete_coll_build_tree_START(level,fanout))/fanout + 
	    gasnete_coll_build_tree_START(level-1,fanout);
	  geom->parent = GASNETE_COLL_REL2ACT(relparent*threads_per_node,root,gasnete_coll_tree_nodes);


	}
		
	/* so now the level of the current node is set. */
	/* now we figure out where to expect the message from */
	/* special case is the 0 */
	/* now we need to set the n destinations */
	tchild[0]= ((mynode - gasnete_coll_build_tree_START(level,fanout))*fanout +
		    gasnete_coll_build_tree_START(level+1,fanout))*threads_per_node;
	for(i=1; i<fanout; i++) {
	  tchild[i] = tchild[i-1]+threads_per_node;
	} 
	geom->child_list = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*(fanout+threads_per_node));
	geom->child_count=0;
	for(i=0; i<fanout; i++) {
	  if(tchild[i]<gasnete_coll_tree_nodes) {
	    geom->child_list[geom->child_count]=GASNETE_COLL_REL2ACT(tchild[i], root,gasnete_coll_tree_nodes);
	    geom->child_count++;
	  }
	}
	for(j=1 ;j<threads_per_node;  j++) {
	  if(relrank+j<gasnete_coll_tree_nodes) {
	    geom->child_list[geom->child_count]=GASNETE_COLL_REL2ACT(relrank+j, root,gasnete_coll_tree_nodes);
	    geom->child_count++;
	  }
	}
	  
	if(geom->child_count==0) {
	  gasneti_free(geom->child_list);
	  geom->child_list=NULL;
	} else {
	  geom->child_list = (gasnet_node_t*)gasneti_realloc(geom->child_list, 
					   sizeof(gasnet_node_t)*geom->child_count);
	}
	gasneti_free(tchild);

      }
      else {
      geom->parent = GASNETE_COLL_REL2ACT(relrank - (relrank % threads_per_node),root,gasnete_coll_tree_nodes);
      geom->child_list = NULL;
      geom->child_count = 0;
      
      }
    }
    break;
  
    
  case GASNETE_COLL_BINOMIAL_TREE:
    {
      
      gasnet_node_t child, src;
      gasnet_node_t *temp_dest_list;
      int mask = 1;
      int num_child=0;
      temp_dest_list = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*gasnete_coll_build_tree_mylog2(gasnete_coll_tree_nodes));
      mask = 0x1;
      while (mask < gasnete_coll_tree_nodes) {
        if (relrank & mask) {
          src = (gasnete_coll_tree_mynode >= mask) ? (gasnete_coll_tree_mynode - mask)
	    : (gasnete_coll_tree_mynode + (gasnete_coll_tree_nodes - mask));
          geom->parent = src;
          break;
        }
        mask <<= 1;
      }
      
      mask >>= 1;
      while (mask > 0) {
        if (relrank + mask < gasnete_coll_tree_nodes) {
          child = relrank + mask;
          if (child >= gasnete_coll_tree_nodes) child -= gasnete_coll_tree_nodes;
          temp_dest_list[num_child]=GASNETE_COLL_REL2ACT(child,root,gasnete_coll_tree_nodes);
          num_child++;
        }
        mask >>= 1;
      }
      if (num_child > 0) {
        geom->child_list = (gasnet_node_t *)gasneti_malloc(sizeof(gasnet_node_t)*num_child);
        for (child = 0; child<(num_child); child++) {
          geom->child_list[child] = temp_dest_list[child];
        }
      } else {
	geom->child_list = NULL;
      }
      
      if (relrank != 0) {
	int id, i, j;
	i = relrank - GASNETE_COLL_ACT2REL(src, root,gasnete_coll_tree_nodes);
	/* compute floor(log_base_2(i)): */

	
      } 
      geom->child_count = num_child;
     /* geom->fanout = gasnete_coll_tree_nodes; */
      gasneti_free(temp_dest_list);

    }
    break;
  case GASNETE_COLL_DFS_RECURSIVE_TREE:
    gasnete_coll_build_recursive_tree(fanout, gasnete_coll_tree_mynode, gasnete_coll_tree_nodes, 
                                      root, &geom->parent, &geom->child_list, 
                                      &geom->child_count, GASNETE_COLL_DFS_RECURSIVE);
    break;
  case GASNETE_COLL_REV_RECURSIVE_TREE:
    gasnete_coll_build_recursive_tree(fanout, gasnete_coll_tree_mynode, gasnete_coll_tree_nodes, 
                                      root, &geom->parent, &geom->child_list, 
                                      &geom->child_count, GASNETE_COLL_REV_RECURSIVE);
    break;
    
  default:
    gasneti_fatalerror("UNKNOWN TREE TYPE\n");
  }
  geom->root = root;
  geom->tree_type.tree_class = class;
  geom->tree_type.fanout = fanout;
  return geom;

  
}
#undef GASNETE_COLL_DFS_RECURSIVE
#undef GASNETE_COLL_REV_RECURSIVE

#undef GASNETE_COLL_ACT2REL
#undef GASNETE_COLL_REL2ACT

gasnet_node_t gasnete_coll_get_sub_tree_size(gasnete_coll_local_tree_geom_t *geom, int subtreeroot, int gasnete_coll_tree_nodes) {
  gasnete_coll_local_tree_geom_t *temp;
  int i;
  int ret = 1;
  /* for each child recursively run the depth first search */
  temp = gasnete_coll_build_tree(geom->tree_type.tree_class, geom->tree_type.fanout, geom->root, 
                                 subtreeroot, gasnete_coll_tree_nodes, 1);
  
  for(i=0; i<temp->child_count; i++) {
    ret += gasnete_coll_get_sub_tree_size(geom, temp->child_list[i], gasnete_coll_tree_nodes);
  }
  gasneti_free(temp->child_list);
  gasneti_free(temp);
  return ret;
  
}

void gasnete_coll_set_sub_tree_sizes(gasnete_coll_local_tree_geom_t *geom, int gasnete_coll_tree_mynode, int gasnete_coll_tree_nodes) {
  int i;
  gasnete_coll_local_tree_geom_t *temp;

  if(geom->child_count > 0) {
    geom->subtree_sizes = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*geom->child_count);
    for(i=0; i<geom->child_count; i++) {
      geom->subtree_sizes[i] = gasnete_coll_get_sub_tree_size(geom, geom->child_list[i], gasnete_coll_tree_nodes);
    }
  } else {
    geom->subtree_sizes = NULL;
  }
  if(geom->root!=gasnete_coll_tree_mynode) {
    geom->parent_subtree_size = 1;
    temp = gasnete_coll_build_tree(geom->tree_type.tree_class, geom->tree_type.fanout, geom->root, 
                                   geom->parent, gasnete_coll_tree_nodes, 1);
    for(i=0; i<temp->child_count; i++) {
      geom->parent_subtree_size += gasnete_coll_get_sub_tree_size(geom, temp->child_list[i], gasnete_coll_tree_nodes);
    }
    gasneti_free(temp->child_list);
    gasneti_free(temp);
  } else {
    geom->parent_subtree_size = 0;
  }
  geom->mysubtree_size = 1;
  for(i=0; i<geom->child_count; i++) {
    geom->mysubtree_size += geom->subtree_sizes[i];
  }
}

/* these functions should only be called by the root of the geom object*/
void gasnete_coll_set_dfs_order_helper(gasnete_coll_local_tree_geom_t *geom, int subtreeroot, int gasnete_coll_tree_nodes,
                                                gasnet_node_t *lst, gasnet_node_t *pos) {
  int num_added = 0, i;
  gasnete_coll_local_tree_geom_t *temp;
  gasneti_assert(*pos <= gasnete_coll_tree_nodes);
  lst[*pos] = subtreeroot;
  (*pos)++;
  
  temp = gasnete_coll_build_tree(geom->tree_type.tree_class, geom->tree_type.fanout, geom->root, 
                                 subtreeroot, gasnete_coll_tree_nodes, 1);
  for(i=0; i<temp->child_count; i++) {
    gasnete_coll_set_dfs_order_helper(geom, temp->child_list[i], gasnete_coll_tree_nodes, lst, pos);
  }
  gasneti_free(temp->child_list);
  gasneti_free(temp);
}


void gasnete_coll_set_dfs_order(gasnete_coll_local_tree_geom_t *geom, int gasnete_coll_tree_nodes) {
  int i;
  gasnet_node_t pos = 0;
  gasneti_assert(geom->root == gasneti_mynode);
  
  if(geom->child_count > 0) {
    geom->dfs_order = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*gasnete_coll_tree_nodes);
    geom->dfs_order[pos] = geom->root;
    pos++;
    for(i=0; i<geom->child_count; i++) {
      gasnete_coll_set_dfs_order_helper(geom, geom->child_list[i], gasnete_coll_tree_nodes, geom->dfs_order, &pos);
    }
  } else {
    geom->dfs_order = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t)*1);
    geom->dfs_order[0] = gasneti_mynode;
  }
}

#if 1
gasnet_node_t* gasnete_coll_get_sibling_list(gasnete_coll_local_tree_geom_t *geom, gasnet_node_t gasnete_coll_tree_mynode, gasnet_node_t gasnete_coll_tree_nodes, gasnet_node_t *num_siblings, gasnet_node_t *sibling_id) {
  gasnete_coll_local_tree_geom_t *temp;
  int i;
  gasnet_node_t *ret_list;
  int flag =0;
  
  if(gasnete_coll_tree_mynode!=geom->root) {
    /*build a temporary tree with our parent as the root*/
    temp = gasnete_coll_build_tree(geom->tree_type.tree_class, geom->tree_type.fanout, geom->root,
                                   geom->parent, gasnete_coll_tree_nodes, 1);
    gasnete_coll_set_sub_tree_sizes(temp, geom->parent, gasnete_coll_tree_nodes); 
    
    /*use the resultant tree to deduce the children (which are our siblings)*/
    *num_siblings = temp->child_count;
    
    ret_list = (gasnet_node_t*) gasneti_malloc(sizeof(gasnet_node_t) * (*num_siblings));
    
    /*create deep copy of sibling_list*/
    memcpy(ret_list, temp->subtree_sizes, sizeof(gasnet_node_t)*(*num_siblings));
    
    /*by definition there will be a child list since i asked for the parents child list*/
    /*thus since i am a child of my parent this list will be not null*/
 
    flag =0;
    for(i=0; i<(*num_siblings); i++) {
      if(gasnete_coll_tree_mynode == temp->child_list[i]) {
	*sibling_id = i;
        flag = 1;
	break;
      }
    }
    gasneti_free(temp->child_list);
    gasneti_free(temp->subtree_sizes);
    
    if(flag == 0) {
      gasneti_fatalerror("%d> FATAL TREE ERROR: I am not in my parents child list\n", (int)gasnete_coll_tree_mynode);
      
    }
    gasneti_free(temp);
    
  } else {
    *sibling_id = 0;
    *num_siblings = 0;
    ret_list = NULL;
  }
  return ret_list;
}

void gasnete_coll_set_sibling_info(gasnete_coll_local_tree_geom_t *geom, int gasnete_coll_tree_mynode, int gasnete_coll_tree_nodes) {
  gasnet_node_t *list;
  int i;
  list =gasnete_coll_get_sibling_list(geom, gasnete_coll_tree_mynode, gasnete_coll_tree_nodes, &(geom->num_siblings), &(geom->sibling_id));

  geom->sibling_offset = 0;
  for(i=0; i<geom->sibling_id; i++) {
    geom->sibling_offset += list[i];
  }
  if(geom->num_siblings > 0) {
    gasneti_free(list);
  }
}

#endif


/* create a local view of the tree */
/* args: tree_type: what type hte tree is
		 root: the root relative to this team
				 thus if the members of this team are  1 2 4 8 9 and we want a tree rooted at 4 we'd need to pass in 3
		 a team argument
*/
gasnete_coll_local_tree_geom_t *gasnete_coll_tree_geom_create_local(gasnete_coll_tree_type_t in_type, int rootrank, gasnete_coll_team_t team)  {
	 gasnete_coll_local_tree_geom_t* geom;
  #if GASNET_COLL_TREE_DEBUG
  fprintf(stderr, "%d> setting up tree geom\n", gasneti_mynode);
#endif  
   geom = gasnete_coll_build_tree(in_type.tree_class, in_type.fanout, rootrank, team->myrank, team->total_ranks, 1);

   geom->allocated = 1;   
   geom->total_size = team->total_ranks;
   /*gasnete_coll_set_dissemination_order(geom, gasneti_mynode, gasneti_nodes);*/ 
   gasnete_coll_set_sub_tree_sizes(geom, team->myrank, team->total_ranks); 
   if(gasneti_mynode == rootrank) {
      gasnete_coll_set_dfs_order(geom, team->total_ranks); 
   } 
   
   /* XXX: need to do proper discovery and broadcast of seq_dfs_order*/
   /* For Now assume that only flat and chain trees have this property*/
   if(geom->root == 0 && geom->tree_type.tree_class == GASNETE_COLL_NARY_TREE && 
      (geom->tree_type.fanout >= team->total_ranks-1 || geom->tree_type.fanout == 1)) {
    geom->seq_dfs_order = 1;
   } else {
    geom->seq_dfs_order = 0;
   }
     
   gasnete_coll_set_sibling_info(geom, team->myrank, team->total_ranks); 
#if 0
   gasnete_coll_print_tree(geom, gasneti_mynode);
#endif

   return geom;
}


/*---------------------------------------------------------------------------------*/
/* Operations to access the tree geometry cache */

uint32_t gasnete_coll_pipe_seg_size = 1024;

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
static gasnete_coll_tree_geom_t *gasnete_coll_tree_geom_fetch_helper(gasnete_coll_tree_type_t in_type, gasnete_coll_tree_geom_t *geom_cache) {
  gasnete_coll_tree_geom_t *curr_geom = geom_cache;
  while(curr_geom != NULL) {
	if(curr_geom->tree_type.tree_class == in_type.tree_class) {
		if(in_type.tree_class == GASNETE_COLL_BINOMIAL_TREE || curr_geom->tree_type.fanout == in_type.fanout) 
			return curr_geom;
		else
			curr_geom = curr_geom->next;
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
  
  curr_geom = gasnete_coll_tree_geom_fetch_helper(type, geom_cache_head);
  if(curr_geom == NULL) {
    int i;
#if 0
    if(gasneti_mynode ==0) fprintf(stderr, "%d> new tree: %d type %d fanout\n",gasneti_mynode, type.tree_class, type.fanout);
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
#if 0
      if(gasneti_mynode==0) fprintf(stderr, "%d> tree found: %d type %d fanout\n", gasneti_mynode, type.tree_class, type.fanout);
      if(gasneti_mynode==0) fprintf(stderr, "%d> new root: %d\n", gasneti_mynode, root); 
#endif
      curr_geom->local_views[root] = gasnete_coll_tree_geom_create_local(type, root, team);
#if 0		  
      /* create all the local views */
      int i;
      for(i=0; i<team->total_ranks; i++) {
	/* some local views might have already been allocated 
	   through other intermediary steps*/
	if(curr_geom->local_views[i] == NULL) {
	  curr_geom->local_views[i] = gasnete_coll_tree_geom_create_local(type i, team);
	  
	} 
      }
#endif
    }
#if 0
    if(gasneti_mynode==0) fprintf(stderr, "%d> tree found: type: %d fanout: %d root: %d\n", gasneti_mynode, type.tree_class, type.fanout,root);
#endif
    ret = curr_geom->local_views[root];
  }
  
#ifdef GASNETC_HAVE_AMRDMA
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
	team->dissem_cache_tail = temp;  
    }
    return temp;
}

void gasnete_coll_release_dissemination(gasnete_coll_dissem_info_t *obj, gasnete_coll_team_t team) {
    /* do nothing for now */
}
