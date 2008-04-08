/* $Id: groups.c,v 1.4 2004-09-08 00:42:13 manoj Exp $ */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MPI
#  define MPI
#endif
#include "armcip.h"
#include "message.h"

#define DEBUG_ 0

void ARMCI_Bcast_(void *buffer, int len, int root, ARMCI_Comm comm);
void ARMCI_Group_create(int n, int *pid_list, ARMCI_Group *group);
int ARMCI_Group_rank(ARMCI_Group *group, int *rank);
void ARMCI_Group_size(ARMCI_Group *group, int *size);

static void get_group_clus_id(ARMCI_iGroup *igroup, int grp_nproc, 
                              int *grp_clus_id) {
    int i, *ranks1, *ranks2;
    MPI_Group group2;
    
    /* Takes the list of processes from one group and attempts to determine
     * the corresponding ranks in a second group (here, MPI_COMM_WORLD) */

    ranks1 = (int *)malloc(2*grp_nproc*sizeof(int));
    ranks2 = ranks1 + grp_nproc;
    for(i=0; i<grp_nproc; i++) ranks1[i] = i;
    MPI_Comm_group(MPI_COMM_WORLD, &group2);
    MPI_Group_translate_ranks(igroup->igroup, grp_nproc, ranks1, group2, ranks2);
    
    /* get the clus_id of processes */
    for(i=0; i<grp_nproc; i++) grp_clus_id[i] = armci_clus_id(ranks2[i]);
    free(ranks1);
}

/**
 * Group cluster information "grp_clus_info" (similar to armci_clus_info)
 */
static void group_process_list(ARMCI_Group *group, 
                               armci_grp_attr_t *grp_attr) {
    ARMCI_iGroup *igroup = (ARMCI_iGroup *)group;
    ARMCI_Comm comm = igroup->icomm;

    int grp_me, grp_nproc, grp_nclus, *grp_clus_id, grp_clus_me;
    armci_clus_t *grp_clus_info=NULL;
#ifdef CLUSTER
    int i, len, clus_id, cluster=0, root=0;
#endif
    
    if(comm==MPI_COMM_NULL || igroup->igroup==MPI_GROUP_NULL) 
       armci_die("group_process_list: NULL COMMUNICATOR",0);
    
    ARMCI_Group_rank(group, &grp_me);
    ARMCI_Group_size(group, &grp_nproc);
    
#ifdef CLUSTER
    /* process 0 gets group cluster information: grp_nclus, grp_clus_info */
    if(grp_me == 0) {
       
       /* get the cluster_id of processes in the group */
       grp_clus_id = (int *)malloc(grp_nproc*sizeof(int));
       get_group_clus_id(igroup, grp_nproc, grp_clus_id);
       
       /* first find out how many cluster nodes we got for this group */
       grp_nclus=1;
       for(i=0; i<grp_nproc-1; i++) {
          if(grp_clus_id[i] != grp_clus_id[i+1]) ++grp_nclus;
       }
       
       grp_clus_info = (armci_clus_t*)malloc(grp_nclus*sizeof(armci_clus_t));
       if(!grp_clus_info)armci_die("malloc failed for grp_clusinfo",grp_nclus);

       cluster = 1;
       clus_id = grp_clus_id[0];
       grp_clus_info[0].nslave = 1;
       grp_clus_info[0].master = 0;
       strcpy(grp_clus_info[0].hostname, armci_clus_info[clus_id].hostname);

       for(i=1; i<grp_nproc; i++) {
          if(grp_clus_id[i-1] == grp_clus_id[i]) 
             ++grp_clus_info[cluster-1].nslave;
          else {
             clus_id = grp_clus_id[i];
             grp_clus_info[cluster].nslave = 1;
             grp_clus_info[cluster].master = i;
             strcpy(grp_clus_info[cluster].hostname, 
                    armci_clus_info[clus_id].hostname);
             ++cluster;
          }
       }

       free(grp_clus_id);
       if(grp_nclus != cluster)
          armci_die("inconsistency processing group clusterinfo", grp_nclus);

#   if DEBUG_
       {
          int i,j;
          for(i=0; i<cluster;i++) {
             printf("%d: Cluster %d: Master=%d, #Slaves=%d, HostName=%s\n", 
                    grp_nclus, i, grp_clus_info[i].master, 
                    grp_clus_info[i].nslave, grp_clus_info[i].hostname);
             fflush(stdout);
          }
       }
#   endif
    }

    /* process 0 broadcasts group cluster information */
    len = sizeof(int);
    ARMCI_Bcast_(&grp_nclus, len, root, comm);
    
    if(grp_me){
       /* allocate memory */
       grp_clus_info = (armci_clus_t*)malloc(grp_nclus*sizeof(armci_clus_t));
       if(!armci_clus_info)armci_die("malloc failed for clusinfo",armci_nclus);
    }
    
    len = sizeof(armci_clus_t)*grp_nclus;
    ARMCI_Bcast_(grp_clus_info, len, root, comm);

    /* determine current group cluster node id by comparing me to master */
    grp_clus_me =  grp_nclus-1;
    for(i =0; i< grp_nclus-1; i++)
       if(grp_me < grp_clus_info[i+1].master){
          grp_clus_me=i;
          break;
       }

#else
    grp_clus_me = 0;
    grp_nclus = 1;
    grp_clus_info = (armci_clus_t*)malloc(grp_nclus*sizeof(armci_clus_t));
    if(!grp_clus_info)armci_die("malloc failed for clusinfo",grp_nclus);
    strcpy(grp_clus_info[0].hostname, armci_clus_info[0].hostname);
    grp_clus_info[0].master=0;
    grp_clus_info[0].nslave=grp_nproc;
#endif
    grp_attr->grp_me        = grp_me;
    grp_attr->grp_clus_info = grp_clus_info;
    grp_attr->grp_nclus     = grp_nclus;
    grp_attr->grp_clus_me   = grp_clus_me;
}

