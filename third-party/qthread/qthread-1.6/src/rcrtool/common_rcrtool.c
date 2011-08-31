#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <time.h>
#include <sys/time.h>
#include "perf_util.h"
#include "common_rcrtool.h"
#include "qt_rcrtool.h"
#include "bcGen.h"

int rcrToolContinue = 1;
typedef struct _parallelRegion{
    void (*func) (void *);
    const char* funcName;
    unsigned int entryNum;
} parallelRegion;

#define LATENCYTEST 10000

#define TITLE "RCR Daemon (RCRdaemon) 1.00"

// Maximum number of processors
#define MAX_PROCESSORS  128
// Length of buffer for storing paths into the debug file system
#define PATH_BUF_LENGTH 256

// Intel Nehalem supports 3 generic counters and 4 selectable counters concurrently
// AMD Opteron supports 4 selectable counters concurrently
#define MAXNUMOFEVENTS 7

// Monitoring interval per metric (in nanosecond)
#define MONITORINTERVALINSEC 1
#define MONITORINTERVALINNANOSEC 0

// How often the values should be updated (in nanosecond)
#define UPDATEFREQUENCYINSEC 0
#define UPDATEFREQUENCYINNANOSEC 0

// Filesystem hooks to CPU scaling system
#define DFS_ROOTDIR_RCR "/sys/kernel/debug/RCRTool"
#define DFS_ROOTDIR_RAT "/sys/kernel/debug/RCRToolRat"

// Maximum time window in RCRDaemon (in nanosecond).  Currently set to 60
// seconds.  Increasing this value may affect overall overhead in memory size
// and latency.  This value must be carefully selected.
#define MAX_TIME_WINDOW 60000000000

#define AMD_OPTERON

/*!
 * Define the relevant metrics and the performance counters required to
 * calculate them
 */
#ifdef INTEL_NEHALEM
// Initialize the even list for Bill machine
static const int   numOfMetersPerCore   = 3;
// Currently, the uncore counter is not supported in Intel Nehalem chip
static const int   numOfMetersPerSocket = 0;
static const char* meterspercore[]      = {"CPI", "L2MissCycleRatio", "L3MissCycleRatio"};
static const char* meterspersocket[]    = {"MemoryBandwidth"};

static const char* eventsPerCore[]      = {
    "PERF_COUNT_HW_CPU_CYCLES,PERF_COUNT_HW_INSTRUCTIONS",
    "PERF_COUNT_HW_CPU_CYCLES,MEM_LOAD_RETIRED:LLC_UNSHARED_HIT,MEM_LOAD_RETIRED:OTHER_CORE_L2_HIT_HITM",
    "PERF_COUNT_HW_CPU_CYCLES,MEM_LOAD_RETIRED:LLC_MISS"};
static const int   eventsNumPerCore[]   = {2, 3, 2};

static const char* eventsPerSocket[]    = {"UNC_LLC_LINES_IN:ANY"};
static const int   eventsNumPerSocket[] = {1};

#elif defined(AMD_OPTERON)
static const int   numOfMetersPerCore   = 2;
static const int   numOfMetersPerSocket = 4;

static const char* metersPerCore[]      = {"CPI", "L2MissRatio"};
static const char* metersPerSocket[]    = {"L3MissRatio", "MemoryBandwidth", "MemoryConcurrency", "MemoryLatency"};

static const char* eventsPerCore[]      = {
    "PERF_COUNT_HW_CPU_CYCLES,PERF_COUNT_HW_INSTRUCTIONS",
    "PERF_COUNT_HW_INSTRUCTIONS,REQUESTS_TO_L2:ALL,L2_CACHE_MISS:ALL,L2_FILL_WRITEBACK:ALL"};
static const int   eventsNumPerCore[]   = {2, 4};

static const char* eventsPerSocket[]    = {
    "PERF_COUNT_HW_INSTRUCTIONS,READ_REQUEST_TO_L3_CACHE:ALL,L3_CACHE_MISSES:ALL",
    "PERF_COUNT_HW_INSTRUCTIONS,DRAM_ACCESSES_PAGE:ALL",
    "CPU_CLK_UNHALTED,L3_CACHE_MISSES:ALL,CPU_READ_COMMAND_REQUESTS_TO_TARGET_NODE_0_3:ALL",
    "CPU_CLK_UNHALTED,CPU_READ_COMMAND_LATENCY_TO_TARGET_NODE_0_3:ALL,CPU_READ_COMMAND_REQUESTS_TO_TARGET_NODE_0_3:ALL"};

//static const int eventsNumPerSocket[]   = {4, 2, 3, 3};
static const int eventsNumPerSocket[]   = {3, 2, 3, 3};
#endif

/*!
 * Add a meter value.
 *
 * \param q
 * \param val
 * \param ts
 *
 * \return void
 */
void addMeterValue(MeterValue **q, double val, uint64_t ts) {

    MeterValue* tempMeterValue;

    tempMeterValue            = (MeterValue*)malloc(sizeof(MeterValue));
    tempMeterValue->value     = val;
    tempMeterValue->timestamp = ts;
    tempMeterValue->next      = *q;
    *q                        = tempMeterValue;
}


/*!
 * Get average meter value.
 *
 * \param MeterValue** Pointer to a list of core meter
 *                <I>MeterValue</I> structures.
 * \param tw time window size (in nanosecond)
 *
 * \return Average meter value or 0 on error.
 */
double getAvgMeterValue(MeterValue **q, uint64_t tw) {
    double val = 0.0;
    double sum = 0.0;
    int    cnt = 0;
    MeterValue *temp, *r, *prev;
    uint64_t latestts;

    if (*q == NULL)
        return(0.0);

    temp = *q;
    sum += temp->value;
    latestts = temp->timestamp;
    cnt++;

    if (tw > MAX_TIME_WINDOW)
        tw = MAX_TIME_WINDOW;

    prev = temp;
    temp = temp->next;

    while (temp != NULL && (latestts - temp->timestamp) <= tw) {
        sum += temp->value;
        cnt++;
        temp = temp->next;
    }

    val = sum / cnt;

    prev->next = NULL;

    // Assuming that the time window size is not frequently changed
    // Remove too old values in the list
    if (temp != NULL && (latestts-temp->timestamp) > tw) {
        while (temp != NULL) {
            r    = temp;
            temp = temp->next;
            free(r);
        }
    }

    return(val);
}


/*!
 * Get max meter value.
 *
 * \param MeterValue** Pointer to a list of core meter
 *                <I>MeterValue</I> structures.
 * \param tw time window size (in nanosecond)
 *
 * \return Maximum meter value or 0 on error.
 */
