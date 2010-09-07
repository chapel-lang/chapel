/*   $Source: /var/local/cvs/gasnet/other/smp-collectives/smp_coll_dissem.c,v $
 *     $Date: 2009/10/22 20:24:55 $
 * $Revision: 1.3 $
 * Description: Shared Memory Collectives
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <smp-collectives/smp_coll_dissem.h>

void smp_coll_print_dissem_order(dissem_info_t *obj, int MYTHREAD) {
  int i;
  int j;
  FILE *fp;
  char buffer[100];
  
  sprintf(buffer, "./outfiles/dissem_order_%d.txt", MYTHREAD);
  
  fp = fopen(buffer, "a");
  
  for(i=0; i<obj->dissemination_phases; i++) {
    if(obj->barrier_order[i].n > 0) {
      for(j=0; j<obj->barrier_order[i].n; j++) {
	fprintf(fp, "%d> radix %d, step %d, peer %d of %d is %d\n", MYTHREAD, obj->dissemination_radix, i, j, obj->barrier_order[i].n, obj->barrier_order[i].elem_list[j]); 
      }
    } else {
      fprintf(fp, "%d> radix %d, step %d, no peers\n", MYTHREAD, obj->dissemination_radix, i);
    }
  }

  fclose(fp);
}

void smp_coll_dump_dissem_order(dissem_info_t *obj, int MYTHREAD) {
  int i;
  int j;

  char buffer[100];

  for(i=0; i<obj->dissemination_phases; i++) {
    if(obj->barrier_order[i].n > 0) {
      for(j=0; j<obj->barrier_order[i].n; j++) {
	fprintf(stdout, "%d> radix %d, step %d, peer %d of %d is %d\n", MYTHREAD, obj->dissemination_radix, i, j, obj->barrier_order[i].n, obj->barrier_order[i].elem_list[j]); 
      }
    } else {
      fprintf(stdout, "%d> radix %d, step %d, no peers\n", MYTHREAD, obj->dissemination_radix, i);
    }
  }


}

dissem_info_t *smp_coll_build_dissemination(int r, int MYTHREAD, int THREADS) {
  dissem_info_t *ret;
  int h,w,i,j,distance,x,numpeers,destproc;
  ret = (dissem_info_t*) gasneti_malloc(sizeof(dissem_info_t));
  
  w = smp_coll_mylogn(THREADS, r);
  ret->dissemination_radix = r;
  ret->dissemination_phases = w;

  

  ret->barrier_order = (dissem_vector_t*) gasneti_malloc(sizeof(dissem_vector_t)*ret->dissemination_phases);


  distance = 1;
  /* phase 2: communication in log_r(THREADS) steps*/
  for(i=0; i<w; i++) {
    if(i==(w-1)) {
      /*h = ceil(THREADS/DIST);*/
      h = THREADS/distance;
      if(THREADS % distance != 0) 
	h++;
    } else {
      h = r;
    }
    ret->barrier_order[i].n = h-1;
    ret->barrier_order[i].elem_list = (int*) gasneti_malloc(sizeof(int)*(h-1));
    for(j=1; j<h; j++) {
      ret->barrier_order[i].elem_list[j-1] = (MYTHREAD + j*distance) % THREADS;
    }
    /*scale the dissemination radix*/
    distance *= r;
  }
  
 
  if(r == 2 && POWEROFTWO(THREADS)) {
    ret->all_reduce_ok = 1;
    ret->all_reduce_order = (dissem_vector_t*)gasneti_malloc(sizeof(dissem_vector_t)*w);
    
    distance = THREADS;
    
    for(i=0; i<w; i++) {
      j = (MYTHREAD + (distance/2))%distance;
      
      ret->all_reduce_order[i].n = 2;
      /*in each phase position[0] contains where i send to*/
      
      ret->all_reduce_order[i].elem_list = (int*) gasneti_malloc(sizeof(int)*1);
      ret->all_reduce_order[i].elem_list[0] = j + (MYTHREAD / distance) * distance;
      distance = distance / 2;
    }
  } else {
    ret->all_reduce_ok = 0;
    ret->all_reduce_order = NULL;
  }
  


  ret->max_dissem_blocks =(THREADS/ret->dissemination_radix);
  for(i=0; i<w; i++) {
    int curr_count = 0;
    for(j=0; j<THREADS; j++) {
      if( ((j / smp_coll_mypown(ret->dissemination_radix, i)) % ret->dissemination_radix) 
	  == 1) curr_count++; 
    }
    ret->max_dissem_blocks=MAX(ret->max_dissem_blocks, curr_count);
  }


  return ret;
}


void smp_coll_free_dissemination(dissem_info_t* obj) {
  
  int i;
  for(i=0; i<obj->dissemination_phases; i++) {
    if(obj->barrier_order[i].n>0) gasneti_free(obj->barrier_order[i].elem_list);
  }
  gasneti_free(obj->barrier_order);
  if(obj->all_reduce_order !=NULL ) {
    for(i=0; i<obj->dissemination_phases; i++) {
      if(obj->all_reduce_order[i].n>0) gasneti_free(obj->all_reduce_order[i].elem_list);
    }
    gasneti_free(obj->all_reduce_order);
  }
  
  
  gasneti_free(obj);
}
