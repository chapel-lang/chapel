/**
 * RCR logger implementation. 
 * By David O'Brien based on code by Min Yeol Lim 
 *
 * Includes some code based on the UDP server example from the Linux Gazette
 *   (http://www.linuxgazette.com/node/8758)
 * Uses some code based on the libpfm4 examples from Perfmon2
 *   (http://perfmon2.sourceforge.net/)
 */


#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <err.h>
#include <fcntl.h>

#include "bcGen.h"
#include "common_rcrtool.h"

#define LATENCYTEST 10000

#define TITLE "RCR Daemon (RCRdaemon) 1.00"

// Maximum number of processors
#define MAX_PROCESSORS 256

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
#define ROOTDIR "/sys/kernel/debug/RCRTool"

/*
typedef enum _rcrtool_meter_type {
    RCR_TYPE_IMMEDIATE = 0,
    RCR_TYPE_AVERAGE,
    RCR_TYPE_MAXIMUM,
    RCR_TYPE_SUM
} rcrtool_meter_type;
*/

// Maximum time window in RCRDaemon (in nanosecond)
// Now, 60 seconds
// Increasing this may affect to overall overheads in memory size and latency
// Got to be carefully selected
#define MAX_TIME_WINDOW 60000000000

//#define INTEL_NEHALEM
#define AMD_OPTERON


/////////////////////////////////////////////////////////////////////////////////////
// Define the relevant metrics and the performance counters required to calculate them
/////////////////////////////////////////////////////////////////////////////////////
#ifdef INTEL_NEHALEM
// Initialize the even list for Bill machine
const int numOfMetersPerCore = 3;
// Currently, the uncore counter is not supported in Intel Nehalem chip
const int numOfMetersPerSocket = 0;
const char *meterspercore[] = {"CPI", "L2MissCycleRatio", "L3MissCycleRatio"};
const char *meterspersocket[] = {"MemoryBandwidth"};

const char *eventsPerCore[] = {"PERF_COUNT_HW_CPU_CYCLES,PERF_COUNT_HW_INSTRUCTIONS",
    "PERF_COUNT_HW_CPU_CYCLES,MEM_LOAD_RETIRED:LLC_UNSHARED_HIT,MEM_LOAD_RETIRED:OTHER_CORE_L2_HIT_HITM",
    "PERF_COUNT_HW_CPU_CYCLES,MEM_LOAD_RETIRED:LLC_MISS"};
const int eventsNumPerCore[] = {2, 3, 2};

const char *eventsPerSocket[] = {"UNC_LLC_LINES_IN:ANY"};
const int eventsNumPerSocket[] = {1};

#elif defined(AMD_OPTERON)
const int numOfMetersPerCore = 2;
//int numOfMetersPerSocket = 3;
const int numOfMetersPerSocket = 4;
const int numOfMetersPerNode = 0;

const char *metersPerCore[] = {"CPI", "L2MissRatio"};
const char *metersPerSocket[] = {"L3MissRatio", "MemoryBandwidth", "MemoryConcurrency", "MemoryLatency"};
//char *meterspersocket[] = {"L3MissRatio", "MemoryBandwidth", "MemoryConcurrency"};
const char *metersPerNode[] = {};

const char *eventsPerCore[] = {"PERF_COUNT_HW_CPU_CYCLES,PERF_COUNT_HW_INSTRUCTIONS",
    "PERF_COUNT_HW_INSTRUCTIONS,REQUESTS_TO_L2:ALL,L2_CACHE_MISS:ALL,L2_FILL_WRITEBACK:ALL"};
const int eventsNumPerCore[] = {2, 4};