double getMaxMeterValue(MeterValue **q, uint64_t tw) {
    double max = 0.0;
    MeterValue *tempMeterValue,*r,*prevMeterValue;
    uint64_t latestts;

    if (*q == NULL)
        return(0.0);

    tempMeterValue     = *q;
    max      = tempMeterValue->value;
    latestts = tempMeterValue->timestamp;

//    printf("max %f\n", temp->value);

    if (tw > MAX_TIME_WINDOW)
        tw = MAX_TIME_WINDOW;

    prevMeterValue = tempMeterValue;
    tempMeterValue = tempMeterValue->next;


    while (tempMeterValue != NULL && (latestts - tempMeterValue->timestamp) <= tw) {
//        printf("max %f, %llu\n", temp->value, temp->timestamp);
        if (tempMeterValue->value > max)
            max = tempMeterValue->value;
        prevMeterValue = tempMeterValue;
        tempMeterValue = tempMeterValue->next;
    }

    prevMeterValue->next = NULL;


    // Assuming that the time window size is not frequently changed.
    // Remove from the list values that are too old.
    if (tempMeterValue != NULL && (latestts-tempMeterValue->timestamp) > tw) {
        // printf("%llu, %llu, %llu\n", latestts, temp->timestamp, tw);
        while (tempMeterValue != NULL) {
            r    = tempMeterValue;
            tempMeterValue = tempMeterValue->next;
            free(r);
            //printf("freeing something\n");
        }
    }

    //printf("removed old data\n");

    return(max);
}


typedef struct _CPUINFO {
    int nodeID;
    int socketID;
    int coreID;
    char dfsPath[PATH_BUF_LENGTH];
} CPUINFO;

//static CPUINFO cpu_mapping_table[MAX_PROCESSORS];
static CPUINFO* cpu_mapping_table;

typedef struct _SOCKETINFO {
    int nodeID;
    int numOfProcessors;
    //int processorIDs[MAX_PROCESSORS];
    int* processorIDs;
    char dfsPath[PATH_BUF_LENGTH];
} SOCKETINFO;

//static SOCKETINFO socket_mapping_table[MAX_PROCESSORS];
static SOCKETINFO* socket_mapping_table;

// Hardware configurations
static int numOfProcessors;
static int numOfNodes;
static int numOfSockets;
static int numOfCoresPerSocket;
//static int numOfCacheLevels = 3;
// Hyperthreading enabled?
static int HTenabled;
static int cacheLineSize = 64;

//Daemon configuration variable
char bDaemon = 0;
char bVerbose = 0;
char bOutputToNull = 1;

/*!
 * Read the hardware configuration and allocate the proper memory for counters.
 * Initialize variables.
 *
 * \return int
 */
int initSystemConfiguration(void) {

    const int bufSize = 1024;
    char  cpuinfoBuf[bufSize];
    char* token;
    int   processorID = 0;
    int   socketID    = 0;
    int   coreID      = 0;
    int   maxSocketID = 0;
    int   maxCoreID   = 0;
    int   i, j, coreNum, first;

    if (bVerbose) printf("Initializing RCRdaemon.\n");

    //Allocate and fill in the socket and cpu mapping tables.
    socket_mapping_table = (SOCKETINFO*)malloc(numOfSockets * sizeof(SOCKETINFO));
    cpu_mapping_table    = (CPUINFO*)malloc(numOfProcessors * sizeof(CPUINFO));
    for (i = 0; i < numOfSockets; i++) {
        socket_mapping_table[i].numOfProcessors = 0;
        socket_mapping_table[i].processorIDs    = (int*)malloc(numOfProcessors * sizeof(int));
    }

    FILE *fd = fopen("/proc/cpuinfo", "r");

    while (fgets(cpuinfoBuf, bufSize, fd) != NULL) {
        if (strncmp(cpuinfoBuf, "processor", 9) == 0) {
            token = strtok(cpuinfoBuf, ":");
            token = strtok(NULL, ":");
            processorID = atoi(token);
        } else if (strncmp(cpuinfoBuf, "physical id", 11) == 0) {
            token = strtok(cpuinfoBuf, ":");
            token = strtok(NULL, ":");
            socketID = atoi(token);
            if (socketID > maxSocketID)
                maxSocketID = socketID;
            socket_mapping_table[socketID].nodeID = 0;
            socket_mapping_table[socketID].processorIDs[socket_mapping_table[socketID].numOfProcessors++] = processorID;
        } else if (strncmp(cpuinfoBuf, "core id", 7) == 0) {
            token = strtok(cpuinfoBuf, ":");
            token = strtok(NULL, ":");
            coreID = atoi(token);
            if (coreID > maxCoreID)
                maxCoreID = coreID;
        } else if (strncmp(cpuinfoBuf, "cpu cores", 9) == 0) {
            token = strtok(cpuinfoBuf, ":");
            token = strtok(NULL, ":");
            numOfCoresPerSocket = atoi(token);
            cpu_mapping_table[processorID].nodeID = 0;
            cpu_mapping_table[processorID].socketID = socketID;
            cpu_mapping_table[processorID].coreID = coreID;
        } else if (strncmp(cpuinfoBuf, "cache_alignment", 15) == 0) {
            token = strtok(cpuinfoBuf, ":");
            token = strtok(NULL, ":");
            cacheLineSize = atoi(token);
        }
    }

    fclose(fd);

    numOfNodes      = 1;
    numOfSockets    = maxSocketID + 1;
    numOfProcessors = processorID + 1;

    //Assume no hyperthreading for now
    if (0 && numOfProcessors == (numOfSockets * numOfCoresPerSocket))
        HTenabled = 0;
    else {
        HTenabled = 1;

        for (i = 0; i < numOfSockets; i++) {
            for (j = 0; j < numOfCoresPerSocket; j++) {
                first = 1;
                for (coreNum = 0; coreNum < numOfProcessors; coreNum++) {
                    if ((first == 1) && (cpu_mapping_table[coreNum].socketID == i) && (cpu_mapping_table[coreNum].coreID == j)) {
                        first = 0;
                        continue;
                    }
                    if ((first == 0) && (cpu_mapping_table[coreNum].socketID == i) && (cpu_mapping_table[coreNum].coreID == j)) {
                        cpu_mapping_table[coreNum].coreID = j + numOfCoresPerSocket;
                    }
                }
            }
        }
    }

    //Precompute the core directory paths.
    for (coreNum = 0; coreNum < numOfProcessors; coreNum++) {
        getCoreDirPath(coreNum, cpu_mapping_table[coreNum].dfsPath);
    }

    if (bVerbose) {
        printf("Nodes = %d  Sockets = %d  Processors = %d  CoresPreSocket = %d\n", numOfNodes, numOfSockets, numOfProcessors, numOfCoresPerSocket);
        for (coreNum = 0; coreNum < numOfProcessors; coreNum++) {
            printf("[%d, %d, %d]\n", cpu_mapping_table[coreNum].nodeID, cpu_mapping_table[coreNum].socketID, cpu_mapping_table[coreNum].coreID);
        }
    }
    return(0);
}

/*!
 *
 *
 * \param processorID
 * \param str
 *
 * \return int
 */
