/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testhello.c $
 * Description: GASNet "Hello, World" test/example
 * Copyright 2010, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet.h>
#include <stdio.h>

/* Macro to check return codes and terminate with useful message. */
#define GASNET_SAFE(fncall) do {                                     \
    int _retval;                                                     \
    if ((_retval = fncall) != GASNET_OK) {                           \
      fprintf(stderr, "ERROR calling: %s\n"                          \
                      " at: %s:%i\n"                                 \
                      " error: %s (%s)\n",                           \
              #fncall, __FILE__, __LINE__,                           \
              gasnet_ErrorName(_retval), gasnet_ErrorDesc(_retval)); \
      fflush(stderr);                                                \
      gasnet_exit(_retval);                                          \
    }                                                                \
  } while(0)

int main(int argc, char **argv)
{
  gasnet_node_t rank, size;
  size_t segsz = GASNET_PAGESIZE;
  size_t heapsz = GASNET_PAGESIZE;
  int argi;

  GASNET_SAFE(gasnet_init(&argc, &argv));
  rank = gasnet_mynode();
  size = gasnet_nodes();

  argi = 1;
  if ((argi < argc) && !strcmp(argv[argi], "-m")) {
    segsz = gasnet_getMaxLocalSegmentSize();
    ++argi;
  }
    
  GASNET_SAFE(gasnet_attach(NULL, 0, segsz, heapsz));

  /* Only first and last print here, to keep managable I/O volume at scale */
  if (!rank || (rank == size-1))
    printf("Hello from node %d of %d\n", (int)rank, (int)size);

  /* Spec says client should include a barrier before gasnet_exit() */
  gasnet_barrier_notify(0,GASNET_BARRIERFLAG_ANONYMOUS);
  gasnet_barrier_wait(0,GASNET_BARRIERFLAG_ANONYMOUS);

  gasnet_exit(0);

  /* Not reached in most implementations */
  return 0;
}
