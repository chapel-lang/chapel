#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

void *rcrtoolDaemon(void *arg);
//void *rcrtoolDaemon(void);

extern pthread_t rcrToolPThreadID;

extern int rcrToolContinue;

typedef enum _qt_rcrtool_level {
    RCR_NONE = 0,
    //RCR_METERS,
    RCR_TRIGGERS,
    RCR_THROTTLE
} qt_rcrtool_level;

extern qt_rcrtool_level rcrtoollevel;

typedef enum _qt_rcrtool_log_level {
    //RCR_NONE = 0,
    RCR_XOMP_SECTIONS = 1,
    RCR_XOMP_LOOPS,
    RCR_APP_STATE_DUMP,
    RCR_APP_STATE_DUMP_ALWAYS
} qt_rcrtool_log_level;

extern qt_rcrtool_log_level rcrtoolloglevel;

typedef enum _XOMP_Type {
    XOMP_UNK = 0,
    XOMP_TASK_START,
    XOMP_TASKWAIT,
    XOMP_TASK_END,
    XOMP_PARALLEL_START,
    XOMP_PARALLEL_END,
    XOMP_FOR_LOOP_START,
    XOMP_FOR_LOOP_END,
    XOMP_BARRIER,
    XOMP_STREAM_START,
    XOMP_STREAM_END,
    XOMP_INIT,
    XOMP_TERMINATE,
    XOMP_CRITICAL,
    XOMP_SINGLE,
    XOMP_RAT_DEBUG
} XOMP_Type;

void rcrtool_log(qt_rcrtool_log_level level, XOMP_Type type, unsigned int thread_id, uint64_t data, const char* data2);

#define RCR_HASH_TABLE_SIZE     256
#define RCR_STACK_SIZE          RCR_HASH_TABLE_SIZE
#define RCR_HASH_ENTRY_SIZE     64
#define RCR_HASH_LOOKUP_FAILURE -1
#define RCR_APP_NAME_MAX_SIZE   1024

typedef struct _ShepWorkerInfo {
    int   nshepherds;
    int   nworkerspershep;
    void* worker;
    int   qaffinity;
} ShepWorkerInfo;

extern ShepWorkerInfo swinfo;

typedef struct _RCRHashEntry {
    char funcName[RCR_HASH_ENTRY_SIZE];
    int loopCount;
    int loopQueue[64];
    int loopQueueDepth;
    unsigned int count;
    unsigned int numOMPassignedThreads;
} RCRHashEntry;

extern RCRHashEntry hashTable[RCR_HASH_TABLE_SIZE];
extern int RCRParallelSectionStack[RCR_STACK_SIZE];
extern int RCRParallelSectionStackPos;
extern char RCRAppName[RCR_APP_NAME_MAX_SIZE];