int getNodeDirPath(int processorID, char* nodeDirPath) {

    sprintf(nodeDirPath, "%s/Node_%04d", DFS_ROOTDIR_RCR, cpu_mapping_table[processorID].nodeID);

    return(0);
}

/*!
 *
 *
 * \param processorID
 * \param str
 *
 * \return int
 */
int getSocketDirPath(int processorID, char* socketDirPath) {

    sprintf(socketDirPath, "%s/Node_%04d/Socket_%04d", DFS_ROOTDIR_RCR, cpu_mapping_table[processorID].nodeID, cpu_mapping_table[processorID].socketID);

    return(0);
}

/*!
 * Returns the path in the debug file system that corresponds to the processor
 * whose ID is <I>processorID</I>
 *
 * \param processorID ID of processor to find the debug file system directory
 *                    for.
 * \param coreDirPath Returns the debug file system directory path of the
 *                    processor in question.
 *
 * \return int Returns 0.
 */
int getCoreDirPath(int processorID, char *coreDirPath) {

    sprintf(coreDirPath, "%s/Node_%04d/Socket_%04d/Core_%04d", DFS_ROOTDIR_RCR, cpu_mapping_table[processorID].nodeID, cpu_mapping_table[processorID].socketID, cpu_mapping_table[processorID].coreID);

    return(0);
}


/*!
 *
 *
 * \param processorID
 * \param meterNum
 *
 * \return int
 */
int getCoreMeterState(int processorID, int meterNum) {
    char dfsFilename[PATH_BUF_LENGTH];
    //char coreDirPath[PATH_BUF_LENGTH];
    uint64_t nodeEnabled, coreMeterEnabled;

    if ((meterNum < 0) && (meterNum >= numOfMetersPerCore))
        return(0);

    sprintf(dfsFilename, "%s/enable", DFS_ROOTDIR_RCR);

    FILE* fp = fopen(dfsFilename, "r");
    if (!fp) return(-1);

    fscanf(fp, "%lu", &nodeEnabled);
    fclose(fp);

    //getCoreDirPath(processorID, coreDirPath);
    sprintf(dfsFilename, "%s/%s/enable", cpu_mapping_table[processorID].dfsPath, metersPerCore[meterNum]);

    fp = fopen(dfsFilename, "r");
    if (!fp) return(-1);

    fscanf(fp, "%lu", &coreMeterEnabled);
    fclose(fp);

    return(nodeEnabled && coreMeterEnabled);
}


/*!
 *
 *
 * \param processorID
 * \param meterNum
 *
 * \return uint64_t Returns the core metere value, of -1 on failure.
 */
uint64_t getCoreMeterInterval(int processorID, int meterNum) {
    char     dfsMeterName[PATH_BUF_LENGTH];
    //char     coreDirPath[PATH_BUF_LENGTH];
    uint64_t coreMeterInterval;

    if ((meterNum < 0) && (meterNum >= numOfMetersPerCore))
        return(0);

    //getCoreDirPath(processorID, coreDirPath);
    sprintf(dfsMeterName, "%s/%s/interval", cpu_mapping_table[processorID].dfsPath, metersPerCore[meterNum]);

    FILE* fp = fopen(dfsMeterName, "r");
    if (!fp) return(-1);

    fscanf(fp, "%lu", &coreMeterInterval);
    fclose(fp);

    // In order to obtain valid event count numbers,
    // a minimum monitoring interval should be specified
    // By default, it is 1 millisecond.
//    if (buf == 0)
//        buf = 1000;

    return(coreMeterInterval);
}


/*!
 *
 *
 * \param processorID
 * \param meterNum
 *
 * \return uint64_t
 */
uint64_t getCoreMeterTimeWindow(int processorID, int meterNum) {
    char     dfsMeterName[PATH_BUF_LENGTH];
    //char     coreDirPath[PATH_BUF_LENGTH];
    uint64_t coreMeterTimeWindow;

    if (meterNum < 0 && meterNum >= numOfMetersPerCore)
        return(0);

    //getCoreDirPath(processorID, coreDirPath);
    sprintf(dfsMeterName, "%s/%s/timewindow", cpu_mapping_table[processorID].dfsPath, metersPerCore[meterNum]);

    FILE* fp = fopen(dfsMeterName, "r");
    if (!fp) return(-1);

    fscanf(fp,"%lu", &coreMeterTimeWindow);
    fclose(fp);

    return(coreMeterTimeWindow);
}


/*!
 *
 *
 * \param processorID
 * \param meterNum
 * \param type
 *
 * \return uint64_t
 */
uint64_t getCoreMeter(int processorID, int meterNum, RCR_type meterType) {
    char     dfsMeterName[PATH_BUF_LENGTH];
    //char     coreDirPath[PATH_BUF_LENGTH];
    uint64_t coreMeter;

    if ((meterNum < 0) && (meterNum >= numOfMetersPerCore))
        return(0);

    //getCoreDirPath(processorID, coreDirPath);

    switch (meterType) {
        case RCR_TYPE_IMMEDIATE:
            sprintf(dfsMeterName, "%s/%s/current", cpu_mapping_table[processorID].dfsPath, metersPerCore[meterNum]);
            break;
        case RCR_TYPE_AVERAGE:
            sprintf(dfsMeterName, "%s/%s/average", cpu_mapping_table[processorID].dfsPath, metersPerCore[meterNum]);
            break;
        case RCR_TYPE_SUM:
            break;
        case RCR_TYPE_MAXIMUM:
            sprintf(dfsMeterName, "%s/%s/maximum", cpu_mapping_table[processorID].dfsPath, metersPerCore[meterNum]);
            break;
    }

    FILE* fp = fopen(dfsMeterName, "r");
    if (!fp) return(-1);

    fscanf(fp,"%lu", &coreMeter);

    fclose(fp);

    return(coreMeter);
}


/*!
 *
 *
 * \param processorID
 * \param meterNum
 * \param type
 * \param value
 *
 * \return int 0 on success, or -1 on failure.
 */
int setCoreMeter(int processorID, int meterNum, RCR_type meterType, uint64_t value) {
    char dfsMeterName[PATH_BUF_LENGTH];
    //char coreDirPath[PATH_BUF_LENGTH];

    if (meterNum < 0 && meterNum >= numOfMetersPerCore)
        return(-1);

    //getCoreDirPath(processorID, coreDirPath);

    switch (meterType) {
        case RCR_TYPE_IMMEDIATE:
            sprintf(dfsMeterName, "%s/%s/current", cpu_mapping_table[processorID].dfsPath, metersPerCore[meterNum]);
            break;
        case RCR_TYPE_AVERAGE:
            sprintf(dfsMeterName, "%s/%s/average", cpu_mapping_table[processorID].dfsPath, metersPerCore[meterNum]);
            break;
        case RCR_TYPE_SUM:
            break;
        case RCR_TYPE_MAXIMUM:
            sprintf(dfsMeterName, "%s/%s/maximum", cpu_mapping_table[processorID].dfsPath, metersPerCore[meterNum]);
            break;
    }

    FILE* fp = fopen(dfsMeterName, "w");
    if (!fp) return(-1);

    value =

    fprintf(fp,"%lu\n", value);

    fclose(fp);

    return(0);
}


