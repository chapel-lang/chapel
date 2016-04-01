/* Process Topologies */
#include "mpi_dummy.h"

int MPI_Cart_create(MPI_Comm comm_old, int ndims, int *dims, int *periods, int reorder, MPI_Comm *comm_cart);  
int MPI_Dims_create(int nnodes, int ndims, int *dims);  
int MPI_Graph_create(MPI_Comm comm_old, int nnodes, int *index, int *edges, int reorder, MPI_Comm *comm_graph);  
int MPI_Topo_test(MPI_Comm comm, int *status);  
int MPI_Graphdims_get(MPI_Comm comm, int *nnodes, int *nedges);  
int MPI_Graph_get(MPI_Comm comm, int maxindex, int maxedges, int *index, int *edges);  
int MPI_Cartdim_get(MPI_Comm comm, int *ndims);  
int MPI_Cart_get(MPI_Comm comm, int maxdims, int *dims, int *periods, int *coords);  
int MPI_Cart_rank(MPI_Comm comm, int *coords, int *rank);  
int MPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int *coords);  
int MPI_Graph_neighbors_count(MPI_Comm comm, int rank, int *nneighbors);  
int MPI_Graph_neighbors(MPI_Comm comm, int rank, int maxneighbors, int *neighbors);  
int MPI_Cart_shift(MPI_Comm comm, int direction, int disp, int *rank_source, int *rank_dest);  
int MPI_Cart_sub(MPI_Comm comm, int *remain_dims, MPI_Comm *newcomm);  
int MPI_Cart_map(MPI_Comm comm, int ndims, int *dims, int *periods, int *newrank);  
int MPI_Graph_map(MPI_Comm comm, int nnodes, int *index, int *edges, int *newrank);  
