/* Groups, Contexts, Communicators */
#include "mpi_dummy.h"

int MPI_Group_size(MPI_Group group, int *size);  
int MPI_Group_rank(MPI_Group group, int *rank);  
int MPI_Group_translate_ranks (MPI_Group group1, int n, int *ranks1, MPI_Group group2, int *ranks2);  
int MPI_Group_compare(MPI_Group group1,MPI_Group group2, int *result);  
int MPI_Comm_group(MPI_Comm comm, MPI_Group *group);  
int MPI_Group_union(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);  
int MPI_Group_intersection(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);  
int MPI_Group_difference(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup);  
int MPI_Group_incl(MPI_Group group, int n, int *ranks, MPI_Group *newgroup);  
int MPI_Group_excl(MPI_Group group, int n, int *ranks, MPI_Group *newgroup);  
//int MPI_Group_range_incl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);  
//int MPI_Group_range_excl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup);  
int MPI_Group_range_incl(MPI_Group group, int n, int ranges[], MPI_Group *newgroup);  
int MPI_Group_range_excl(MPI_Group group, int n, int ranges[], MPI_Group *newgroup);  
// The above two are changed to parse correctly
int MPI_Group_free(MPI_Group *group);  
int MPI_Comm_size(MPI_Comm comm, int *size);  
int MPI_Comm_rank(MPI_Comm comm, int *rank);  
int MPI_Comm_compare(MPI_Comm comm1,MPI_Comm comm2, int *result);  
int MPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm);  
int MPI_Comm_create(MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm);  
int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm);  
int MPI_Comm_free(MPI_Comm *comm);  
int MPI_Comm_test_inter(MPI_Comm comm, int *flag);  
int MPI_Comm_remote_size(MPI_Comm comm, int *size);  
int MPI_Comm_remote_group(MPI_Comm comm, MPI_Group *group);  
int MPI_Intercomm_create(MPI_Comm local_comm, int local_leader, MPI_Comm peer_comm, int remote_leader, int tag, MPI_Comm *newintercomm);  
int MPI_Intercomm_merge(MPI_Comm intercomm, int high, MPI_Comm *newintracomm);  
//int MPI_Keyval_create(MPI_Copy_function *copy_fn, MPI_Delete_function *delete_fn, int *keyval, void* extra_state);  
//int MPI_Keyval_free(int *keyval);  
int MPI_Attr_put(MPI_Comm comm, int keyval, void* attribute_val);  
int MPI_Attr_get(MPI_Comm comm, int keyval, void* attribute_val, int *flag);  
int MPI_Attr_delete(MPI_Comm comm, int keyval);  