/*!
 *
 *
 * \param processorID
 * \param meterNum
 *
 * \return int
 */
int getSocketMeterState(int processorID, int meterNum) {
    char dfsMeterName[PATH_BUF_LENGTH];
    char socketDirPath[PATH_BUF_LENGTH];
    uint64_t buf1, buf2;

    if (meterNum < 0 && meterNum >= numOfMetersPerSocket)
        return(0);

    sprintf(dfsMeterName, "%s/enable", DFS_ROOTDIR_RCR);

    FILE* fp = fopen(dfsMeterName, "r");
    if (!fp) return(-1);

    fscanf(fp,"%lu", &buf1);
    fclose(fp);


    getSocketDirPath(processorID, socketDirPath);
    sprintf(dfsMeterName, "%s/%s/enable", socketDirPath, metersPerSocket[meterNum]);

    fp = fopen(dfsMeterName, "r");
    if (!fp) return(-1);

    fscanf(fp,"%lu", &buf2);
    fclose(fp);

    return(buf1 && buf2);
}


/*!
 *
 *
 * \param processorID
 * \param meterNum
 *
 * \return uint64_t
 */
uint64_t getSocketMeterInterval(int processorID, int meterNum) {
    char dfsMeterName[PATH_BUF_LENGTH];
    uint64_t buf;
    char str[PATH_BUF_LENGTH];

    if (meterNum < 0 && meterNum >= numOfMetersPerSocket)
        return(0);

    getSocketDirPath(processorID, str);
    sprintf(dfsMeterName, "%s/%s/interval", str, metersPerSocket[meterNum]);

    FILE* fp = fopen(dfsMeterName, "r");
    if (!fp) return(-1);

    fscanf(fp,"%lu", &buf);
    fclose(fp);

    // In order to obtain valid event count numbers,
    // a minimum monitoring interval should be specified
    // By default, it is 1 millisecond.
//    if (buf == 0)
//        buf = 1000;

    return(buf);
}


/*!
 *
 *
 * \param processorID
 * \param meterNum
 *
 * \return uint64_t
 */
uint64_t getSocketMeterTimeWindow(int processorID, int meterNum) {
    char dfsMeterName[PATH_BUF_LENGTH];
    uint64_t buf;
    char str[PATH_BUF_LENGTH];

    if (meterNum < 0 && meterNum >= numOfMetersPerSocket)
        return(0);

    getSocketDirPath(processorID, str);
    sprintf(dfsMeterName, "%s/%s/timewindow", str, metersPerSocket[meterNum]);

    FILE* fp = fopen(dfsMeterName, "r");
    if (!fp) return(-1);

    fscanf(fp,"%lu", &buf);
    fclose(fp);

    return(buf);
}

/*!
 *
 *
 * \param processorID
 * \param meterNum
 * \param type
 *
 * \return uint64_t
 */
uint64_t getSocketMeter(int processorID, int meterNum, RCR_type meterType) {
    char dfsMeterName[PATH_BUF_LENGTH];
    char socketDirPath[PATH_BUF_LENGTH];
    uint64_t socketMeterValue;

    if (meterNum < 0 && meterNum >= numOfMetersPerSocket)
        return(0);

    getSocketDirPath(processorID, socketDirPath);

    switch (meterType) {
        case RCR_TYPE_IMMEDIATE:
            sprintf(dfsMeterName, "%s/%s/current", socketDirPath, metersPerSocket[meterNum]);
            break;
        case RCR_TYPE_AVERAGE:
            sprintf(dfsMeterName, "%s/%s/average", socketDirPath, metersPerSocket[meterNum]);
            break;
        case RCR_TYPE_SUM:
            break;
        case RCR_TYPE_MAXIMUM:
            sprintf(dfsMeterName, "%s/%s/maximum", socketDirPath, metersPerSocket[meterNum]);
            break;
    }

    FILE* fp = fopen(dfsMeterName, "r");
    if (!fp) return(-1);

    fscanf(fp,"%lu", &socketMeterValue);

    fclose(fp);

    return(socketMeterValue);
}

/*!
 *
 *
 * \param processorID
 * \param meterNum
 * \param type
 * \param value
 *
 * \return int
 */
int setSocketMeter(int processorID, int meterNum, RCR_type meterType, uint64_t value) {
    char dfsMeterName[PATH_BUF_LENGTH];
    char socketDirPath[PATH_BUF_LENGTH];

    if (meterNum < 0 && meterNum >= numOfMetersPerSocket)
        return(-1);

    getSocketDirPath(processorID, socketDirPath);

    switch (meterType) {
        case RCR_TYPE_IMMEDIATE:
            sprintf(dfsMeterName, "%s/%s/current", socketDirPath, metersPerSocket[meterNum]);
            break;
        case RCR_TYPE_AVERAGE:
            sprintf(dfsMeterName, "%s/%s/average", socketDirPath, metersPerSocket[meterNum]);
            break;
        case RCR_TYPE_SUM:
            break;
        case RCR_TYPE_MAXIMUM:
            sprintf(dfsMeterName, "%s/%s/maximum", socketDirPath, metersPerSocket[meterNum]);
            break;
    }

    FILE* fp = fopen(dfsMeterName, "w");
    if (!fp) return(-1);

    value =

    fprintf(fp,"%lu\n", value);

    fclose(fp);

    return(0);
}


/*!
 * Returns the reset value, stored at node level in the debug filesystem.
 *
 * \return uint64_t Returns the value of the reset flag, otherwise return -1 
 *         on error
 */
uint64_t getResetValue(void) {
    char dfsMeterName[PATH_BUF_LENGTH];
    uint64_t resetValue;

    sprintf(dfsMeterName, "%s/reset", DFS_ROOTDIR_RCR);

    FILE* fp = fopen(dfsMeterName, "r");
    if (!fp) return(-1);

    fscanf(fp,"%lu", &resetValue);

    fclose(fp);

    return(resetValue);
}


/*!
 * Set the reset flag in the dfs system to 0.  This is at the node level.
 *
 * \return int Returns 0 on success, or -1 on error.
 */
int setResetValue(void) {
    char dfsMeterName[PATH_BUF_LENGTH];

    sprintf(dfsMeterName, "%s/reset", DFS_ROOTDIR_RCR);

    FILE* fp = fopen(dfsMeterName, "w");
    if (!fp) return(-1);

    fprintf(fp,"%u\n", 0);

    fclose(fp);

    return(0);
}

/*
int createAppDFSEntry(const char* appName) {
    char filename[PATH_BUF_LENGTH];

    sprintf(filename, "%s/%s", DFS_ROOTDIR_RAT, appName);

    FILE* fp = fopen(filename, "w+");
    if (!fp) return -1;

    fclose(fp);
    return 0;
}
*/

