#ifndef _CHPL_SYS_H_
#define _CHPL_SYS_H_

#include <stdint.h>

uint64_t _bytesPerLocale(void);
int32_t _coresPerLocale(void);
int32_t _maxThreads(void);

#endif
