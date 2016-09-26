/* 
 * options.c
 */

#define DAG_RECORDER 2
#include "dag_recorder_impl.h"

static char 
getenv_bool(const char * v, char * y) {
  char * x = getenv(v);
  if (!x) return 0;
  if (strcasecmp(x, "0") == 0
      || strcasecmp(x, "n") == 0
      || strcasecmp(x, "false") == 0) {
    *y = 0;
  } else {
    *y = 1;
  }
  return 1;
}


/* read environment variable v and parse it as a
   small integer */
static int 
getenv_byte(const char * v, char * y) {
  char * x = getenv(v);
  if (!x) return 0;
  *y = (char)atoi(x);
  return 1;
} 

/* read environment variable v and parse it as an
   integer */
static int 
getenv_int(const char * v, int * y) {
  char * x = getenv(v);
  if (!x) return 0;
  *y = atoi(x);
  return 1;
} 

/* read environment variable v and parse it as a
   long integer */
static int 
getenv_long(const char * v, long * y) {
  char * x = getenv(v);
  if (!x) return 0;
  *y = atol(x);
  return 1;
} 

/* read environment variable v and parse it as an
   unsigned long long integer */
static int 
getenv_ull(const char * v, unsigned long long * y) {
  char * x = getenv(v);
  if (!x) return 0;
  long long z = atoll(x);
  *y = (unsigned long long)z;
  return 1;
} 

/* read environment variable v and parse it as 
   a string */
static int 
getenv_str(const char * v, const char ** y) {
  char * x = getenv(v);
  if (!x) return 0;
  *y = strdup(x);
  return 1;
} 

void
dr_opts_init(dr_options * opts) {
  dr_options opts_[1];
  if (!opts) {
    opts = opts_;
    dr_options_default(opts);
  }
  GS.opts = *opts;
}

/* fill opts with default values for all options */
void dr_options_default_(dr_options * opts) {
  * opts = dr_options_default_values;
  if (getenv_bool("DAG_RECORDER",             &opts->on)
      || getenv_bool("DR",                    &opts->on)) {}
  if (getenv_str("DAG_RECORDER_FILE_PREFIX",  &opts->dag_file_prefix)
      || getenv_str("DR_PREFIX",              &opts->dag_file_prefix)) {}
  if (getenv_bool("DAG_RECORDER_DAG_FILE",    &opts->dag_file_yes)
      || getenv_bool("DR_DAG",                &opts->dag_file_yes)) {}
  if (getenv_bool("DAG_RECORDER_STAT_FILE",   &opts->stat_file_yes)
      || getenv_bool("DR_STAT",               &opts->stat_file_yes)) {}
  if (getenv_bool("DAG_RECORDER_GPL_FILE",    &opts->gpl_file_yes)
      || getenv_bool("DR_GPL",                &opts->gpl_file_yes)) {}
  if (getenv_bool("DAG_RECORDER_DOT_FILE",    &opts->dot_file_yes)
      || getenv_bool("DR_DOT",                &opts->dot_file_yes)) {}
  if (getenv_bool("DAG_RECORDER_TEXT_FILE",   &opts->text_file_yes)
      || getenv_bool("DR_TEXT",               &opts->text_file_yes)) {}
  /* NOTE: we do not set sqlite_file via environment variables */
  if (getenv_int("DAG_RECORDER_GPL_SIZE",     &opts->gpl_sz)
      || getenv_int("DR_GPL_SZ",              &opts->gpl_sz)) {}
  if (getenv_str("DAG_RECORDER_TEXT_FILE_SEP",    &opts->text_file_sep)
      || getenv_str("DR_TEXT_SEP",                &opts->text_file_sep)) {}
  if (getenv_byte("DAG_RECORDER_DBG_LEVEL",   &opts->dbg_level)
      || getenv_byte("DR_DBG",                &opts->dbg_level)) {}
  if (getenv_byte("DAG_RECORDER_VERBOSE_LEVEL",  &opts->verbose_level)
      || getenv_byte("DR_VERBOSE",               &opts->verbose_level)) {}
  if (getenv_byte("DAG_RECORDER_CHK_LEVEL",   &opts->chk_level)
      || getenv_byte("DR_CHK",                &opts->chk_level)) {}
  if (getenv_ull("DAG_RECORDER_UNCOLLAPSE_MIN", &opts->uncollapse_min)
      || getenv_ull("DR_UNCOLLAPSE_MIN",        &opts->uncollapse_min)) {}
  if (getenv_ull("DAG_RECORDER_COLLAPSE_MAX", &opts->collapse_max)
      || getenv_ull("DR_COLLAPSE_MAX",        &opts->collapse_max)) {}
  if (getenv_long("DAG_RECORDER_NODE_COUNT",  &opts->node_count_target)
      || getenv_long("DR_NC",                 &opts->node_count_target)) {}
  if (getenv_long("DAG_RECORDER_PRUNE_THRESHOLD",  &opts->prune_threshold)
      || getenv_long("DR_PRUNE",              &opts->prune_threshold)) {}

  if (getenv_long("DAG_RECORDER_COLLAPSE_MAX_COUNT",  &opts->collapse_max_count)
      || getenv_long("DR_COLLAPSE_MAX_COUNT", &opts->collapse_max_count)) {}


  if (getenv_long("DAG_RECORDER_ALLOC_UNIT_MB", &opts->alloc_unit_mb)
      || getenv_long("DR_ALLOC_UNIT_MB",      &opts->alloc_unit_mb)) {}
  if (getenv_long("DAG_RECORDER_PRE_ALLOC_PER_WORKER",   
		  &opts->pre_alloc_per_worker)
      || getenv_long("DR_PRE_ALLOC_PER_WORKER", 
		     &opts->pre_alloc_per_worker)) {}
  if (getenv_long("DAG_RECORDER_PRE_ALLOC",   &opts->pre_alloc)
      || getenv_long("DR_PRE_ALLOC",          &opts->pre_alloc)) {}
}

