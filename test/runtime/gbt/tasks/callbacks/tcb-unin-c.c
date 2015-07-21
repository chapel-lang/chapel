#include <stdio.h>
#include <stdlib.h>

#include "chpl-tasks-callbacks.h"

#include "tcb-unin-c.h"

#define TCB_DATA_N 36 // only need 18 when #locales==1, but 36 otherwise
#include "tcb-util.h"


/*====================
 * Private stuff
 */

static void cb_create_1(const chpl_task_cb_info_t*);
static void cb_begin_1(const chpl_task_cb_info_t*);
static void cb_end_1(const chpl_task_cb_info_t*);
static void cb_any_2(const chpl_task_cb_info_t*);


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


/*====================
 * Public interface
 */

void install_callbacks(void) {
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


void uninstall_one_callback(int nCallbacks) {
  tcb_wait_for_nCallbacks(nCallbacks);

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


void report_callbacks(int nCallbacks) {
  tcb_wait_for_nCallbacks(nCallbacks);
  tcb_report();
}
