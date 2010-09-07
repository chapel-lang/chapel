/* detect-sysv: Copyright 2008, Jason Duell
  This is a program that will tell you the maximum
  supported Posix shared memory segment available on a machine.  (or, of
  course, if it doesn't compile/run, that they aren't supported).
  On Linux, link with -lrt
*/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdint.h>


#define SHM_NAME "/posixshmemelem"

#define MEGABYTE (1024*1024LU)

int main(int argc, char **argv)
{
    int fd;
    void *addr;
    size_t sz, gotsz = 0;

    /* find biggest size we can open */
    for (sz = MEGABYTE; sz *= 2; ) {
	fd = shm_open(SHM_NAME, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1) {
	    perror("error in shm_open");
	    exit (-1);
	}
	shm_unlink(SHM_NAME);
	if (ftruncate(fd, sz) == -1) {
	    perror("error in ftruncate");
	    printf("Biggest size was %lu (%lu MB)\n", (unsigned long)gotsz , (unsigned long)gotsz/MEGABYTE);
	    exit (-1);
	}
	addr = mmap(NULL, sz, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (!addr) {
	    perror("error in mmap");
	    printf("Biggest size was %lu (%lu MB)\n", (unsigned long)gotsz , (unsigned long)gotsz/MEGABYTE);
	    exit (-1);
	}
	printf("got region of %lu bytes (%lu MB)\n", (unsigned long)sz , (unsigned long)sz/MEGABYTE);
	close(fd);
	munmap(addr, sz);
	gotsz = sz;
    }

    return 0;
}