const char *eventsPerSocket[] = {"PERF_COUNT_HW_INSTRUCTIONS,READ_REQUEST_TO_L3_CACHE:ALL,L3_CACHE_MISSES:ALL",
    "PERF_COUNT_HW_INSTRUCTIONS,DRAM_ACCESSES_PAGE:ALL",
    "CPU_CLK_UNHALTED,L3_CACHE_MISSES:ALL,CPU_READ_COMMAND_REQUESTS_TO_TARGET_NODE_0_3:ALL",
    "CPU_CLK_UNHALTED,CPU_READ_COMMAND_LATENCY_TO_TARGET_NODE_0_3:ALL,CPU_READ_COMMAND_REQUESTS_TO_TARGET_NODE_0_3:ALL"};
//int eventsNumPerSocket[] = {4, 2, 3};
const int eventsNumPerSocket[] = {4, 2, 3, 3};
#endif
/////////////////////////////////////////////////////////////////////////////////////

// Daemon configuration variables
char bDaemon = 0, bVerbose = 0, bOutputToNull = 1;
static unsigned long bFrequency = 0;

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
 * Emit some help info to the console.  The program will then exit.
 *
 * \param progName Name of this program, i.e. argv[0]
 */
void usage(char* progName) {
    printf(
          TITLE "\n"
          "  By David O'Brien\n"
          "\n"
          "Syntax:\n"
          "  %s [-d [-O]] [-v]\n"
          "\n"
          "Options:\n"
          "  -d  Daemon mode; fork a daemon process to run in background\n"
          "  -O  When daemonizing, don't redirect stdout/stderr to null\n"
          "  -v  Verbose; output every step of the way.\n"
          "\n"
          "Parameters:\n",
          progName);
    exit(0);
}

typedef struct _CPUINFO {
    int nodeID;
    int socketID;
    int coreID;
} CPUINFO;

static CPUINFO cpu_mapping_table[MAX_PROCESSORS];

typedef struct _SOCKETINFO {
    int nodeID;
    int numOfProcessors;
    int processorIDs[MAX_PROCESSORS];
} SOCKETINFO;

static SOCKETINFO socket_mapping_table[MAX_PROCESSORS];

// Hardware configurations
static int numOfProcessors;
static int numOfNodes;
static int numOfSockets;
static int numOfCoresPerSocket;
//static int numOfCacheLevels = 3;
// Hyperthreading enabled?
static int HTenabled;
static int cacheLineSize = 64;

/**
 * Read the hardware configuration and allocate the proper memory for counters.
 * Initialize variables.
 *
 * @return int
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
    int   i, j, k, first;

    if (bVerbose) printf("Initializing RCRdaemon.\n");

    for (i = 0; i < numOfSockets; i++)
        socket_mapping_table[i].numOfProcessors = 0;

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

    if (numOfProcessors == (numOfSockets * numOfCoresPerSocket))
        HTenabled = 0;
    else {
        HTenabled = 1;

        for (i = 0; i < numOfSockets; i++) {
            for (j = 0; j < numOfCoresPerSocket; j++) {
                first = 1;
                for (k = 0; k < numOfProcessors; k++) {
                    if ((first == 1) && (cpu_mapping_table[k].socketID == i) && (cpu_mapping_table[k].coreID == j)) {
                        first = 0;
                        continue;
                    }
                    if ((first == 0) && (cpu_mapping_table[k].socketID == i) && (cpu_mapping_table[k].coreID == j)) {
                        cpu_mapping_table[k].coreID = j + numOfCoresPerSocket;
                    }
                }
            }
        }
    }

    if (bVerbose) {
        printf("Nodes = %d  Sockets = %d  Processors = %d  CoresPreSocket = %d\n", numOfNodes, numOfSockets, numOfProcessors, numOfCoresPerSocket);
        for (k = 0; k < numOfProcessors; k++) {
            printf("[%d, %d, %d]\n", cpu_mapping_table[k].nodeID, cpu_mapping_table[k].socketID, cpu_mapping_table[k].coreID);
        }
    }
    return(0);
}

/**
 * 
 * 
 * @param processorID 
 * @param str 
 * 
 * @return int 
 */
int getNodeDirPath(int processorID, char *str) {

    sprintf(str, "%s/Node_%04d", ROOTDIR, cpu_mapping_table[processorID].nodeID);

    return 0;
}


