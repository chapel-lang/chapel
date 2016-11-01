/* 
 * read_dag.c
 */

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DAG_RECORDER 2
#include "dag_recorder_impl.h"

dr_pi_dag * 
dr_read_dag(const char * filename) {
  /* open the file */
  int fd = open(filename, O_RDONLY);
  void * a;
  dr_pi_dag * G = dr_malloc(sizeof(dr_pi_dag));
  /* buffer to read the first line */
  char header_buf[DAG_RECORDER_HEADER_LEN+1];
  if (fd == -1) {
    fprintf(stderr, "open: %s (%s)\n", 
	    strerror(errno), filename);
    return 0;
  }
  header_buf[DAG_RECORDER_HEADER_LEN] = 0;
  ssize_t r;
  /* read headers and meta info */
  if ((r = read(fd, header_buf, DAG_RECORDER_HEADER_LEN)) 
      != DAG_RECORDER_HEADER_LEN
      || (r = read(fd, &G->n, sizeof(G->n))) != sizeof(G->n)
      || (r = read(fd, &G->m, sizeof(G->m))) != sizeof(G->m)
      || (r = read(fd, &G->start_clock, sizeof(G->start_clock))) 
      != sizeof(G->start_clock)
      || (r = read(fd, &G->num_workers, sizeof(G->num_workers)))
      != sizeof(G->num_workers)) {
    const char * err = strerror(errno);
    fprintf(stderr, 
	    "read: %s (%s) offset %ld\n", 
	    err, filename, lseek(fd, 0, SEEK_CUR));
    close(fd);
    return 0;
  }
  /* check version mismatch */
  if (strcmp(header_buf, DAG_RECORDER_HEADER)) {
    fprintf(stderr, 
	    "error: format version mismatch. expected %s, read %s\n", 
	    DAG_RECORDER_HEADER, header_buf);
    close(fd);
    return 0;
  }

  off_t header_sz = lseek(fd, 0, SEEK_CUR);
  off_t file_sz = lseek(fd, 0, SEEK_END);
  (void)dr_check(header_sz 
		 == DAG_RECORDER_HEADER_LEN + sizeof(G->n) + sizeof(G->m) 
		 + sizeof(G->start_clock) + sizeof(G->num_workers));
  /* mmap the entire file */
  a = mmap(NULL, file_sz, PROT_READ | PROT_WRITE,
	   MAP_PRIVATE, fd, 0);
  if (a == MAP_FAILED) {
    fprintf(stderr, "mmap: %s (%s)\n", strerror(errno), filename);
    close(fd);
    return 0;
  }
  G->T = (dr_pi_dag_node *)(a + header_sz);
  G->E = (dr_pi_dag_edge *)&G->T[G->n];
  dr_pi_string_table * S = (dr_pi_string_table *)&G->E[G->m];

#if 0
  long st_sz = S->sz;
  off_t expected_sz = header_sz 
    + sizeof(dr_pi_dag_node) * G->n
    + sizeof(dr_pi_dag_edge) * G->m
    + st_sz;

  if (expected_sz != file_sz) {
    fprintf(stderr, "error: file %s may be corrupted; it has %ld nodes and %ld edges; we expected %ld bytes, but the file is %ld bytes\n", 
	    filename, G->n, G->m, expected_sz, file_sz);
    close(fd);
    return 0;
  }
#endif

  /* index table after the header of S */
  S->I = (long *)&S[1];
  /* char array after the index table */
  S->C = (const char *)&S->I[S->n];
  G->S = S;
#if 0
  long n = S->n;
  long i;
  printf("%ld strings\n", n);
  for (i = 0; i < n; i++) {
    printf("string %ld : %s\n", i, S->C + S->I[i]);
  }
#endif

  close(fd);
  return G;
}

/* record_dag.c */
int dr_gen_dot(dr_pi_dag * G);
int dr_gen_gpl(dr_pi_dag * G);
void dr_opts_init(dr_options * opts);

int dr_read_and_analyze_dag_(const char * filename) {
  dr_pi_dag * G = dr_read_dag(filename);
  if (G) {
    dr_opts_init(0);
    if (dr_gen_basic_stat(G) == 0) return 0;
    if (dr_gen_dot(G) == 0) return 0;
    if (dr_gen_gpl(G) == 0) return 0;
    return 1;
  } else {
    return 0;
  }
}
