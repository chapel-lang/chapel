#include <stdio.h>

// This seems not to come from <string.h> without effort; just declare it.
extern char* strdup(const char*);


static struct tcb_data {
  const char* prefix;
  chpl_task_cb_kind_t kind;
  chpl_task_cb_info_t info;
} tcb_data[TCB_DATA_N];

static volatile int tcb_data_idx = 0;

static pthread_mutex_t tcb_data_mux = PTHREAD_MUTEX_INITIALIZER;


void tcb_record(const char* prefix,
                chpl_task_cb_kind_t kind, const chpl_task_cb_info_t* info) {
  if (pthread_mutex_lock(&tcb_data_mux) != 0) {
    perror("pthread_mutex_lock()");
    exit(1);
  }

  //
  // Protected region
  //
  {
    struct tcb_data* p;

    if (tcb_data_idx < TCB_DATA_N)
      p = &tcb_data[tcb_data_idx++];
    else {
      fprintf(stderr, "tcb_data_idx overflow at %d!\n", tcb_data_idx);
      exit(1);
    }

    p->prefix = prefix;
    p->kind = kind;

    switch (kind) {
    case chpl_task_cb_kind_task_begin:
      p->info.begin_info.version = info->begin_info.version;
      p->info.begin_info.filename = strdup(info->begin_info.filename);
      p->info.begin_info.lineno = info->begin_info.lineno;
      p->info.begin_info.id = info->begin_info.id;
      p->info.begin_info.is_executeOn = info->begin_info.is_executeOn;
      break;

    case chpl_task_cb_kind_task_end:
      p->info.end_info.version = info->end_info.version;
      p->info.end_info.id = info->end_info.id;
      break;

    default:
      fprintf(stderr, "tcb_record(): unexpected kind %d!\n", (int) kind);
      exit(1);
      break;
    }
  }

  if (pthread_mutex_unlock(&tcb_data_mux) != 0) {
    perror("pthread_muutex_lock()");
    exit(1);
  }
}


void tcb_wait_for_nCallbacks(int nCallbacks) {
  int nWaits;

  for (nWaits = 0; nWaits < 3 && tcb_data_idx < nCallbacks; nWaits++)
    sleep(1);
}


void tcb_report(void) {
  if (pthread_mutex_lock(&tcb_data_mux) != 0) {
    perror("pthread_mutex_lock()");
    exit(1);
  }

  //
  // Protected region
  //
  {
    int i;

    for (i = 0; i < tcb_data_idx; i++) {
      struct tcb_data* p = &tcb_data[i];

      switch (p->kind) {
      case chpl_task_cb_kind_task_begin:
        printf("%s task %ld @%s:%d, %s executeOn\n",
               p->prefix,
               (long int) p->info.begin_info.id,
               p->info.begin_info.filename,
               p->info.begin_info.lineno,
               p->info.begin_info.is_executeOn ? "is" : "not");
        break;


      case chpl_task_cb_kind_task_end:
        printf("%s task %ld end\n",
               p->prefix,
               (long int) p->info.end_info.id);
        break;

      default:
        fprintf(stderr, "tcb_report(): unexpected kind %d!\n", (int) p->kind);
        exit(1);
        break;
      }
    }
  }

  if (pthread_mutex_unlock(&tcb_data_mux) != 0) {
    perror("pthread_muutex_lock()");
    exit(1);
  }
}
