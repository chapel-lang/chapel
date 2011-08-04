#ifndef QTHREAD_RCRTOOL_TRIGGERS_H
#define QTHREAD_RCRTOOL_TRIGGERS_H

#include <sys/types.h>
//#include <inttypes.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <string.h>
//#include <stdarg.h>
//#include <sys/wait.h>
//#include <err.h>
//#include <fcntl.h>
//
//#include <sys/ipc.h>
//#include <sys/shm.h>

/*! 
 * 
 */
typedef enum _rcrtool_trigger_type {
    T_TYPE_CORE = 0,
    T_TYPE_SOCKET,
    T_TYPE_NODE
} rcrtool_trigger_type;

typedef enum _rcrtool_trigger_throw_type {
    T_TYPE_LOW = 0,
    T_TYPE_HIGH
} rcrtool_trigger_throw_type;

/*!
 * Data structure holding the trigger information.  Includes info about 
 * thresholds for each kind of metric and the key for the shared mem location.
 */
typedef struct trigger {
	int    meterType;           // TYPE_CORE, TYPE_SOCKET or TYPE_NODE
	int    id;             // core/socket/node id
	char*  meterName;      // metric name, for eg. "MemoryConcurrency"
	int    meterNum;       // index into array of meter names
	key_t  flagShmKey;     // the shared memory key for the trigger flag
	key_t  appStateShmKey; // the shared memory key for the application state
	double threshold_ub;   // upper bound of metric value
	double threshold_lb;   // lower bound of metric value
} Trigger;

/***********************************************************************
* definition for the trigger map; used by all methods
***********************************************************************/
extern Trigger** triggerMap;   // allocated and pupulated in buildTriggerMap()
extern int       numTriggers;  // populated in buildTriggerMap()

void buildTriggerMap(const char *fileName);

#endif