/* print set optoins */
void 
dr_opts_print(dr_options * opts) {
  if (!opts) opts = &GS.opts;
  if (opts->verbose_level >= 1) {
    FILE * wp = stderr;
    fprintf(wp, "DAG Recorder Options:\n");
    fprintf(wp, "dag_file_prefix (DAG_RECORDER_DAG_FILE_PREFIX,DR_PREFIX) : %s\n", 
	    opts->dag_file_prefix);
    fprintf(wp, "dag_file_yes (DAG_RECORDER_DAG_FILE,DR_DAG) : %d\n", 
	    opts->dag_file_yes);
    fprintf(wp, "stat_file_yes (DAG_RECORDER_STAT_FILE,DR_STAT) : %d\n", 
	    opts->stat_file_yes);
    fprintf(wp, "gpl_file_yes (DAG_RECORDER_GPL_FILE,DR_GPL) : %d\n", 
	    opts->gpl_file_yes);
    fprintf(wp, "dot_file_yes (DAG_RECORDER_DOT_FILE,DR_DOT) : %d\n", 
	    opts->dot_file_yes);
    fprintf(wp, "text_file_yes (DAG_RECORDER_TEXT_FILE,DR_TEXT) : %d\n", 
	    opts->text_file_yes);
    fprintf(wp, "gpl_sz (DAG_RECORDER_GPL_SIZE,DR_GPL_SZ) : %d\n", 
	    opts->gpl_sz);
    fprintf(wp, "text_file_sep (DAG_RECORDER_TEXT_FILE_SEP,DR_TEXT_SEP) : %s\n", 
	    opts->text_file_sep);
    fprintf(wp, "dbg_level (DAG_RECORDER_DBG_LEVEL,DR_DBG) : %d\n", 
	    opts->dbg_level);
    fprintf(wp, "verbose_level (DAG_RECORDER_VERBOSE_LEVEL,DR_VERBOSE) : %d\n", 
	    opts->verbose_level);
    fprintf(wp, "chk_level (DAG_RECORDER_CHK_LEVEL,DR_CHK) : %d\n", 
	    opts->chk_level);
    fprintf(wp, "uncollapse_min (DAG_RECORDER_UNCOLLAPSE_MIN,DR_UNCOLLAPSE_MIN) : %llu\n", 
	    opts->uncollapse_min);
    fprintf(wp, "collapse_max (DAG_RECORDER_COLLAPSE_MAX,DR_COLLAPSE_MAX) : %llu\n", 
	    opts->collapse_max);
    fprintf(wp, "node_count_target (DAG_RECORDER_NODE_COUNT,DR_NC) : %ld\n", 
	    opts->node_count_target);
    fprintf(wp, "prune_threshold (DAG_RECORDER_PRUNE_THRESHOLD,DR_PRUNE) : %ld\n", 
	    opts->prune_threshold);
    fprintf(wp, "alloc_unit_mb (DAG_RECORDER_ALLOC_UNIT_MB,DR_ALLOC_UNIT_MB) : %ld\n", 
	    opts->alloc_unit_mb);
    fprintf(wp, "pre_alloc_per_worker (DAG_RECORDER_PRE_ALLOC_PER_WORKER,DR_PRE_ALLOC_PER_WORKER) : %ld\n", 
	    opts->pre_alloc_per_worker);
    fprintf(wp, "pre_alloc (DAG_RECORDER_PRE_ALLOC,DR_PRE_ALLOC) : %ld\n", 
	    opts->pre_alloc);
  }
}
