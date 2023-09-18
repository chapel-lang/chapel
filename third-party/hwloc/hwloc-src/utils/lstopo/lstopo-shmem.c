/*
 * Copyright © 2018-2020 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>
#include <private/private.h>

#include <hwloc.h>
#include <hwloc/shmem.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "lstopo.h"

struct lstopo_shmem_header {
#define LSTOPO_SHMEM_HEADER_VERSION 1U
  uint32_t header_version;
  uint32_t header_length;
  uint64_t mmap_address;
  uint64_t mmap_length;
  uint64_t file_offset;
};

extern uint64_t shmem_output_addr;

#ifndef MAP_ANONYMOUS /* for Mac OS X 9 */
#define MAP_ANONYMOUS MAP_ANON
#endif

static unsigned long
find_mmap_addr(unsigned long length)
{
  unsigned long addr;
  void *tmp_mmap;
  int err;

  /* try to find a good address starting from something in the middle of the entire/full address space */
#if SIZEOF_VOID_P == 8
  addr = 0x8000000000000000UL;
#else
  addr = 0x80000000UL;
#endif
  printf("Testing mmaps to find room for length %lu\n", length);

again:
  tmp_mmap = mmap((void*)(uintptr_t)addr, length, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, -1, 0);
  if (tmp_mmap != MAP_FAILED) {
    err = munmap((void*)(uintptr_t)tmp_mmap, length);
    assert(!err);
    if (tmp_mmap == (void*)(uintptr_t) addr) {
      /* worked! */
      printf(" test mmap at 0x%lx succeeded, let's use that!\n", addr);
      return addr;
    }
    printf(" test mmap at 0x%lx returned another address\n", addr);
  } else
    printf(" test mmap at 0x%lx failed (errno %d)\n", addr, errno);
  /* couldn't map there, try again with a smaller address */
  addr >>= 1;
  if (addr)
    goto again;

  return 0;
}

int
output_shmem(struct lstopo_output *loutput, const char *filename)
{
  struct lstopo_shmem_header header;
  size_t shmem_length;
  struct stat st;
  int fd, err;

  if (!filename || !strcasecmp(filename, "-.shmem")) {
    fprintf(stderr, "Cannot export shmem topology to stdout.\n");
    return -1;
  }

  if (!stat(filename, &st) && !loutput->overwrite) {
    fprintf(stderr, "Failed to export shmem topology to %s (%s)\n", filename, strerror(EEXIST));
    return -1;
  }

  err = hwloc_shmem_topology_get_length(loutput->topology, &shmem_length, 0);
  if (err < 0) {
    if (errno == ENOSYS)
      fprintf(stderr, "shmem topology not supported\n"); /* this line must match the grep line in test-lstopo-shmem */
    else
      fprintf(stderr, "Failed to compute shmem topology export length\n");
    return -1;
  }

  fd = open(filename, O_RDWR|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
  if (fd < 0) {
    fprintf(stderr, "Failed to open shmem topology file %s (%s)\n", filename, strerror(errno));
    return -1;
  }

  if (!loutput->shmem_output_addr)
    loutput->shmem_output_addr = find_mmap_addr(shmem_length);
  if (!loutput->shmem_output_addr) {
    fprintf(stderr, "Failed to find a shmem topology mmap address\n"); /* this line must match the grep line in test-lstopo-shmem */
    close(fd);
    unlink(filename);
    return -1;
  }

  header.header_version = LSTOPO_SHMEM_HEADER_VERSION;
  header.header_length = sizeof(header);
  header.mmap_address = loutput->shmem_output_addr;
  header.mmap_length = shmem_length;
  header.file_offset = hwloc_getpagesize();
  err = write(fd, &header, sizeof(header));
  if (err != sizeof(header)) {
    fprintf(stderr, "Failed to write shmem topology header\n");
    close(fd);
    unlink(filename);
    return -1;
  }

  if (hwloc_shmem_topology_write(loutput->topology, fd, header.file_offset, (void*)(uintptr_t)loutput->shmem_output_addr, shmem_length, 0) < 0) {
    if (errno == EBUSY)
      fprintf(stderr, "Failed to export shmem topology, memory range is busy\n"); /* this line must match the grep line in test-lstopo-shmem */
    else
      fprintf(stderr, "Failed to export shmem topology to %s (%s)\n", filename, strerror(errno));
    close(fd);
    unlink(filename);
    return -1;
  }

  close(fd);

  printf("Exported shmem topology to %s for mmap address 0x%llx length %lu\n", filename, (unsigned long long) loutput->shmem_output_addr, (unsigned long) shmem_length);

  return 0;
}

int lstopo_shmem_adopt(const char *input, hwloc_topology_t *topologyp)
{
  hwloc_topology_t adopted;
  struct lstopo_shmem_header header;
  int fd, err;

  fd = open(input, O_RDONLY);
  if (fd < 0)
    return -1;

  err = read(fd, &header, sizeof(header));
  if (err < (int) sizeof(header)) {
    fprintf(stderr, "Failed to read shmem topology header\n");
    close(fd);
    return -1;
  }

  if (header.header_version != LSTOPO_SHMEM_HEADER_VERSION
      || header.header_length != sizeof(header)) {
    fprintf(stderr, "Unexpected shmem topology header version %u length %u (instead of %u %u)\n",
	    header.header_version, header.header_length,
	    LSTOPO_SHMEM_HEADER_VERSION, (unsigned) sizeof(header));
    close(fd);
    return -1;
  }

  err = hwloc_shmem_topology_adopt(&adopted, fd, header.file_offset, (void*)(uintptr_t)header.mmap_address, header.mmap_length, 0);
  close(fd);
  if (err < 0) {
    if (errno == EBUSY)
      fprintf(stderr, "Failed to adopt shmem topology, memory range is busy\n"); /* this line must match the grep line in test-lstopo-shmem */
    else
      fprintf(stderr, "Failed to adopt shmem topology (%s)\n", strerror(errno));
    return -1;
  }

  err = hwloc_topology_dup(topologyp, adopted);
  hwloc_topology_destroy(adopted);
  if (err < 0) {
    fprintf(stderr, "Failed to duplicate adopted shmem topology (%s)\n", strerror(errno));
    return -1;
  }

  return 0;
}