/*!
 * Complain with perror, then exit.
 * 
 * \param msg Nature of the problem (don't include newline)
 */
void die(char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/*!
 * 
 * 
 * \param bOutputToNull 
 */
void daemonize(char bOutputToNull) {
    int fd;

    switch (fork()) {
    case -1:
        die("Unable to fork");
    case 0:
        break;
    default:
        exit(0);
    }

    if (setsid() == -1) {
        die("Unable to set process group");
    }

    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        dup2(fd, STDIN_FILENO);
        if (bOutputToNull) dup2(fd, STDOUT_FILENO);
        if (bOutputToNull) dup2(fd, STDERR_FILENO);
        if (fd > 2) close(fd);
    } else {
        die("Unable to detach file discriptors");
    }
}

/*!
 * 
 */
void doWork(int nshepherds, int nworkerspershep) {
    
    numOfSockets    = nshepherds;
    numOfProcessors = nshepherds * nworkerspershep;

    initSystemConfiguration();

    //int status, ncpus
    int ret, i, j, cpu, meternum, sock, num;

    struct timespec interval, freq, remainder;
    interval.tv_sec  = MONITORINTERVALINSEC;
    interval.tv_nsec = MONITORINTERVALINNANOSEC;
    freq.tv_sec      = UPDATEFREQUENCYINSEC;
    freq.tv_nsec     = UPDATEFREQUENCYINNANOSEC;

    double currentVal, maxVal, avgVal;

    MeterValue **core_meter_list   = malloc(numOfProcessors * numOfMetersPerCore   * sizeof(MeterValue *));
    for (i=0; i<numOfProcessors * numOfMetersPerCore; i++)
        core_meter_list[i] = NULL;

    MeterValue **socket_meter_list = malloc(numOfSockets    * numOfMetersPerSocket * sizeof(MeterValue *));
    for (i=0; i<numOfSockets * numOfMetersPerSocket; i++)
        socket_meter_list[i] = NULL;

    uint64_t *valuesPerCore   = malloc(numOfProcessors * MAXNUMOFEVENTS * sizeof(uint64_t));
    uint64_t *timePerCore     = malloc(numOfProcessors * MAXNUMOFEVENTS * sizeof(uint64_t));
    uint64_t *valuesPerSocket = malloc(numOfSockets    * MAXNUMOFEVENTS * sizeof(uint64_t));
    uint64_t *timePerSocket   = malloc(numOfSockets    * MAXNUMOFEVENTS * sizeof(uint64_t));

    // Setup the perf events
    // Open performance descriptors for all events
    if (bVerbose) printf("Setup the perf events.\n");

    perf_event_desc_t *fds         = NULL;
    perf_event_desc_t **core_fds   = malloc(numOfProcessors * numOfMetersPerCore   * sizeof(perf_event_desc_t));
    perf_event_desc_t **socket_fds = malloc(numOfSockets    * numOfMetersPerSocket * sizeof(perf_event_desc_t));

    // for per-core events
    for (cpu=0; cpu<numOfProcessors; cpu++) {
        for (meternum=0; meternum<numOfMetersPerCore; meternum++) {
            num = perf_setup_list_events(eventsPerCore[meternum], &fds);
            if (num < 1)
                errx(1, "cannot setup per-core events\n");

            core_fds[numOfMetersPerCore*cpu+meternum] = fds;
            fds->fd = -1;

            for (i=0; i < eventsNumPerCore[meternum]; i++) {
                fds[i].hw.disabled = 1;
                fds[i].hw.enable_on_exec = 1;
                fds[i].hw.pinned = 1;
                fds[i].hw.read_format = PERF_FORMAT_TOTAL_TIME_RUNNING;
                fds[i].fd = perf_event_open(&fds[i].hw, -1, cpu, -1, 0);

                if (fds[i].fd == -1)
                    err(1, "cannot attach per-core event to CPU%d %s", cpu, fds[i].name);
            }
        }
    }

// Uncore events are not available for Intel Nehalem chip
#ifdef AMD_OPTERON
    // Then, for per-socket events shared by cores
    for (sock = 0; sock < numOfSockets; sock++) {
        for (meternum = 0; meternum < numOfMetersPerSocket; meternum++) {
            num = perf_setup_list_events(eventsPerSocket[meternum], &fds);
            if (num < 1)
                errx(1, "cannot setup per-socket events\n");

            socket_fds[numOfMetersPerSocket * sock + meternum] = fds;
            fds[0].fd = -1;

            for (i = 0; i < eventsNumPerSocket[meternum]; i++) {
                fds[i].hw.disabled       = 1;
                fds[i].hw.enable_on_exec = 1;
                fds[i].hw.pinned         = 1;
                fds[i].hw.read_format    = PERF_FORMAT_TOTAL_TIME_RUNNING;
                fds[i].fd                = perf_event_open(&fds[i].hw, -1, socket_mapping_table[sock].processorIDs[0], -1, 0);

                if (fds[i].fd == -1)
                    err(1, "cannot attach %d socket event to CPU%d %s", sock, socket_mapping_table[sock].processorIDs[0], fds[i].name);
            }
        }
    }
#endif


    if (bVerbose) printf("Started counting events.\n");

    uint64_t delta[MAXNUMOFEVENTS];
    uint64_t elapsedtime[MAXNUMOFEVENTS];
    uint64_t value[2];

    for (i=0; i<numOfProcessors*MAXNUMOFEVENTS; i++) {
        valuesPerCore[i] = 0;
        timePerCore[i] = 0;
    }

    for (i=0; i<numOfSockets*MAXNUMOFEVENTS; i++) {
        valuesPerSocket[i] = 0;
        timePerSocket[i] = 0;
    }

    int idleinterval = 1;

    int *enabled = malloc(sizeof(int) * numOfProcessors);
    for (i=0; i<numOfProcessors; i++) {
        enabled[i] = 0;
    }

    struct timeval startts;
    //struct timeval curts;

    gettimeofday(&startts, NULL);

    int enabledcnt = 0;

//    int testcnt = 0;
//    for (testcnt = 0; testcnt<LATENCYTEST; testcnt++) {
    int firstTimeThroughLoop[16];
    unsigned long int totalL3CacheMissesEnd[16];
    unsigned long int totalL3CacheMissesStart[16];
    for (i=0; i<numOfSockets; i++) {
        firstTimeThroughLoop[i] = 1;
        totalL3CacheMissesEnd[i] = 0;
        totalL3CacheMissesStart[i] = 0;
    }
    while (rcrToolContinue) {

        if (getResetValue() == 1) {
            // Disable to post all the values of working models

            setResetValue();
            idleinterval = 1;
        }

        // Update the values obtained by per-core events
        for (meternum=0; meternum < numOfMetersPerCore; meternum++) {

            enabledcnt = 0;

            //Read events per core
            for (i=0; i < numOfProcessors; i++) {
                if (getCoreMeterState(i, meternum)) {
                    enabled[i] = 1;
                    enabledcnt++;
                } else {
                    enabled[i] = 0;
                }
            }

            for (i=0; i < numOfProcessors; i++) {
                if (enabled[i] == 0)
                    continue;
                fds = core_fds[numOfMetersPerCore*i+meternum];
                for (j=0; j < eventsNumPerCore[meternum]; j++) {
                    ret = ioctl(fds[j].fd, PERF_EVENT_IOC_ENABLE, 0);
                    if (ret)
                        err(1, "cannot enable core event %s\n", fds[j].name);
                }
            }

            for (i=0; i < numOfProcessors; i++) {
                if (enabled[i] == 0)
                    continue;

                fds = core_fds[numOfMetersPerCore*i+meternum];
                for (j=0; j < eventsNumPerCore[meternum]; j++) {
                    ret = read(fds[j].fd, value, sizeof(value));
                    if (ret != sizeof(value)) {
                        if (ret == -1)
                            err(1, "cannot read core event %d on %d: %d", j, i, ret);
                        else
                            warnx("could not read core event %d on %d", j, i);
                    }
                    valuesPerCore[i*MAXNUMOFEVENTS+j] = value[0];
                    timePerCore[i*MAXNUMOFEVENTS+j] = value[1];
                }
            }
            //Finished reading events

            // The interval is supposed to set individually for each meter.
            // However, in this version, the intervals set to the meters of each core are used
//            if (bVerbose)
//                if (interval.tv_sec != getCoreMeterInterval(0, meternum)/1000000 ||
//                    interval.tv_nsec != (getCoreMeterInterval(0, meternum) - interval.tv_sec*1000000) * 1000)
//                    printf("[Core] %s meter monitoring interval is changed\n", meterspercore[meternum]);

            //printf("Before core sleep\n");
            //printf("%lu, %lu\n", getCoreMeterInterval(0, meternum)/1000000, (getCoreMeterInterval(0, meternum) - interval.tv_sec*1000000) * 1000);
            interval.tv_sec = getCoreMeterInterval(0, meternum)/1000000;
            interval.tv_nsec = (getCoreMeterInterval(0, meternum) - interval.tv_sec*1000000) * 1000;
            if (enabledcnt > 0 &&  (interval.tv_sec > 0 || interval.tv_nsec > 0)) {
                nanosleep(&interval, &remainder);
//                //printf("nanosleep during %lu.%09lu\n", interval.tv_sec, interval.tv_nsec);
            }
            //printf("After core sleep\n");

            for (i=0; i < numOfProcessors; i++) {
                if (enabled[i] == 0)
                    continue;

                fds = core_fds[numOfMetersPerCore*i+meternum];
                for (j=0; j < eventsNumPerCore[meternum]; j++) {
                    ret = read(fds[j].fd, value, sizeof(value));
                    if (ret != sizeof(value)) {
                        if (ret == -1)
                            err(1, "cannot read core event %d on %d: %d", j, i, ret);
                        else
                            warnx("could not read core event %d on %d", j, i);
                    }
                    delta[j] =  value[0] - valuesPerCore[i*MAXNUMOFEVENTS+j];
                    //valuesPerCore[j] = value[0];
                    elapsedtime[j] =  value[1] - timePerCore[i*MAXNUMOFEVENTS+j];
                    //timePerCore[j] = value[1];

//                    printf("%llu, %llu\n", delta[j], elapsedtime[j]);
                }

                // Update CPI for each core
                if (strncmp(metersPerCore[meternum], "CPI", 3) == 0) {
                    if (delta[1] != 0) {
                        currentVal = delta[0] * 1.0  / delta[1];
                    } else {
                        if (bVerbose)
                            printf("[Core %d] Could not get CPI.\n", i);
                        currentVal = 0.0;
                    }

                    setCoreMeter(i, meternum, RCR_TYPE_IMMEDIATE, currentVal*1000000);

                    addMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), currentVal, value[1]);
                    maxVal = getMaxMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), getCoreMeterTimeWindow(i, meternum));
                    setCoreMeter(i, meternum, RCR_TYPE_MAXIMUM, maxVal*1000000);

                    avgVal = getAvgMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), getCoreMeterTimeWindow(i, meternum));
                    setCoreMeter(i, meternum, RCR_TYPE_AVERAGE, avgVal*1000000);

                    /*
                       leave breadcrumbs
                    */
                    putBreadcrumbs(TYPE_CORE, i, "CPI", currentVal);

                    if (bVerbose)
                        printf("[Core %d] CPI: Current=%f, Max=%f, Avg=%f.\n", i, currentVal, maxVal, avgVal);

                } else if (strncmp(metersPerCore[meternum], "L2MissCycleRatio", 16) == 0) {
                    if (delta[0] != 0) {
                        currentVal = (delta[1] * 35.0 +  delta[2] * 74.0)/ delta[0];
                    } else {
                        if (bVerbose) {
                            printf("[Core %d] Could not get L2 Miss Cycle Ratio.\n", i);
                        }
                        currentVal = 0.0;
                    }

                    setCoreMeter(i, meternum, RCR_TYPE_IMMEDIATE, currentVal*1000000);

                    addMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), currentVal, value[1]);
                    maxVal = getMaxMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), getCoreMeterTimeWindow(i, meternum));
                    setCoreMeter(i, meternum, RCR_TYPE_MAXIMUM, maxVal*1000000);

                    avgVal = getAvgMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), getCoreMeterTimeWindow(i, meternum));
                    setCoreMeter(i, meternum, RCR_TYPE_AVERAGE, avgVal*1000000);

                    /*
                       leave breadcrumbs
                    */
                    putBreadcrumbs(TYPE_CORE, i, "L2MissCycleRatio", currentVal);

                    if (bVerbose)
                        printf("[Core %d] L2 Miss Cycle Ratio: Current=%f, Max=%f, Avg=%f.\n", i, currentVal, maxVal, avgVal);

                } else if (strncmp(metersPerCore[meternum], "L3MissCycleRatio", 16) == 0) {
                    if (delta[0] != 0) {
                        currentVal = (delta[1] * 180.0)/ delta[0];
                    } else {
                        if (bVerbose) {
                            printf("[Core %d] Could not get L3 Miss Cycle Ratio.\n", i);
                        }
                        currentVal = 0.0;
                    }

                    setCoreMeter(i, meternum, RCR_TYPE_IMMEDIATE, currentVal*1000000);

                    addMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), currentVal, value[1]);
                    maxVal = getMaxMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), getCoreMeterTimeWindow(i, meternum));
                    setCoreMeter(i, meternum, RCR_TYPE_MAXIMUM, maxVal*1000000);

                    avgVal = getAvgMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), getCoreMeterTimeWindow(i, meternum));
                    setCoreMeter(i, meternum, RCR_TYPE_AVERAGE, avgVal*1000000);

                    /*
                       leave breadcrumbs
                    */
                    putBreadcrumbs(TYPE_CORE, i, "L3MissCycleRatio", currentVal);

                    if (bVerbose)
                        printf("[Core %d] L3 Miss Cycle Ratio: Current=%f, Max=%f, Avg=%f.\n", i, currentVal, maxVal, avgVal);

                } else if (strncmp(metersPerCore[meternum], "L2MissRatio", 11) == 0) {
                    if (delta[1]+delta[3] != 0) {
                        currentVal = (double) delta[2] / (delta[1] + delta[3]);
                    } else {
                        if (bVerbose) {
                            printf("[Core %d] Could not get L2 Miss Ratio.\n", i);
                        }
                        currentVal = 0.0;
                    }

                    setCoreMeter(i, meternum, RCR_TYPE_IMMEDIATE, currentVal*1000000);

                    addMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), currentVal, value[1]);
                    maxVal = getMaxMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), getCoreMeterTimeWindow(i, meternum));
                    setCoreMeter(i, meternum, RCR_TYPE_MAXIMUM, maxVal*1000000);

                    avgVal = getAvgMeterValue(&(core_meter_list[numOfMetersPerCore*i+meternum]), getCoreMeterTimeWindow(i, meternum));
                    setCoreMeter(i, meternum, RCR_TYPE_AVERAGE, avgVal*1000000);

                    /*
                       leave breadcrumbs
                    */
                    putBreadcrumbs(TYPE_CORE, i, "L2MissRatio", currentVal);

                    if (bVerbose)
                        printf("[Core %d] L2 Miss Ratio: Current=%f, Max=%f, Avg=%f.\n", i, currentVal, maxVal, avgVal);
                }
            }

            for (i=0; i < numOfProcessors; i++) {
                if (enabled[i] == 0)
                    continue;

                fds = core_fds[numOfMetersPerCore*i+meternum];
                for (j=0; j < eventsNumPerCore[meternum]; j++) {
                    ret = ioctl(fds[j].fd, PERF_EVENT_IOC_DISABLE, 0);
                    if (ret)
                        err(1, "cannot disable core event %s\n", fds[j].name);
                }
            }
        }

        //gettimeofday(&curts, NULL);
        //printf("TIME=%f\n", (curts.tv_sec+curts.tv_usec/1000000.0)-(startts.tv_sec+startts.tv_usec/1000000.0));


