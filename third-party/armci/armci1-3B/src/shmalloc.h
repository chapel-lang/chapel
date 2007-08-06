#ifndef SHMALLOC_H
#define SHMALLOC_H

/*
  Returns data aligned on a quad boundary. Even if the request
  size is zero it returns a non-zero pointer.
*/
extern char *armci_shmalloc(size_t size);

/*
  void shfree(char *pointer)
  Frees memory allocated by armci_shmalloc(). Ignores NULL pointers
  but must not be called twice for the same pointer or called
  with non-shmalloc'ed pointers
*/
extern void  shfree();

/*
  void shmalloc_print_stats();
  Print to standard output the usage statistics ... a wrapper
  for shmalloc_stats();
*/
extern void shmalloc_print_stats();

/*
  void shmalloc_stats(size_t *total, long *nchunk,
                      size_t *inuse, size_t *maxuse,
		      long *nfrags, long *nmcalls, long *nfcalls)
  Returns the statistics about memory usage.
  total  = total amount of memory got from system in bytes
  nchunk = total no. of chunks of memory got from the system
  inuse  = amount of memory application is using in bytes
  maxuse = maximum value of inuse to dat
  nfrags = no. of fragments that memory is split into
  nmcalls= no. of calls to shmalloc
  nfcalls= no. of calls to shfree
*/
extern void  shmalloc_stats();

/*
  void shmalloc_debug(int code)
  Enable debuging
  code  = 0 ... no debugging
  code != 0 ... entire heap is verified on every call
*/
extern void  shmalloc_debug();

/*
  Verify the heap.
*/
extern void shmalloc_verify();

/*
  size ... minimum size in bytes that chunks of data should be
           obtained from the system in.  The default is 131072.

  maxsize ... maximum amount of memory that should be obtained
              from the system.  The default is 12,582,912.
*/
extern void shmalloc_request(size_t size, size_t maxsize);

#endif
