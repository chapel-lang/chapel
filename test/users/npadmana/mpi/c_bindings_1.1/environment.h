/* Environmental Inquiry */
#include "mpi_dummy.h"

int MPI_Get_processor_name(char *name, int *resultlen);  
////int MPI_Errhandler_create(MPI_Handler_function *function, MPI_Errhandler *errhandler);  
int MPI_Errhandler_set(MPI_Comm comm, MPI_Errhandler errhandler);  
int MPI_Errhandler_get(MPI_Comm comm, MPI_Errhandler *errhandler);  
//int MPI_Errhandler_free(MPI_Errhandler *errhandler);  
int MPI_Error_string(int errorcode, char *string, int *resultlen);  
int MPI_Error_class(int errorcode, int *errorclass);  
double MPI_Wtime();  
double MPI_Wtick();  
////int MPI_Init(int *argc, char ***argv);  
int MPI_Finalize();  
int MPI_Initialized(int *flag);  
int MPI_Abort(MPI_Comm comm, int errorcode);  
