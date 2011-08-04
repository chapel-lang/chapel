#ifndef __COMMON_RCRTOOL_H__
#define __COMMON_RCRTOOL_H__

#include "bcGen.h"
#include <inttypes.h>

/**
 *  Resolve to "TRUE" if given thing is true, else "FALSE"
 */
#define tf(b) ((b)?"TRUE":"FALSE")

//Daemon configuration variable
extern char bDaemon;
extern char bVerbose;
extern char bOutputToNull;

typedef struct _MeterValue {
    double             value;      // meter value
    uint64_t           timestamp;  // nanosecond unit
    struct _MeterValue* next;
} MeterValue;

void     die(char* msg);
void     daemonize(char bOutputToNull);
void     doWork(int nshepherds, int nworkerspershep);

//static const char TITLE[] = "RCR Daemon (RCRdaemon) 2.00";

// Maximum number of processors
static const int MAX_PROCESSORS = 128;
// Length of buffer for storing paths into the debug file system
//static const int PATH_BUF_LENGTH = 256;
#define PATH_BUF_LENGTH 256

// Intel Nehalem supports 3 generic counters and 4 selectable counters concurrently
// AMD Opteron supports 4 selectable counters concurrently
static const int MAXNUMOFEVENTS = 7;

// Monitoring interval per metric (in nanosecond)
static const time_t   MONITORINTERVALINSEC     = 1;
static const long int MONITORINTERVALINNANOSEC = 0;

// How often the values should be updated (in nanosecond)
static const time_t   UPDATEFREQUENCYINSEC     = 0;
static const long int UPDATEFREQUENCYINNANOSEC = 0;

//// Filesystem hooks to CPU scaling system
#define DFS_ROOTDIR_RCR "/sys/kernel/debug/RCRTool"
#define DFS_ROOTDIR_RAT "/sys/kernel/debug/RCRToolRat"

// Maximum time window in RCRDaemon (in nanosecond).  Currently set to 60
// seconds.  Increasing this value may affect overall overhead in memory size
// and latency.  This value must be carefully selected.
static const uint64_t MAX_TIME_WINDOW = 60000000000;

/**
 *  Resolve to "TRUE" if given thing is true, else "FALSE"
 */
#define tf(b) ((b)?"TRUE":"FALSE")

//#define INTEL_NEHALEM
//#define AMD_OPTERON

// Core meter types
typedef enum _RCRCoreMeter {
    C_ERROR                      = -1,
    C_CPI = 0,
    C_L2MissRatio,
    C_L2MissCycleRatio,
    C_L3MissCycleRatio,

    //must be last item in the enum
    C_NUMBER_OF_CORE_METERS
} RCRCoreMeter;

#ifndef QTHREAD_RCRTOOL
static const char* RCRCoreMeters[]      = {"CPI", "L2MissRatio", "L2MissCycleRatio", "L3MissCycleRatio"};

# ifdef AMD_OPTERON
static const char* RCRSocketMeters[]    = {"L3MissRatio", "MemoryBandwidth", "MemoryConcurrency", "MemoryLatency"};
# endif

# ifdef INTEL_NEHALEM
static const char* RCRSocketMeters[]    = {"L3MissRatio", "MemoryBandwidth", "IMTOccupancy0", "IMTOccupancy1"};
# endif

static const char* RCRCoreEvents[] = {
    "PERF_COUNT_HW_CPU_CYCLES",
    "PERF_COUNT_HW_INSTRUCTIONS",
    "REQUESTS_TO_L2:ALL",
    "L2_CACHE_MISS:ALL",
    "L2_FILL_WRITEBACK:ALL"
};

static const char* RCRSocketEvents[] = {
    "PERF_COUNT_HW_INSTRUCTIONS",
    "READ_REQUEST_TO_L3_CACHE:ALL",
    "L3_CACHE_MISSES:ALL",
    "DRAM_ACCESSES_PAGE:ALL",
    "CPU_CLK_UNHALTED",
    "CPU_READ_COMMAND_REQUESTS_TO_TARGET_NODE_0_3:ALL",
    "CPU_READ_COMMAND_LATENCY_TO_TARGET_NODE_0_3:ALL",
};

#endif