// For now,
// The uncore events are not yet supported in Linux Perf Events
// Uncomment the following code when it is available
// Only works for AMD Opteron machine

        // Update the metrics using shared events, such as Memory Bandwidth, L3 Cache Misses
        // Update the values obtained by per-core events
        for (meternum=0; meternum < numOfMetersPerSocket; meternum++) {

            enabledcnt = 0;


            for (i=0; i < numOfProcessors; i++) {
                if (getSocketMeterState(i, meternum)) {
                    enabled[i] = 1;
                    enabledcnt++;
                } else {
                    enabled[i] = 0;
                }
            }

            for (i=0; i < numOfSockets; i++) {
                if (enabled[i] == 0)
                    continue;

                fds = socket_fds[numOfMetersPerSocket*i+meternum];
                for (j=0; j < eventsNumPerSocket[meternum]; j++) {
                    ret = ioctl(fds[j].fd, PERF_EVENT_IOC_ENABLE, 0);
                    if (ret)
                        err(1, "cannot enable shared event %s\n", fds[j].name);
                }
            }

            for (i=0; i < numOfSockets; i++) {
                if (enabled[i] == 0)
                    continue;

                fds = socket_fds[numOfMetersPerSocket*i+meternum];
                for (j=0; j < eventsNumPerSocket[meternum]; j++) {
                    ret = read(fds[j].fd, value, sizeof(value));
                    if (ret != sizeof(value)) {
                        if (ret == -1)
                            err(1, "cannot read shared event %d on %d: %d", j, i, ret);
                        else
                            warnx("could not read shared event %d on %d", j, i);
                    }

                    if (meternum == 2 && j == 1) {
                        if (firstTimeThroughLoop[i]) {
                            totalL3CacheMissesStart[i] = value[0];
                            firstTimeThroughLoop[i] = 0;
                        }
                        totalL3CacheMissesEnd[i] = value[0];
                    }
                    valuesPerSocket[i*MAXNUMOFEVENTS+j] = value[0];
                    timePerSocket[i*MAXNUMOFEVENTS+j] = value[1];
//                    printf("[Socket %d BEFORE] %llu\n", i, value[0]);
                }
            }

//            if (bVerbose)
//                if (interval.tv_sec != getSocketMeterInterval(0, meternum)/1000000 ||
//                    interval.tv_nsec != (getSocketMeterInterval(0, meternum) - interval.tv_sec*1000000) * 1000)
//                    printf("[Socket] %s meter monitoring interval is changed\n", meterspersocket[meternum]);

            //printf("Before socket sleep\n");
            interval.tv_sec = getSocketMeterInterval(0, meternum)/1000000;
            interval.tv_nsec = (getSocketMeterInterval(0, meternum) - interval.tv_sec*1000000) * 1000;
            if (enabledcnt > 0 &&  (interval.tv_sec > 0 || interval.tv_nsec > 0)) {
                nanosleep(&interval, &remainder);
//                //printf("nanosleep during %lu.%09lu\n", interval.tv_sec, interval.tv_nsec);
            }
            //printf("After socket sleep\n");

            for (i=0; i < numOfSockets; i++) {
                if (enabled[i] == 0)
                    continue;

                fds = socket_fds[numOfMetersPerSocket*i+meternum];
                for (j=0; j < eventsNumPerSocket[meternum]; j++) {
                    ret = read(fds[j].fd, value, sizeof(value));
                    if (ret != sizeof(value)) {
                        if (ret == -1)
                            err(1, "cannot read shared event %d on %d: %d", j, i, ret);
                        else
                            warnx("could not read shared event %d on %d", j, i);
                    }
                    delta[j] =  value[0] - valuesPerSocket[i*MAXNUMOFEVENTS+j];
                    //valuesPerSocket[j] = value[0];
                    elapsedtime[j] =  value[1] - timePerSocket[i*MAXNUMOFEVENTS+j];
                    //timePerSocket[j] = value[1];

//                    printf("[Socket %d AFTER] %llu\n", i, value[0]);
//                    printf("[Socket %d DIFF] %llu\n", i, delta[j]);
                }


                // Update MemoryBandwidth for each socket
                if (strncmp(metersPerSocket[meternum], "MemoryBandwidth", 15) == 0) {
#ifdef INTEL_NEHALEM
//            if (elapsedtime[0] != 0.0) {
//                MemoryBandwidth[i] = (delta[0] * 1.0 * cacheLineSize)  / elapsedtime[0];
//
//                if (bVerbose)
//                    printf("[Socket %d] Memory Bandwidth (B/s) %f, %f.\n", i, getMemoryBandwidth(i)/1000000.0, MemoryBandwidth[i]);
//              setMemoryBandwidth(i, MemoryBandwidth[i]*1000000);
//            } else
//                if (bVerbose)
//                    printf("[Socket %d] Could not get Memory Bandwidth.\n", i);
//
#elif defined(AMD_OPTERON)
                    if (elapsedtime[1] != 0) {
                        currentVal = ((double) delta[1] * 1.0 * cacheLineSize) / (double) elapsedtime[1];
                    } else {
                        if (bVerbose) {
                            printf("[Socket %d] Could not get Memory Bandwidth.\n", i);
                        }
                        currentVal = 0.0;
                    }

                    setSocketMeter(i, meternum, RCR_TYPE_IMMEDIATE, currentVal*1000000);

                    addMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), currentVal, value[1]);
                    maxVal = getMaxMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), getSocketMeterTimeWindow(i, meternum));
                    setSocketMeter(i, meternum, RCR_TYPE_MAXIMUM, maxVal*1000000);

                    avgVal = getAvgMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), getSocketMeterTimeWindow(i, meternum));
                    setSocketMeter(i, meternum, RCR_TYPE_AVERAGE, avgVal*1000000);

                    /*
                       leave breadcrumbs
                    */
                    putBreadcrumbs(TYPE_SOCKET, i, "MemoryBandwidth", currentVal);

                    if (bVerbose)
                        printf("[Socket %d] Memory Bandwidth (B/s): Current=%f, Max=%f, Avg=%f.\n", i, currentVal, maxVal, avgVal);

