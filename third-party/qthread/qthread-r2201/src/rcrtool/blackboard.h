#ifndef __BLACKBOARD_H__
#define __BLACKBOARD_H__

#ifdef __cplusplus
//extern "C" {
#endif

#include <sys/shm.h>
#include "common_rcrtool.h"

#define BLACKBOARDSHMKEY       8234
#define APPSTATESHMKEY         8235
#define METERTEXTFIELDLENGTH   64
struct RCRMeter;
struct _RCRBlackboard;
struct RCRNode;
struct RCRSocket;
struct RCRCore;
struct RCR;

typedef struct RCRMeter*              RCRMeter_v;
typedef struct RCRNode*               RCRNode_v;
typedef struct RCRSocket*             RCRSocket_v;
typedef struct RCRCore*               RCRCore_v;

// RCRTool meter types
typedef enum _RCRMonitorLevel {
    RCR_MON_NONE = 0,
    RCR_MON_METERS,
    RCR_MON_ALL,
} RCRMonitorLevel;

// RCRTool meter types
typedef enum _RCRLogginLevel {
    RCR_LOG_NONE = 0,
    RCR_LOG_ALL,
} RCRLogginLevel;



struct RCRMeter {
    int           enable;                          // 0 or 1 
    unsigned long interval;                        // time in microsecs between samples. 
    unsigned long window;                          // in microsecs. 
    double        average;                         // average value over time window. 
    double        current;                         // latest sample value of meter. 
    double        maximum;                         // max sampled value over window.
    double        uBound;                          // upper bound of accepted values. 
    double        lBound;                          // lower bound of accepted values. 
    char          meterName[METERTEXTFIELDLENGTH+1]; // text name of meter.
    char          xAxisName[METERTEXTFIELDLENGTH+1]; // text name of x-axis.
    char          yAxisName[METERTEXTFIELDLENGTH+1]; // text name of y-axis.
};

struct RCRNode {
    int             enable;           // 0 or 1 
    int             nodeID;           // node number
    int             numOfSockets;     // Number of sockets on this node
    int             sockets_i;        // Array of sockets on this node.
    struct _RCRBlackboard* parentBlackboard; // Parent blackboard
//  RCRBoardShared  sharedResources;  //For shared resources, such as IO or network
};

struct RCRSocket {
    int             enable;          // 0 or 1 
    int             numOfCores;      // Number of cores on this socket
    int             numOfMeters;     // Number of meters on this socket
    int             socketID;        // Sequential number of socket accross all nodes
//  RCRCore_v       cores;           // Array of cores on this socket
    int             cores_i;         // Array of cores on this socket
    struct RCRNode*        parentNode;      // Parent node
//  RCRMeter_v      socketMeters;    // array of socket level meters
    int             socketMeters_i;  // array of socket level meters
//  RCRSocketShared sharedResources; // for socket share resources, such as shared L3 or DIMMs
};

struct RCRCore {
    int        enable;       // 0 or 1 
    int        numOfMeters;  // Number of meters on this core
    int        coreID;       // Sequential number of core accross all sockets as found in /proc/cpuinfo
    struct RCRSocket* parentSocket; // Parent socket
//  RCRMeter_v coreMeters;   // array of core level meters
    int        coreMeters_i; // array of core level meters
};

/*!
 * 
 * 
 * \return RCRBlackboard* 
 */
struct _RCRBlackboard* getShmBlackboard(int mode);

struct _RCRBlackboard {
    int             enable;               // 0 or 1 
    size_t          shmSize;              // size of this shared memory blackboard 
    RCRMonitorLevel mLevel;               // Level of monitoring.
    RCRLogginLevel  lLevel;               // Type or Level of meter logging.
    int             numOfNodes;           // Number of nodes on system
    int             numOfSockets;         // Number of nodes on system
    int             numOfCores;           // Number of nodes on system
    int             numOfMetersPerCore;   // Number of meters per core 
    int             numOfMetersPerSocket; // Number of meters per socket
    RCRNode_v       nodes;                // Array of nodes on the system
    unsigned long   socketMeterInterval;  // time in microsecs between samples. 
    unsigned long   coreMeterInterval;    // time in microsecs between samples. 
};

#ifdef __cplusplus
//}
#endif

#endif