/////////////////////////////////////////////////////////////////////////////////////
// Define the relevant metrics and the performance counters required to calculate them
/////////////////////////////////////////////////////////////////////////////////////
#ifdef INTEL_NEHALEM
#if 0
// Initialize the even list for Bill machine
int numOfMetersPerCore = 3;
// Currently, the uncore counter is not supported in Intel Nehalem chip
int numOfMetersPerSocket = 0;
char *metersPerCore[] = {"CPI", "L2MissCycleRatio", "L3MissCycleRatio"};
char *metersPerSocket[] = {"MemoryBandwidth"};

char *eventsPerCore[] = {"PERF_COUNT_HW_CPU_CYCLES,PERF_COUNT_HW_INSTRUCTIONS",
    "PERF_COUNT_HW_CPU_CYCLES,MEM_LOAD_RETIRED:LLC_UNSHARED_HIT,MEM_LOAD_RETIRED:OTHER_CORE_L2_HIT_HITM",
    "PERF_COUNT_HW_CPU_CYCLES,MEM_LOAD_RETIRED:LLC_MISS"};
int eventsNumPerCore[] = {2, 3, 2};

char *eventsPerSocket[] = {"UNC_LLC_LINES_IN:ANY"};
int eventsNumPerSocket[] = {1};
#endif
#elif defined(AMD_OPTERON)

# endif

// Socket meter types
typedef enum _RCRSocketMeter {
    S_ERROR       = -1,
    S_L3MissRatio = 0,
    S_MemoryBandwidth,
    S_MemoryConcurrency,
    S_MemoryLatency,

    //must be last item in the enum
    S_NUMBER_OF_SOCKET_METERS
} RCRSocketMeter;

// Core event types
typedef enum _RCRCoreEvent {
    C_PERF_COUNT_HW_INSTRUCTIONS = 0,
    C_PERF_COUNT_HW_CPU_CYCLES,
    C_REQUESTS_TO_L2_ALL,
    C_L2_CACHE_MISS_ALL,
    C_L2_FILL_WRITEBACK_ALL,
    //must be last item in the enum
    C_NUMBER_OF_CORE_EVENTS
} RCRCoreEvent;

static const int eventsPerCoreMeter[][4] = {
    {C_PERF_COUNT_HW_CPU_CYCLES, C_PERF_COUNT_HW_INSTRUCTIONS},
    {C_PERF_COUNT_HW_INSTRUCTIONS, C_REQUESTS_TO_L2_ALL, C_L2_CACHE_MISS_ALL, C_L2_FILL_WRITEBACK_ALL}};

static const int numOfEventsPerCoreMeter[] = {2, 4};

// Socket event types
typedef enum _RCRSocketEvent {
    S_PERF_COUNT_HW_INSTRUCTIONS = 0,
    S_READ_REQUEST_TO_L3_CACHE_ALL = 1,
    S_L3_CACHE_MISSES_ALL,
    S_DRAM_ACCESSES_PAGE_ALL,
    S_CPU_CLK_UNHALTED,
    S_CPU_READ_COMMAND_REQUESTS_TO_TARGET_NODE_0_3_ALL,
    S_CPU_READ_COMMAND_LATENCY_TO_TARGET_NODE_0_3_ALL,
    //must be last item in the enum
    S_NUMBER_OF_SOCKET_EVENTS
} RCRSocketEvent;

//static const char* RCRSocketMeters[]    = {"L3MissRatio", "MemoryBandwidth", "MemoryConcurrency", "MemoryLatency"};
static const int eventsPerSocketMeter[][3] = {
    {S_PERF_COUNT_HW_INSTRUCTIONS, S_READ_REQUEST_TO_L3_CACHE_ALL, S_L3_CACHE_MISSES_ALL},
    {S_PERF_COUNT_HW_INSTRUCTIONS, S_DRAM_ACCESSES_PAGE_ALL},
    {S_CPU_CLK_UNHALTED, S_L3_CACHE_MISSES_ALL, S_CPU_READ_COMMAND_REQUESTS_TO_TARGET_NODE_0_3_ALL},
    {S_CPU_CLK_UNHALTED, S_CPU_READ_COMMAND_LATENCY_TO_TARGET_NODE_0_3_ALL, S_CPU_READ_COMMAND_REQUESTS_TO_TARGET_NODE_0_3_ALL}};

static const int numOfEventsPerSocketMeter[] = {3, 2, 3, 3};

//# endif

RCRSocketMeter getSocketMeterEnumFromStr(const char* meterName);
RCRCoreMeter   getCoreMeterEnumFromStr(const char* meterName);

#endif