#endif
                } else if (strncmp(metersPerSocket[meternum], "L3MissRatio", 11) == 0) {
                    if (delta[1] != 0) {
                        currentVal = (double) delta[2] / (double) delta[1];
                    } else {
                        if (bVerbose) {
                            printf("[Socket %d] Could not get L3 Miss Ratio.\n", i);
                        }
                        currentVal = 0.0;
                    }

                    setSocketMeter(i, meternum, RCR_TYPE_IMMEDIATE, currentVal*1000000);

                    addMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), currentVal, value[1]);
                    maxVal = getMaxMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), getSocketMeterTimeWindow(i, meternum));
                    setSocketMeter(i, meternum, RCR_TYPE_MAXIMUM, maxVal*1000000);

                    avgVal = getAvgMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), getSocketMeterTimeWindow(i, meternum));
                    setSocketMeter(i, meternum, RCR_TYPE_AVERAGE, avgVal*1000000);

                    /*
                       leave breadcrumbs
                    */
                    putBreadcrumbs(TYPE_SOCKET, i, "L3MissRatio", currentVal);

                    if (bVerbose)
                        printf("[Socket %d] L3 Miss Ratio: Current=%f, Max=%f, Avg=%f.\n", i, currentVal, maxVal, avgVal);

                } else if (strncmp(metersPerSocket[meternum], "MemoryConcurrency", 17) == 0) {
                    if (delta[2] != 0) {
                        currentVal = (double) delta[1] / (double) delta[2];
                    } else {
                        if (bVerbose) {
                            printf("[Socket %d] Could not get Memory Concurrency.\n", i);
                        }
                        currentVal = 0.0;
                    }

                    setSocketMeter(i, meternum, RCR_TYPE_IMMEDIATE, currentVal*1000000);

                    addMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), currentVal, value[1]);
                    maxVal = getMaxMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), getSocketMeterTimeWindow(i, meternum));
                    setSocketMeter(i, meternum, RCR_TYPE_MAXIMUM, maxVal*1000000);

                    avgVal = getAvgMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), getSocketMeterTimeWindow(i, meternum));
                    setSocketMeter(i, meternum, RCR_TYPE_AVERAGE, avgVal*1000000);

                    /*
                       leave breadcrumbs
                    */
                    putBreadcrumbs(TYPE_SOCKET, i, "MemoryConcurrency", currentVal);

                    if (bVerbose)
                        printf("[Socket %d] Memory Concurrency: Current=%f, Max=%f, Avg=%f.\n", i, currentVal, maxVal, avgVal);

                } else if (strncmp(metersPerSocket[meternum], "MemoryLatency", 13) == 0) {
                    if (delta[2] != 0) {
                        currentVal = (double) delta[1] / (double) delta[2];
                    } else {
                        if (bVerbose) {
                            printf("[Socket %d] Could not get Memory Latency.\n", i);
                        }
                        currentVal = 0.0;
                    }

                    setSocketMeter(i, meternum, RCR_TYPE_IMMEDIATE, currentVal*1000000);

                    addMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), currentVal, value[1]);
                    maxVal = getMaxMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), getSocketMeterTimeWindow(i, meternum));
                    setSocketMeter(i, meternum, RCR_TYPE_MAXIMUM, maxVal*1000000);

                    avgVal = getAvgMeterValue(&(socket_meter_list[numOfMetersPerSocket*i+meternum]), getSocketMeterTimeWindow(i, meternum));
                    setSocketMeter(i, meternum, RCR_TYPE_AVERAGE, avgVal*1000000);

                    /*
                       leave breadcrumbs
                    */
                    putBreadcrumbs(TYPE_SOCKET, i, "MemoryLatency", currentVal);

                    if (bVerbose)
                        printf("[Socket %d] Memory Latency: Current=%f, Max=%f, Avg=%f.\n", i, currentVal, maxVal, avgVal);
                }
            }

            for (i=0; i < numOfSockets; i++) {
                if (enabled[i] == 0)
                    continue;

                fds = socket_fds[numOfMetersPerSocket*i+meternum];
                for (j=0; j <  eventsNumPerSocket[meternum]; j++) {
                    ret = ioctl(fds[j].fd, PERF_EVENT_IOC_DISABLE, 0);
                    if (ret)
                        err(1, "cannot disable shared event %s\n", fds[j].name);
                }
            }
        }



        //gettimeofday(&curts, NULL);
        //printf("TIME=%f\n", (curts.tv_sec+curts.tv_usec/1000000.0)-(startts.tv_sec+startts.tv_usec/1000000.0));

        //printf("Before loop sleep\n");
        if (freq.tv_sec != 0 || freq.tv_nsec != 0)
            nanosleep(&freq, &remainder);
        //printf("After loop sleep\n");

        //gettimeofday(&curts, NULL);
        //printf("TIME=%f\n", (curts.tv_sec+curts.tv_usec/1000000.0)-(startts.tv_sec+startts.tv_usec/1000000.0));

        if (bVerbose) {
            printf("\n##################################################################\n");
        }
    }


    for (i=0; i<numOfSockets; i++) {
        printf("Total L3 misses for Socket %d = %lu.\n", i, totalL3CacheMissesEnd[i]-totalL3CacheMissesStart[i]);
    }

//    gettimeofday(&curts, NULL);
//    printf("TIME=%f\n", (curts.tv_sec+curts.tv_usec/1000000.0)-(startts.tv_sec+startts.tv_usec/1000000.0));

}
