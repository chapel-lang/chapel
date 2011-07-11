/**
 * Breadcrumbs generation for RCR Daemon. 
 * By Anirban Mandal
 *
 */

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

#include "triggers.h"
#include "blackboard.h"

void buildTriggerMap(const char *fileName);
int  putBreadcrumbs(rcrtool_trigger_type type, int id, const char *meterName, double currentval);
char getBreadcrumbs(rcrtool_trigger_type type, int id, char *meterName);
void dumpAppState(key_t key, rcrtool_trigger_type triggerType, int socketOrCoreID);
void clearAppState(void);
void shmPutFlag(key_t key, char val);
//void shmPutString(key_t key, const char* strData, size_t size);
void releaseShmLoc(const void* shm);
char* getShmStringLoc(key_t key, size_t size);
char shmGet(key_t key);
void printTriggerMap(void);
//char *trim(char *const s);
//char *rtrim(char *const s);
//char *ltrim(char *const s);
int getNumTriggers(void);
Trigger** getTriggers(void);
struct _RCRBlackboard* getShmBlackboardRW(void);
struct _RCRBlackboard* getShmBlackboardRO(void);
void throwTrigger(key_t appStateShmKey, rcrtool_trigger_type triggerType, rcrtool_trigger_throw_type throwKind, int socketOrCoreID, int triggerNum);
