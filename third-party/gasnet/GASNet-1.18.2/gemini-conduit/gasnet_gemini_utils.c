#include <stdint.h>
#include <assert.h>
#include <gasnet_gemini.h>
#include <pmi_cray.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <gni_pub.h>

#ifndef MPI_SUCCESS
#define MPI_SUCCESS 0
#endif

static long int mygetenv(const char *name)
{
  char *valuestring = getenv(name);
  long int value;
  if (valuestring == NULL) return (-1);
  value = atol(valuestring);
  /*fprintf(stderr, "rank %d, get(%s) -> %s, %ld\n", rank, name, valuestring, value);*/
  return(value);
}

unsigned int *gasnetc_UGNI_AllAddr;

void *gasnetc_gather_nic_addresses(void)
{
  gni_return_t status;
  uint32_t myaddress, pmiaddress;
  uint32_t cpu_id;
  uint32_t device_id = gasnetc_GNIT_Device_Id();
  gasnetc_UGNI_AllAddr = gasneti_malloc(gasneti_nodes * sizeof(uint32_t));
  gasneti_assert(gasnetc_UGNI_AllAddr);
  status = GNI_CdmGetNicAddress(device_id, &myaddress, &cpu_id);
  if (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort();
  } else {
    /*fprintf(stderr, "rank %d Cdm_GetNicAddress %x\n", rank, myaddress); */
  }
  pmiaddress = mygetenv("PMI_GNI_LOC_ADDR");
  if (pmiaddress != myaddress) {
#if GASNETC_DEBUG
    fprintf(stderr, "rank %d PMI_GNI_LOC_ADDR is %d, using it\n", gasneti_mynode, pmiaddress);
#endif
    myaddress = pmiaddress;
  } else {
    /*fprintf(stderr, "rank %d PMI_GNI_LOC_ADDR is %d, same\n", rank, pmiaddress);*/
  }

  gasnetc_GNIT_Allgather(&myaddress, sizeof(uint32_t), gasnetc_UGNI_AllAddr);
  if (gasnetc_UGNI_AllAddr[gasneti_mynode] != myaddress) {
    fprintf(stderr, "rank %d gathernic got %x should be %x\n", gasneti_mynode,
	    gasnetc_UGNI_AllAddr[gasneti_mynode], myaddress);
    gasnetc_GNIT_Abort();
  }
  /*
  {
  int i;
  fprintf(stderr, "rank %d addresses ", rank);
  for (i = 0; i < gasneti_nodes; i += 1) {
    fprintf(stderr, " %x", gasnetc_UGNI_AllAddr[i]);
  }
  fprintf(stderr, "\n");
  }
  */
  return(gasnetc_UGNI_AllAddr);
}


int gasnetc_gem_init(char **errorstringp)
{
   int size, rank;
   int spawned;
   int appnum;
   int ret;

   if ((ret = PMI2_Init (&spawned, &size, &rank, &appnum)) != MPI_SUCCESS) {
     *errorstringp = (char *) "Failure in PMI2_Init\n";
     return(GASNET_ERR_NOT_INIT);
   }
   gasneti_nodes = size;
   gasneti_mynode = rank;
   return(GASNET_OK);
}


/* Algorithm
 * Get the number of ranks on my supernode
 * Allocate an array of that size node numbers
 * Get the ranks of all local nodes
 * Find the smallest
 * AllGather that number so everyone has an array indexed by rank of the
 *  smallest rank on their supernode
 */



void gasnetc_GNIT_Job_size(int *nranks)
{
  *nranks = gasneti_nodes;
}

void gasnetc_GNIT_Rank(int *inst_id)
{
  *inst_id = gasneti_mynode;
}

char gasnetc_GNIT_Ptag(void)
{
  return(mygetenv("PMI_GNI_PTAG"));
}

int gasnetc_GNIT_Cookie(void)
{
  return(mygetenv("PMI_GNI_COOKIE"));
}

int gasnetc_GNIT_Device_Id(void)
{
  return(mygetenv("PMI_GNI_DEV_ID"));
}

void gasnetc_GNIT_Allgather(void *local, long length, void *global)
{
  long itemsize = 1 + 1 + (length / sizeof(long)); /* length in longs */
  long *unsorted = gasneti_malloc(itemsize * sizeof(long) * gasneti_nodes);
  long *sorted = gasneti_malloc(itemsize * sizeof(long) * gasneti_nodes);
  uint32_t peer;
  uint32_t i;
  int status;
  gasneti_assert(unsorted);
  gasneti_assert(sorted);
  /* use beginning of sorted for the local data */
  sorted[0] = gasneti_mynode;
  memcpy(&sorted[1], local, length);  
  /* initialize the unsorted array */
  for (i = 0; i < itemsize * gasneti_nodes; i += 1) {
    unsorted[i] = -1;
  }
  status = PMI_Allgather(sorted, unsorted, itemsize * sizeof(long));
  if (status != PMI_SUCCESS) {
    fprintf(stderr, "rank %d: PMI_Allgather failed\n", gasneti_mynode);
    gasnetc_GNIT_Abort();
  }
  for (i = 0; i < gasneti_nodes; i += 1) {
    peer = unsorted[i * itemsize];
    if (peer >= gasneti_nodes) {
      fprintf(stderr, "rank %d PMI_Allgather failed, item %d is %d\n", 
	      gasneti_mynode, i, peer);
      gasnetc_GNIT_Abort();
    }
    memcpy(&sorted[peer * itemsize], 
	   &unsorted[i * itemsize],
	   itemsize * sizeof(long));
  }
  for (i = 0; i < gasneti_nodes; i += 1) {
    if (sorted[i * itemsize] != i) {
      fprintf(stderr, "rank %d Allgather rank %d missing\n", gasneti_mynode, i);
      gasnetc_GNIT_Abort();
    }
    memcpy((void *) ((uintptr_t) global + (i * length)), &sorted[(i * itemsize) + 1], length);
  }
  /* check own data */
  if (memcmp(local, (void *) ((uintptr_t ) global + (gasneti_mynode * length)), length) != 0) {
    fprintf(stderr, "rank %d, allgather error\n", gasneti_mynode);
    gasnetc_GNIT_Abort();
  }
  gasneti_free(unsorted);
  gasneti_free(sorted);
}


void gasnetc_GNIT_TEST_SUCCESS(void)
{
  fprintf(stderr, "gasnetc_GNIT_TEST_SUCCESS called\n");
}

void gasnetc_GNIT_Finalize(void)
{
  PMI2_Finalize();
}


void gasnetc_GNIT_Barrier(void)
{
  PMI_Barrier();
}
