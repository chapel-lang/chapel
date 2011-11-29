#ifndef _chpl_rt_utils_static_H_
#define _chpl_rt_utils_static_H_
#include "stdchplrt.h"
void chpldev_taskTable_add(chpl_taskID_t taskID, uint32_t lineno, chpl_string filename2, uint64_t tl_info);
void chpldev_taskTable_remove(chpl_taskID_t taskID);
void chpldev_taskTable_set_active(chpl_taskID_t taskID);
void chpldev_taskTable_set_suspended(chpl_taskID_t taskID);
uint64_t chpldev_taskTable_get_tl_info(chpl_taskID_t taskID);
void chpldev_taskTable_print(void);
#endif