/**
 * 
 * 
 * @param processorID 
 * @param str 
 * 
 * @return int 
 */
int getSocketDirPath(int processorID, char *str) {

    sprintf(str, "%s/Node_%04d/Socket_%04d", ROOTDIR, cpu_mapping_table[processorID].nodeID, cpu_mapping_table[processorID].socketID);

    return 0;
}


int getCoreDirPath(int processorID, char *str) {

    sprintf(str, "%s/Node_%04d/Socket_%04d/Core_%04d", ROOTDIR, cpu_mapping_table[processorID].nodeID, cpu_mapping_table[processorID].socketID, cpu_mapping_table[processorID].coreID);

    return 0;
}


/**
 * 
 * 
 * @param processorID 
 * @param meterNum 
 * 
 * @return int 
 */
int getCoreMeterState(int processorID, int meterNum) {
    char filename[256];
    uint64_t buf1, buf2;
    char str[256];

    if (meterNum < 0 && meterNum >= numOfMetersPerCore)
        return 0;

    sprintf(filename, "%s/enable", ROOTDIR);

    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf1);
    fclose(fp);

    getCoreDirPath(processorID, str);
    sprintf(filename, "%s/%s/enable", str, metersPerCore[meterNum]);

    fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf2);
    fclose(fp);

    return buf1 && buf2;
}


/**
 * 
 * 
 * @param processorID 
 * @param meterNum 
 * 
 * @return uint64_t 
 */
uint64_t getCoreMeterInterval(int processorID, int meterNum) {
    char filename[256];
    uint64_t buf;
    char str[256];

    if (meterNum < 0 && meterNum >= numOfMetersPerCore)
        return 0;

    getCoreDirPath(processorID, str);
    sprintf(filename, "%s/%s/interval", str, metersPerCore[meterNum]);

    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf);
    fclose(fp);

    // In order to obtain valid event count numbers,
    // a minimum monitoring interval should be specified
    // By default, it is 1 millisecond.
//    if (buf == 0)
//        buf = 1000;

    return buf;
}


/**
 * 
 * 
 * @param processorID 
 * @param meterNum 
 * 
 * @return uint64_t 
 */
uint64_t getCoreMeterTimeWindow(int processorID, int meterNum) {
    char filename[256];
    uint64_t buf;
    char str[256];

    if (meterNum < 0 && meterNum >= numOfMetersPerCore)
        return 0;

    getCoreDirPath(processorID, str);
    sprintf(filename, "%s/%s/timewindow", str, metersPerCore[meterNum]);

    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf);
    fclose(fp);

    return buf;
}


/**
 * 
 * 
 * @param processorID 
 * @param meterNum 
 * @param type 
 * 
 * @return uint64_t 
 */
uint64_t getCoreMeter(int processorID, int meterNum, RCR_type meterType) {
    char filename[256];
    uint64_t buf;
    char str[256];

    if (meterNum < 0 || meterNum >= numOfMetersPerCore)
        return 0;

    getCoreDirPath(processorID, str);

    switch (meterType) {
    case RCR_TYPE_IMMEDIATE:
        sprintf(filename, "%s/%s/current", str, metersPerCore[meterNum]);
        break;
    case RCR_TYPE_AVERAGE:
        sprintf(filename, "%s/%s/average", str, metersPerCore[meterNum]);
        break;
    case RCR_TYPE_SUM:
        break;
    case RCR_TYPE_MAXIMUM:
        sprintf(filename, "%s/%s/maximum", str, metersPerCore[meterNum]);
        break;
    }

    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf);

    fclose(fp);

    return buf;
}

/**
 * No node meters really exist, so this is a dummy function for now. Some
 * composite calculation could be done on a per node basis in the future so am
 * including this for now.
 * 
 * @param nodeID
 * @param meterNum
 * @param type
 * 
 * @return uint64_t
 */
