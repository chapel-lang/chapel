#include <stdio.h>

#include "chpl-tasks.h"

#include "tcb-c.h"

#define TCB_DATA_N 16 // only need 8 when #locales==1, but 16 otherwise
#include "tcb-util.h"


/*====================
 * Private stuff
 */

static void cb_begin_1(chpl_task_cb_kind_t, const chpl_task_cb_info_t*);
static void cb_end_1(chpl_task_cb_kind_t, const chpl_task_cb_info_t*);
static void cb_any_2(chpl_task_cb_kind_t, const chpl_task_cb_info_t*);


static void cb_begin_1(chpl_task_cb_kind_t kind,
                       const chpl_task_cb_info_t* info) {
  tcb_record("CB1", kind, info);
}


static void cb_end_1(chpl_task_cb_kind_t kind,
                     const chpl_task_cb_info_t* info) {
  tcb_record("CB1", kind, info);
}


static void cb_any_2(chpl_task_cb_kind_t kind,
                     const chpl_task_cb_info_t* info) {
  tcb_record("CB2", kind, info);
}


/*====================
 * Public interface
 */

void install_callbacks(void) {
  if (chpl_task_install_callback(chpl_task_cb_kind_task_begin, cb_begin_1)
      != 0) {
    fprintf(stderr, "Cannot install cb_begin_1!\n");
    exit(1);
  }

  if (chpl_task_install_callback(chpl_task_cb_kind_task_end, cb_end_1)
      != 0) {
    fprintf(stderr, "Cannot install cb_end_1!\n");
    exit(1);
  }

  if (chpl_task_install_callback(chpl_task_cb_kind_task_begin, cb_any_2)
      != 0) {
    fprintf(stderr, "Cannot install cb_any_2!\n");
    exit(1);
  }

  if (chpl_task_install_callback(chpl_task_cb_kind_task_end, cb_any_2)
      != 0) {
    fprintf(stderr, "Cannot install cb_any_2 again!\n");
    exit(1);
  }
}


void report_callbacks(int nCallbacks) {
  tcb_wait_for_nCallbacks(nCallbacks);
  tcb_report();
}