/* attribute caching: group_cluster_information and memory_offset should 
   be cached in group data structure */
static void armci_cache_attr(ARMCI_Group *group) {
    armci_grp_attr_t *grp_attr;
    ARMCI_iGroup *igroup = (ARMCI_iGroup *)group;

    /* allocate storage for the attribute content. Note: Attribute contents 
       should be stored in persistent memory */
    grp_attr = &(igroup->grp_attr); 
    
    /* get group cluster information and  grp_attr */
    group_process_list(group, grp_attr);
}

armci_grp_attr_t *ARMCI_Group_getattr(ARMCI_Group *group)
{
    ARMCI_iGroup *igroup = (ARMCI_iGroup *)group;
    return(&(igroup->grp_attr));

}

void ARMCI_Bcast_(void *buffer, int len, int root, ARMCI_Comm comm) {
    int result;
    MPI_Comm_compare(comm, MPI_COMM_WORLD, &result);
    if(result == MPI_IDENT)  armci_msg_brdcst(buffer, len, root); 
    else MPI_Bcast(buffer, len, MPI_BYTE, root, (MPI_Comm)comm);
}

/* NOTE: To startwith,MPI_COMM_WORLD is parent group for every group created */
void ARMCI_Group_create(int n, int *pid_list, ARMCI_Group *group) {
    int i,grp_me;
    ARMCI_iGroup *igroup = (ARMCI_iGroup *)group;
    MPI_Group group_world;
    for(i=0; i<n-1;i++) {
       if(pid_list[i] > pid_list[i+1]){
         armci_die("ARMCI_Group_create: Process ids are not sorted ",armci_me);
         break;
       }
    }
    MPI_Comm_group(MPI_COMM_WORLD, &group_world);
    MPI_Group_incl(group_world, n, pid_list, &(igroup->igroup));
    MPI_Comm_create(MPI_COMM_WORLD,(MPI_Group)(igroup->igroup), 
                    (MPI_Comm*)&(igroup->icomm));
    MPI_Group_rank((MPI_Group)(igroup->igroup), &grp_me);
    /* processes belong to this group should cache attributes */
    if(grp_me != MPI_UNDEFINED) armci_cache_attr(group);
}

/*
void ARMCI_Comm_group_(ARMCI_Comm comm, ARMCI_Group *group) {
    ARMCI_iGroup *igroup = (ARMCI_iGroup *)group;
    MPI_Comm_group((MPI_Comm)comm, (MPI_Group*)&(igroup->igroup));
}
*/

int ARMCI_Group_rank(ARMCI_Group *group, int *rank) {
    ARMCI_iGroup *igroup = (ARMCI_iGroup *)group;
    return MPI_Group_rank((MPI_Group)(igroup->igroup), rank);
}

void ARMCI_Group_size(ARMCI_Group *group, int *size) {
    ARMCI_iGroup *igroup = (ARMCI_iGroup *)group;
    MPI_Group_size((MPI_Group)(igroup->igroup), size);
}

int ARMCI_Absolute_id(ARMCI_Group *group,int group_rank)
{
    int abs_rank,status;
    MPI_Group grp;
    ARMCI_iGroup *igroup = (ARMCI_iGroup *)group;
    status = MPI_Comm_group(MPI_COMM_WORLD,&grp);
    MPI_Group_translate_ranks(igroup->igroup,1,&group_rank,grp,&abs_rank);
    return(abs_rank);
}

/*
  ISSUES:
  1. Make sure ARMCI_Comm_free frees the attribute data structures 
  2. replace malloc with, kr_malloc using local_context.
*/