uint64_t getNodeMeter(int nodeID, int meterNum, RCR_type meterType) {
    char filename[256];
    uint64_t buf;
    char str[256];

    if (meterNum < 0 || meterNum >= numOfMetersPerNode)
        return 0;

    getNodeDirPath(nodeID, str);

    switch (meterType) {
    case RCR_TYPE_IMMEDIATE:
        sprintf(filename, "%s/%s/current", str, metersPerCore[meterNum]);
        break;
    case RCR_TYPE_AVERAGE:
        sprintf(filename, "%s/%s/average", str, metersPerCore[meterNum]);
        break;
    case RCR_TYPE_SUM:
        break;
    case RCR_TYPE_MAXIMUM:
        sprintf(filename, "%s/%s/maximum", str, metersPerCore[meterNum]);
        break;
    }

    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf);

    fclose(fp);

    return buf;
}

/**
 * 
 * 
 * @param processorID 
 * @param meterNum 
 * 
 * @return int 
 */
int getSocketMeterState(int processorID, int meterNum) {
    char filename[256];
    uint64_t buf1, buf2;
    char str[256];

    if (meterNum < 0 && meterNum >= numOfMetersPerSocket)
        return 0;

    sprintf(filename, "%s/enable", ROOTDIR);

    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf1);
    fclose(fp);


    getSocketDirPath(processorID, str);
    sprintf(filename, "%s/%s/enable", str, metersPerSocket[meterNum]);

    fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf2);
    fclose(fp);

    return buf1 && buf2;
}


/**
 * 
 * 
 * @param processorID 
 * @param meterNum 
 * 
 * @return uint64_t 
 */
uint64_t getSocketMeterInterval(int processorID, int meterNum) {
    char filename[256];
    uint64_t buf;
    char str[256];

    if (meterNum < 0 && meterNum >= numOfMetersPerSocket)
        return 0;

    getSocketDirPath(processorID, str);
    sprintf(filename, "%s/%s/interval", str, metersPerSocket[meterNum]);

    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf);
    fclose(fp);

    // In order to obtain valid event count numbers,
    // a minimum monitoring interval should be specified
    // By default, it is 1 millisecond.
//    if (buf == 0)
//        buf = 1000;

    return buf;
}


/**
 * 
 * 
 * @param processorID 
 * @param meterNum 
 * 
 * @return uint64_t 
 */
uint64_t getSocketMeterTimeWindow(int processorID, int meterNum) {
    char filename[256];
    uint64_t buf;
    char str[256];

    if (meterNum < 0 && meterNum >= numOfMetersPerSocket)
        return 0;

    getSocketDirPath(processorID, str);
    sprintf(filename, "%s/%s/timewindow", str, metersPerSocket[meterNum]);

    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf);
    fclose(fp);

    return buf;
}

/**
 * 
 * 
 * @param processorID 
 * @param meterNum 
 * @param MeterType 
 * 
 * @return uint64_t 
 */
uint64_t getSocketMeter(int processorID, int meterNum, RCR_type MeterType) {
    char filename[256];
    uint64_t buf;
    char str[256];

    if (meterNum < 0 || meterNum >= numOfMetersPerSocket)
        return 0;

    getSocketDirPath(processorID, str);

    switch (MeterType) {
    case RCR_TYPE_IMMEDIATE:
        sprintf(filename, "%s/%s/current", str, metersPerSocket[meterNum]);
        break;
    case RCR_TYPE_AVERAGE:
        sprintf(filename, "%s/%s/average", str, metersPerSocket[meterNum]);
        break;
    case RCR_TYPE_SUM:
        break;
    case RCR_TYPE_MAXIMUM:
        sprintf(filename, "%s/%s/maximum", str, metersPerSocket[meterNum]);
        break;
    }

    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf);

    fclose(fp);

    return buf;
}

/**
 * 
 * 
 * @param processorID 
 * @param meterNum 
 * @param type 
 * @param value 
 * 
 * @return int 
 */
