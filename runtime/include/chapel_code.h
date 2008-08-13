#ifndef _chapel_code_H_
#define _chapel_code_H_
void chpl_init_chapel_code(void);
void chpldev_taskTable_add(uint64_t threadID, uint32_t lineno, _string chpl__filename_57106);
void chpldev_taskTable_print(void);
void chpldev_taskTable_remove(uint64_t threadID);
#endif
