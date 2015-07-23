//////////////////////
//
// Interface
//

void tcb_install_callbacks_1(void);
void tcb_install_callbacks_2(void);
void tcb_uninstall_callbacks_1(void);
void tcb_wait_for_nCallbacks(int nCallbacks);
void tcb_report(void);


//////////////////////
//
// Implementation
//

#ifndef _tcb_util_h_
#define _tcb_util_h_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "chpl-tasks-callbacks.h"


// This seems not to come from <string.h> without effort; just declare it.
extern char* strdup(const char*);


static struct tcb_data {
  const char* prefix;
  chpl_task_cb_info_t info;
} tcb_data[TCB_DATA_N];

static int tcb_data_idx = 0;

static pthread_mutex_t tcb_data_mux = PTHREAD_MUTEX_INITIALIZER;


////////////
//
// Private stuff
//

static void tcb_record(const char* prefix,
                       const chpl_task_cb_info_t* info) {
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
    p->info = *info;

    switch (info->info_kind) {
    case chpl_task_cb_info_kind_full:
      p->info.iu.full.filename = strdup(info->iu.full.filename);
      break;

    case chpl_task_cb_info_kind_id_only:
      break;

    default:
      fprintf(stderr, "tcb_record(): unexpected info_kind %d!\n",
              (int) info->info_kind);
      exit(1);
      break;
    }
  }

  if (pthread_mutex_unlock(&tcb_data_mux) != 0) {
    perror("pthread_muutex_lock()");
    exit(1);
  }
}


static void cb_create_1(const chpl_task_cb_info_t* info) {
  tcb_record("CB1", info);
}


static void cb_begin_1(const chpl_task_cb_info_t* info) {
  tcb_record("CB1", info);
}


static void cb_end_1(const chpl_task_cb_info_t* info) {
  tcb_record("CB1", info);
}


static void cb_any_2(const chpl_task_cb_info_t* info) {
  tcb_record("CB2", info);
}


////////////
//
// Public stuff
//

void tcb_install_callbacks_1(void) {
  if (chpl_task_install_callback(chpl_task_cb_event_kind_create,
                                 chpl_task_cb_info_kind_full,
                                 cb_create_1)
      != 0) {
    fprintf(stderr, "Cannot install cb_create_1!\n");
    exit(1);
  }

  if (chpl_task_install_callback(chpl_task_cb_event_kind_begin,
                                 chpl_task_cb_info_kind_full,
                                 cb_begin_1)
      != 0) {
    fprintf(stderr, "Cannot install cb_begin_1!\n");
    exit(1);
  }

  if (chpl_task_install_callback(chpl_task_cb_event_kind_end,
                                 chpl_task_cb_info_kind_id_only,
                                 cb_end_1)
      != 0) {
    fprintf(stderr, "Cannot install cb_end_1!\n");
    exit(1);
  }
}


void tcb_install_callbacks_2(void) {
  if (chpl_task_install_callback(chpl_task_cb_event_kind_create,
                                 chpl_task_cb_info_kind_full,
                                 cb_any_2)
      != 0) {
    fprintf(stderr, "Cannot install cb_any_2 for create!\n");
    exit(1);
  }

  if (chpl_task_install_callback(chpl_task_cb_event_kind_begin,
                                 chpl_task_cb_info_kind_full,
                                 cb_any_2)
      != 0) {
    fprintf(stderr, "Cannot install cb_any_2 for begin!\n");
    exit(1);
  }

  if (chpl_task_install_callback(chpl_task_cb_event_kind_end,
                                 chpl_task_cb_info_kind_id_only,
                                 cb_any_2)
      != 0) {
    fprintf(stderr, "Cannot install cb_any_2 for end!\n");
    exit(1);
  }
}


void tcb_uninstall_callbacks_1(void) {
  //
  // Uninstall the first callback, thus forcing the tasking layer to
  // compact the list.
  //
  if (chpl_task_uninstall_callback(chpl_task_cb_event_kind_create, cb_create_1)
      != 0) {
    fprintf(stderr, "Cannot uninstall cb_create_1!\n");
    exit(1);
  }

  if (chpl_task_uninstall_callback(chpl_task_cb_event_kind_begin, cb_begin_1)
      != 0) {
    fprintf(stderr, "Cannot uninstall cb_begin_1!\n");
    exit(1);
  }

  if (chpl_task_uninstall_callback(chpl_task_cb_event_kind_end, cb_end_1)
      != 0) {
    fprintf(stderr, "Cannot uninstall cb_end_1!\n");
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
    static const char* event_names[chpl_task_cb_num_event_kinds] =
      { "create",
        "begin",
        "end" };

    int i;

    for (i = 0; i < tcb_data_idx; i++) {
      struct tcb_data* p = &tcb_data[i];
      chpl_task_cb_info_t* pi = &p->info;

      switch (pi->info_kind) {
      case chpl_task_cb_info_kind_full:
        printf("%s (%ld) %s task %ld @%s:%d, %s executeOn\n",
               p->prefix,
               (long int) pi->nodeID,
               event_names[pi->event_kind],
               (long int) pi->iu.full.id,
               pi->iu.full.filename,
               pi->iu.full.lineno,
               pi->iu.full.is_executeOn ? "is" : "not");
        break;

      case chpl_task_cb_info_kind_id_only:
        printf("%s (%ld) %s task %ld\n",
               p->prefix,
               (long int) pi->nodeID,
               event_names[pi->event_kind],
               (long int) pi->iu.id_only.id);
        break;

      default:
        fprintf(stderr, "tcb_report(): unexpected info_kind %d!\n",
                (int) pi->info_kind);
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

#endif // _tcb_util_h_