/*
int setSocketMeter(int processorID, int meterNum, int type, uint64_t value) {
    char filename[256];
    char str[256];

    if (meterNum < 0 && meterNum >= numOfMetersPerSocket)
        return -1;

    getSocketDirPath(processorID, str);

    switch (type) {
    case RCR_TYPE_IMMEDIATE:
        sprintf(filename, "%s/%s/current", str, metersPerSocket[meterNum]);
        break;
    case RCR_TYPE_AVERAGE:
        sprintf(filename, "%s/%s/average", str, metersPerSocket[meterNum]);
        break;
    case RCR_TYPE_SUM:
        break;
    case RCR_TYPE_MAXIMUM:
        sprintf(filename, "%s/%s/maximum", str, metersPerSocket[meterNum]);
        break;
    }

    FILE* fp = fopen(filename, "w");
    if (!fp) return -1;

    value = 

    fprintf(fp,"%llu\n", value);

    fclose(fp);

    return 0;
}
*/

/**
 * 
 * 
 * @return uint64_t 
 */
uint64_t getResetValue() {
    char filename[256];
    uint64_t buf;

    sprintf(filename, "%s/reset", ROOTDIR);

    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    fscanf(fp,"%llu", &buf);

    fclose(fp);

    return buf;
}


/**
 * 
 * 
 * @return int 
 */
int setResetValue() {
    char filename[256];

    sprintf(filename, "%s/reset", ROOTDIR);

    FILE* fp = fopen(filename, "w");
    if (!fp) return -1;

    fprintf(fp,"%llu\n", 0);

    fclose(fp);

    return 0;
}


/**
 * Perform the actual work.
 */
void doLoggingWork(void) {

    initSystemConfiguration();

    Trigger** triggerMap = getTriggers();
    int numTriggers      = getNumTriggers();

    int status, ret, i, j, ncpus, cpu, meternum, sock, num, triggerNum;
    struct timespec interval, freq, remainder;

    double currentval, maxval, avgval;
    uint64_t currentVal_ll;
    double currentVal_d;

    struct MeterValue **core_meter_list;
    struct MeterValue **socket_meter_list;

    interval.tv_sec = MONITORINTERVALINSEC;
    interval.tv_nsec = MONITORINTERVALINNANOSEC;

    freq.tv_nsec = bFrequency % 1000000000;
    freq.tv_sec = (bFrequency - (bFrequency % 1000000000)) / 1000000000;

    struct timeval startts;
    struct timeval curts;

    gettimeofday(&startts, NULL);

    int enabledcnt = 0;

    //Write out the header and some other housekeeping
    char headerBuffer[1024];
    printf("Time");
    for (triggerNum = 0; triggerNum < numTriggers; triggerNum++) {
        triggerMap[triggerNum]->meterNum = 0; //Set a default
        switch (triggerMap[triggerNum]->meterType) {
        case TYPE_CORE:
            printf("\t%s_%s_%04i", triggerMap[triggerNum]->meterName, "Core", triggerMap[triggerNum]->id);
            for (meternum = 0; meternum < numOfMetersPerCore; meternum++) {
                if (strncmp(triggerMap[triggerNum]->meterName, metersPerCore[meternum], strlen(metersPerCore[meternum])) == 0) {
                    triggerMap[triggerNum]->meterNum = meternum;
                    break;
                }
            }
            break;
        case TYPE_SOCKET:
            printf("\t%s_%s_%04i", triggerMap[triggerNum]->meterName, "Socket", triggerMap[triggerNum]->id);
            for (meternum = 0; meternum < numOfMetersPerSocket; meternum++) {
                if (strncmp(triggerMap[triggerNum]->meterName, metersPerSocket[meternum], strlen(metersPerSocket[meternum])) == 0) {
                    triggerMap[triggerNum]->meterNum = meternum;
                    break;
                }
            }
            break;
        case TYPE_NODE:
            printf("\t%s_%s_%04i", triggerMap[triggerNum]->meterName, "Node", triggerMap[triggerNum]->id);
            for (meternum = 0; meternum < numOfMetersPerNode; meternum++) {
                if (strncmp(triggerMap[triggerNum]->meterName, metersPerNode[meternum], strlen(metersPerNode[meternum])) == 0) {
                    triggerMap[triggerNum]->meterNum = meternum;
                    break;
                }
            }
            break;
        }
    }
    printf("\n");

    gettimeofday(&startts, NULL);
    char appStateBuf[1024];

    int logAppState = 0;
    while (1) {

        logAppState = 0;
        gettimeofday(&curts, NULL);
        printf("%f", (curts.tv_sec+curts.tv_usec/1000000.0)-(startts.tv_sec+startts.tv_usec/1000000.0));

        for (triggerNum = 0; triggerNum < numTriggers; triggerNum++) {
            switch (triggerMap[triggerNum]->meterType) {
            case TYPE_CORE:
                currentVal_ll = getCoreMeter(triggerMap[triggerNum]->id, triggerMap[triggerNum]->meterNum, RCR_TYPE_IMMEDIATE);
                break;
            case TYPE_SOCKET:
                currentVal_ll = getSocketMeter(triggerMap[triggerNum]->id, triggerMap[triggerNum]->meterNum, RCR_TYPE_IMMEDIATE);
                break;
            case TYPE_NODE:
                currentVal_ll = getNodeMeter(triggerMap[triggerNum]->id, triggerMap[triggerNum]->meterNum, RCR_TYPE_IMMEDIATE);
                break;
            }
            currentVal_d = currentVal_ll / 1000000.0;
            printf("\t%f", currentVal_d);

            logAppState = shmGet(triggerMap[triggerNum]->flagShmKey) == '1';
        }
        //Check for application state
        if (logAppState) {
            char *shmDest = getShmStringLoc(triggerMap[0]->appStateShmKey, 64);
            if (shmDest) {
                strncpy(appStateBuf, shmDest, 64);
                printf("\t%s\n", shmDest);
                releaseShmLoc(shmDest);
            }
        } else {
            //printf("\t0\n");
            printf("\t\n");
        }

        //printf("Before loop sleep\n");
        if (freq.tv_sec != 0 || freq.tv_nsec != 0)
            nanosleep(&freq, &remainder);
        //printf("After loop sleep\n");

    }

}

