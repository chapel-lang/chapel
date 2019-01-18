/* 
 * myth_misc.c
 */
#include <ctype.h>

#include "myth_misc.h"
#include "myth_misc_func.h"

#if MYTH_FLMALLOC_TLS
__thread myth_freelist_t *g_myth_freelist;
#else
myth_freelist_t **g_myth_freelist;
#endif

//Global variabled declaration

//uint64_t g_mmap_total=0,g_mmap_count=0;

__thread unsigned int g_myth_random_temp = 0;

__thread uint64_t g_myth_flmalloc_cycles = 0, g_myth_flmalloc_cnt = 0;
__thread uint64_t g_myth_flfree_cycles = 0, g_myth_flfree_cnt = 0;

static int myth_cpu_list[CPU_SETSIZE];
static cpu_set_t worker_cpusets[CPU_SETSIZE];
static int available_cores = -1;

/* 
   When environment variable MYTH_CPU_LIST is set,
   it enforces a worker to cpu assignment.
   for example, if MYTH_CPU_LIST=3,5,7,9 
   then 
   worker 0 is bound to 3, 
   worker 1 is bound to 5, 
   worker 2 is bound to 7, 
   worker 3 is bound to 9,
   worker 4 is bound to 3, 
   worker 5 is bound to 5, 
   worker 6 is bound to 7, 
   worker 7 is bound to 9,
   ...
*/

/* character stream */
typedef struct char_stream {
  char * a;			/* underlying string */
  int i;			/* the current position */
  int ok_pos;			/* position successfully parsed so far */
} char_stream, * char_stream_t;

typedef struct int_list {
  int * a;
  int i;
  int n;
} int_list, * int_list_t;
 
/* make a character stream returning characters in A */
static char_stream_t init_char_stream(char_stream_t cs, char * a) {
  cs->a = a;
  cs->i = 0;
  cs->ok_pos = 0;
  return cs;
}

static int_list_t init_int_list(int_list_t il, int * a, int n) {
  il->a = a;
  il->i = 0;
  il->n = n;
  return il;
}

static int int_list_add(int_list_t il, int x) {
  int i = il->i;
  int n = il->n;
  if (i < n) {
    il->a[i] = x;
    il->i = i + 1;
    return 1;
  } else {
    return 0;
  }
}

static inline int cur_char(char_stream_t cs) {
  return cs->a[cs->i];
}

static inline int next_char(char_stream_t cs) {
  assert(cs->a[cs->i] != '\n');
  cs->i++;
  return cur_char(cs);
}

/* mark the current position as having been parsed successfully */
static inline void set_ok_pos(char_stream_t cs) {
  cs->ok_pos = cs->i;
}

static void parse_error(char_stream_t cs, char * msg) {
  int i;
  fprintf(stderr, "myth_parse_cpu_list: invalid resource list: %s\n", 
	  msg);
  fprintf(stderr, "  %s\n", cs->a);
  for (i = 0; i < 2 + cs->ok_pos; i++) fputc(' ', stderr);
  for (     ; i < 2 + cs->i; i++) fputc('^', stderr);
  fputc('\n', stderr);
}

/* get a non-negative number or return -1 */
static int parse_int(char_stream_t cs) {
  int x = 0;
  int n_digits = 0;
  while (isdigit(cur_char(cs))) {
    n_digits++;
    x = x * 10 + (cur_char(cs) - '0');
    next_char(cs);
  }
  if (n_digits == 0) { 
    parse_error(cs, "expected a digit"); 
    return -1; 
  }
  return x;
}

/* a, a-b, or a-b:c */
static int parse_range(char_stream_t cs, int_list_t il) {
  int a = parse_int(cs);
  if (a == -1) return 0;	/* NG */
  int b = a + 1;
  int c = 1;			/* stride */
  int x;
  if (cur_char(cs) == '-') {
    next_char(cs);
    b = parse_int(cs);
    if (b == -1) return 0;
    if (cur_char(cs) == ':') {
      next_char(cs);
      c = parse_int(cs);
      if (c == -1) return 0; /* NG */
    }
  } 
  for (x = a; x < b; x += c) {
    if (!int_list_add(il, x)) {
      parse_error(cs, 
		  "myth_parse_cpu_list: too many numbers in MYTH_CPU_LIST\n");
      return 0;
    }
  }
  return 1;			/* OK */
}

static int parse_range_list(char_stream_t cs, int_list_t il) {
  if (!parse_range(cs, il)) return 0;
  set_ok_pos(cs);
  while (cur_char(cs) == ',') {
    next_char(cs);
    if (!parse_range(cs, il)) return 0;
    set_ok_pos(cs);
  }
  if (cur_char(cs) != '\0') { 
    next_char(cs);
    parse_error(cs, "junk at the end of CPU list"); 
    return 0;
  }
  return 1;			/* OK */
}

/* entry: 
   parse_cpu_list("MYTH_CPU_LIST", a, n) 
   fills integer array a with CPU numbers
   specified in MYTH_CPU_LIST. n is the number
   of elements a can store.  if the number of
   CPUs listed in MYTH_CPU_LIST, then it returns
   -1.
*/
static int myth_parse_cpu_list(const char * var, int * a, int n) {
  char_stream cs[1];
  int_list il[1];
  char * s = getenv(var);
  init_int_list(il, a, n);
  if (s) {
    init_char_stream(cs, s);
    if (!parse_range_list(cs, il)) return -1;
  }
#if 0
  int i;
  for (i = 0; i < il->i; i++) {
    if (i > 0) printf(",");
    printf("%d", il->a[i]);
  }
  printf("\n");
#endif
  return il->i;
}


void myth_init_read_available_cpu_list(void) {
  int n = myth_parse_cpu_list("MYTH_CPU_LIST", myth_cpu_list, CPU_SETSIZE);
  if (n == -1) {
    fprintf(stderr, "MYTH_CPU_LIST ignored\n");
  }
  cpu_set_t cset;
  sched_getaffinity(getpid(), sizeof(cpu_set_t), &cset);
  int i;
  for (i = 0; i < CPU_SETSIZE; i++){
    CPU_ZERO(&worker_cpusets[i]);
  }
  available_cores = 0;
  int sz = (n ? n : CPU_SETSIZE);
  for (i = 0; i < sz; i++){
    if (CPU_ISSET((n > 0 ? myth_cpu_list[i] : i), &cset)) {
      CPU_SET((n > 0 ? myth_cpu_list[i] : i), &worker_cpusets[available_cores]);
      available_cores++;
    }
  }
#if 0
  printf("%d available cores:", available_cores);
  for (i = 0; i < available_cores; i++) {
    int x;
    int nx = 0;
    printf(" ");
    printf("{");
    for (x = 0; x < CPU_SETSIZE; x++) {
      if (CPU_ISSET(x, &worker_cpusets[i])) {
	if (nx > 0) printf(",");
	printf("%d", x);
      }
    }
    printf("}");
  }
  printf("\n");
#endif
}

//Return the number of CPU cores
int myth_get_cpu_num(void) {
  if (available_cores <= 0) {
    myth_init_read_available_cpu_list();
    assert(available_cores > 0);
  }
  return available_cores;
}

//Return cpu_set
cpu_set_t myth_get_worker_cpuset(int rank) {
  if (available_cores <= 0) {
    myth_init_read_available_cpu_list();
    assert(available_cores > 0);
  }
  return worker_cpusets[rank % available_cores];
}