/**
 * Daemon entry point.
 *  
 * @param argc
 * 
 * @param argv
 * 
 * @return
 * 
 */
int main(int argc, char** argv) {

    // Program name (i.e. argv[0])
    char* progName;

    progName = argv[0]; // Note our program name
    argc--; argv++; // ...and get rid of it

    // Get all cmdline args
    while (argc >= 1 && argv[0][0]=='-') {
        switch (argv[0][1]) {
        case 'i': 
            bFrequency = atoi(&(argv[0][2])); 
            break;
        case 'd': 
            bDaemon = 1; 
            break;
        case 'O':
            bOutputToNull = 0;
            break;
        default:
            usage(progName); // Invalid argument, give the help message and exit
        }

        argc--; argv++; // Shift cmdline arguments to get rid of this option we just processed
    }

    // Check for port number on command line
    if (argc >= 1) {
        usage(progName);
    }

    // Emit banner with options listing
//  printf(
//        TITLE "\n"
//        "\n"
//        "Options:\n"
//        "  Verbose?      : %s\n"
//        "  Daemon?       : %s\n"
//        "  OutputToNull? : %s  (daemon mode only)\n"
//        "\n",
//        tf(bVerbose),
//        tf(bDaemon),
//        tf(bOutputToNull));

    // Become a daemon if we should
    if (bDaemon) {
        printf("Forking daemon...\n");
        daemonize(bOutputToNull);
    }

    //printf("Up and running!\n");
    // Build the triggerMap for breadcrumbs insertion
    // FIXME: hard-coded fileName for trigger info
    buildTriggerMap("/hpc_shared/home/dobrien/work/RCRTool/RCRdaemon/triggers.config");

    doLoggingWork();

    // We never get here, but who cares
    return EXIT_SUCCESS;
}


