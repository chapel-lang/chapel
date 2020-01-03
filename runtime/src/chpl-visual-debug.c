/*
 * Copyright 2004-2020 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Visual Debug Support file
//

#include "chpl-visual-debug.h"
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-tasks.h"
#include "chpl-tasks-callbacks.h"
#include "chpl-comm-callbacks.h"
#include "chpl-linefile-support.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/param.h>

#include "chplcgfns.h"

extern c_nodeid_t chpl_nodeID; // unique ID for each node: 0, 1, 2, 

int install_callbacks (void);
int uninstall_callbacks (void);

static void cb_task_create (const chpl_task_cb_info_t *info);
static void cb_task_begin (const chpl_task_cb_info_t *info);
static void cb_task_end (const chpl_task_cb_info_t *info);
static void cb_comm_put_nb(const chpl_comm_cb_info_t *info);
static void cb_comm_get_nb(const chpl_comm_cb_info_t *info);
static void cb_comm_put(const chpl_comm_cb_info_t *info);
static void cb_comm_get(const chpl_comm_cb_info_t *info);
static void cb_comm_put_strd(const chpl_comm_cb_info_t *info);
static void cb_comm_get_strd(const chpl_comm_cb_info_t *info);
static void cb_comm_executeOn(const chpl_comm_cb_info_t *info);
static void cb_comm_executeOn_nb(const chpl_comm_cb_info_t *info);
static void cb_comm_executeOn_fast(const chpl_comm_cb_info_t *info);

int chpl_vdebug_fd = -1;
int chpl_vdebug = 0;

#define TID_STRING(buff, tid) (chpl_task_idToString(buff, CHPL_TASK_ID_STRING_MAX_LEN, tid))

#define VDEBUG_GETPUT_FORMAT_NAMES "kind tv srcNodeID dstNodeID commTaskID addr raddr elemSize length commID lineNumber fileno"
#define VDEBUG_GETPUT_FORMAT_STRING "%s: %lld.%06ld %d %d %s %#lx %#lx %zd %zd %d %d %d\n"

int chpl_dprintf (int fd, const char * format, ...) {
  char buffer[2048]; 
  va_list ap;
  int wrv;
  int retval;

  va_start (ap, format);
  retval = vsnprintf (buffer, sizeof (buffer), format, ap);
  va_end(ap);
  if (retval > 0) {
    wrv = write (fd, buffer,retval);
    if (wrv < 0) return -1;
    return retval;
  }
  return -1;
}

static int chpl_make_vdebug_file (const char *rootname) {
    char fname[MAXPATHLEN]; 
    struct stat sb;

    chpl_vdebug = 0;
    chpl_vdebug_fd = -1;

    // Make sure the directory is made.
    if (mkdir (rootname,0777) < 0) {
      if (stat (rootname, &sb) < 0) {
        fprintf (stderr, "Can not make Visual Debug directory %s.\n", rootname);
        return -1;
      }
      if (!S_ISDIR(sb.st_mode)) {
        fprintf (stderr, "%s: not a directory.\n", rootname);
        return -1;
      }
    }
    
    snprintf (fname, sizeof (fname), "%s/%s-%d", rootname, rootname, chpl_nodeID);
    chpl_vdebug_fd = open (fname, O_WRONLY|O_CREAT|O_TRUNC|O_APPEND, 0666);
    if (chpl_vdebug_fd < 0) {
      fprintf (stderr, "Visual Debug failed to open %s: %s\n",
               fname, strerror (errno));
      chpl_vdebug_fd = -1;
      return -1;
    }

    return 0;
}

// Record>  ChplVdebug: ver # nid # tid # seq time.sec user.time system.time 
//
//  Ver # -- version number, currently 1.1
//  nid # -- nodeID
//  tid # -- taskID
//  seq time.sec -- unique number for this run

void chpl_vdebug_start (const char *fileroot, double now) {
  const char * rootname;
  struct rusage ru;
  struct timeval tv;
  chpl_taskID_t startTask = chpl_task_getId();
  char buff[CHPL_TASK_ID_STRING_MAX_LEN];
  (void) gettimeofday (&tv, NULL);

  install_callbacks();

  chpl_vdebug = 0;

  // Close any open files.
  if (chpl_vdebug_fd >= 0)
    chpl_vdebug_stop ();
    
  // Initial call, open file and write initialization information
  
  // Get the root of the file name.
  rootname = (fileroot == NULL || fileroot[0] == 0) ? ".Vdebug" : fileroot; 
  
  // In case of an error, just return
  if (chpl_make_vdebug_file (rootname) < 0)
    return;
  
  // Write initial information to the file, including resource time
  if ( getrusage (RUSAGE_SELF, &ru) < 0) {
    ru.ru_utime.tv_sec = 0;
    ru.ru_utime.tv_usec = 0;
    ru.ru_stime.tv_sec = 0;
    ru.ru_stime.tv_usec = 0;
  }
  chpl_dprintf (chpl_vdebug_fd,
                "ChplVdebug: ver 1.4 nodes %d nid %d tid %s seq %.3lf %lld.%06ld %ld.%06ld %ld.%06ld \n",
                chpl_numNodes, chpl_nodeID, TID_STRING(buff, startTask), now,
                (long long) tv.tv_sec, (long) tv.tv_usec,
                (long) ru.ru_utime.tv_sec, (long) ru.ru_utime.tv_usec,
                (long) ru.ru_stime.tv_sec, (long) ru.ru_stime.tv_usec  );

  // Dump directory names, file names and function names
  if (chpl_nodeID == 0) {
    int ix;
    int numFIDnames;

    chpl_dprintf (chpl_vdebug_fd, "CHPL_HOME: %s\n", CHPL_HOME);
    chpl_dprintf (chpl_vdebug_fd, "DIR: %s\n", chpl_compileDirectory);
    chpl_dprintf (chpl_vdebug_fd, "SAVEC: %s\n", chpl_saveCDir);

    chpl_dprintf (chpl_vdebug_fd, "Tablesize: %d\n", chpl_filenameTableSize);
    for (ix = 0; ix < chpl_filenameTableSize ; ix++) {
      if (chpl_filenameTable[ix][0] == 0) {
        chpl_dprintf (chpl_vdebug_fd, "fname: 0 <unknown>\n");
      } else if (chpl_filenameTable[ix][0] == '<' &&
                 chpl_filenameTable[ix][1] == 'c') {
        chpl_dprintf (chpl_vdebug_fd, "fname: %d <command_line>\n", ix);
      } else {
        chpl_dprintf (chpl_vdebug_fd, "fname: %d %s\n", ix,
                      chpl_filenameTable[ix]);
      }
    }
    for (numFIDnames = 0; chpl_finfo[numFIDnames].name != NULL; numFIDnames++);
    chpl_dprintf (chpl_vdebug_fd, "FIDNsize: %d\n", numFIDnames);
    for (ix = 0; ix < numFIDnames; ix++)
      chpl_dprintf (chpl_vdebug_fd, "FIDname: %d %d %d %s\n", ix,
                    chpl_finfo[ix].lineno, chpl_finfo[ix].fileno,
                    chpl_finfo[ix].name);
  }
  
  chpl_vdebug = 1;
}

// Record>  End: time.sec user.time system.time nodeID taskID
//
// Should be the last record in the file.

void chpl_vdebug_stop (void) {
  struct rusage ru;  
  struct timeval tv;
  chpl_taskID_t stopTask = chpl_task_getId();
  char buff[CHPL_TASK_ID_STRING_MAX_LEN];

  // First, shutdown VisualDebug
  chpl_vdebug = 0;
  uninstall_callbacks();

  // Now log the stop
  if (chpl_vdebug_fd >= 0) {
    (void) gettimeofday (&tv, NULL);
    if ( getrusage (RUSAGE_SELF, &ru) < 0) {
      ru.ru_utime.tv_sec = 0;
      ru.ru_utime.tv_usec = 0;
      ru.ru_stime.tv_sec = 0;
      ru.ru_stime.tv_usec = 0;
    }
    // Generate the End record
    chpl_dprintf (chpl_vdebug_fd, "End: %lld.%06ld %ld.%06ld %ld.%06ld %d %s\n",
                  (long long) tv.tv_sec, (long) tv.tv_usec,
                  (long) ru.ru_utime.tv_sec, (long) ru.ru_utime.tv_usec,
                  (long) ru.ru_stime.tv_sec, (long) ru.ru_stime.tv_usec,
                  chpl_nodeID, TID_STRING(buff, stopTask));
    close (chpl_vdebug_fd);
  }
}

// Record>  VdbMark: time.sec nodeId taskId
//
// This marks taskID as being a xxxVdebug() call.   Any executeOns or tasks
// started by this task and descendants of this task are related to
// the xxxVdebug() call and chplvis should ignore them.

void chpl_vdebug_mark (void) {
  struct timeval tv;
  chpl_taskID_t tagTask = chpl_task_getId();
  char buff[CHPL_TASK_ID_STRING_MAX_LEN];
  (void) gettimeofday (&tv, NULL);
  chpl_dprintf (chpl_vdebug_fd, "VdbMark: %lld.%06ld %d %s\n",
                (long long) tv.tv_sec, (long) tv.tv_usec, chpl_nodeID, TID_STRING(buff, tagTask) );
}

// Record>  tname: tag# tagname

void chpl_vdebug_tagname (const char* tagname, int tagno) {
  chpl_dprintf (chpl_vdebug_fd, "tname: %d %s\n", tagno, tagname);
}

// Record>  Tag: time.sec user.time sys.time nodeId taskId tag# 

void chpl_vdebug_tag (int tagno) {
  struct rusage ru;
  struct timeval tv;
  
  chpl_taskID_t tagTask = chpl_task_getId();
  char buff[CHPL_TASK_ID_STRING_MAX_LEN];

  (void) gettimeofday (&tv, NULL);
  if ( getrusage (RUSAGE_SELF, &ru) < 0) {
    ru.ru_utime.tv_sec = 0;
    ru.ru_utime.tv_usec = 0;
    ru.ru_stime.tv_sec = 0;
    ru.ru_stime.tv_usec = 0;
  }
  chpl_dprintf (chpl_vdebug_fd, "Tag: %lld.%06ld %ld.%06ld %ld.%06ld %d %s %d\n",
                (long long) tv.tv_sec, (long) tv.tv_usec,
                (long) ru.ru_utime.tv_sec, (long) ru.ru_utime.tv_usec,
                (long) ru.ru_stime.tv_sec, (long) ru.ru_stime.tv_usec,
                chpl_nodeID, TID_STRING(buff, tagTask), tagno);
  chpl_vdebug = 1;
}

// Record>  Pause: time.sec user.time sys.time nodeId taskId tag#

void chpl_vdebug_pause (int tagno) {
  struct rusage ru;
  struct timeval tv;
  chpl_taskID_t pauseTask = chpl_task_getId();
  char buff[CHPL_TASK_ID_STRING_MAX_LEN];

  if (chpl_vdebug_fd >=0 && chpl_vdebug == 1) {
    (void) gettimeofday (&tv, NULL);
    if ( getrusage (RUSAGE_SELF, &ru) < 0) {
      ru.ru_utime.tv_sec = 0;
      ru.ru_utime.tv_usec = 0;
      ru.ru_stime.tv_sec = 0;
      ru.ru_stime.tv_usec = 0;
    }
    chpl_dprintf (chpl_vdebug_fd, "Pause: %lld.%06ld %ld.%06ld %ld.%06ld %d %s %d\n",
                  (long long) tv.tv_sec, (long) tv.tv_usec,
                  (long) ru.ru_utime.tv_sec, (long) ru.ru_utime.tv_usec,
                  (long) ru.ru_stime.tv_sec, (long) ru.ru_stime.tv_usec,
                  chpl_nodeID, TID_STRING(buff, pauseTask), tagno);
    chpl_vdebug = 0;
  }
}

// Routines to log data ... put here so other places can
// just call this code to get things logged.
// FIXME: the size argument (size_t) is being cast to int here. chplvis needs
//        to be updated to take in size_t. The elemsize field is no longer
//        relevant as well.

// Record>  nb_put: time.sec srcNodeId dstNodeId commTaskId addr raddr elemsize 
//                  length lineNumber fileName
//

void cb_comm_put_nb (const chpl_comm_cb_info_t *info) {
  if (chpl_vdebug) {
    struct timeval tv;
    const struct chpl_comm_info_comm *cm = &info->iu.comm;
    chpl_taskID_t commTask = chpl_task_getId();
    char buff[CHPL_TASK_ID_STRING_MAX_LEN];
    (void) gettimeofday (&tv, NULL);
    chpl_dprintf (chpl_vdebug_fd, 
                  VDEBUG_GETPUT_FORMAT_STRING, "nb_put",
                  (long long) tv.tv_sec, (long) tv.tv_usec,  info->localNodeID,
                  info->remoteNodeID, TID_STRING(buff, commTask), (unsigned long) cm->addr,
                  (unsigned long) cm->raddr, (size_t)1, cm->size,
                  cm->commID, cm->lineno, cm->filename);
  }
}

// Record>  nb_get: time.sec dstNodeId srcNodeId commTaskId addr raddr elemsize 
//                  length lineNumber fileName
//
// Note: dstNodeId is node requesting Get

void cb_comm_get_nb (const chpl_comm_cb_info_t *info) {
  if (chpl_vdebug) {
    struct timeval tv;
    const struct chpl_comm_info_comm *cm = &info->iu.comm;
    chpl_taskID_t commTask = chpl_task_getId();
    char buff[CHPL_TASK_ID_STRING_MAX_LEN];
    (void) gettimeofday (&tv, NULL);
    chpl_dprintf (chpl_vdebug_fd,
                  VDEBUG_GETPUT_FORMAT_STRING, "nb_get",
                  (long long) tv.tv_sec, (long) tv.tv_usec,  info->localNodeID,
                  info->remoteNodeID, TID_STRING(buff, commTask), (unsigned long) cm->addr,
                  (unsigned long) cm->raddr, (size_t)1, cm->size,
                  cm->commID, cm->lineno, cm->filename);
  }
}

// Record>  put: time.sec srcNodeId dstNodeId commTaskId addr raddr elemsize 
//               length lineNumber fileName


void cb_comm_put (const chpl_comm_cb_info_t *info) {
  if (chpl_vdebug) {
    struct timeval tv;
    const struct chpl_comm_info_comm *cm = &info->iu.comm;
    chpl_taskID_t commTask = chpl_task_getId();
    char buff[CHPL_TASK_ID_STRING_MAX_LEN];
    (void) gettimeofday (&tv, NULL);
    chpl_dprintf (chpl_vdebug_fd,
                  VDEBUG_GETPUT_FORMAT_STRING, "put",
                  (long long) tv.tv_sec, (long) tv.tv_usec, info->localNodeID,
                  info->remoteNodeID, TID_STRING(buff, commTask), (unsigned long) cm->addr,
                  (unsigned long) cm->raddr, (size_t)1, cm->size,
                  cm->commID, cm->lineno, cm->filename);
  }
}

// Record>  get: time.sec dstNodeId srcNodeId commTaskId addr raddr elemsize 
//               length lineNumber fileName
//
// Note:  dstNodeId is for the node making the request

void cb_comm_get (const chpl_comm_cb_info_t *info) {
  if (chpl_vdebug) {
    struct timeval tv;
    const struct chpl_comm_info_comm *cm = &info->iu.comm;
    chpl_taskID_t commTask = chpl_task_getId();
    char buff[CHPL_TASK_ID_STRING_MAX_LEN];
    (void) gettimeofday (&tv, NULL);
    chpl_dprintf (chpl_vdebug_fd,
                  VDEBUG_GETPUT_FORMAT_STRING, "get",
                  (long long) tv.tv_sec, (long) tv.tv_usec,  info->localNodeID,
                  info->remoteNodeID, TID_STRING(buff, commTask), (unsigned long) cm->addr,
                  (unsigned long) cm->raddr, (size_t)1, cm->size,
                  cm->commID, cm->lineno, cm->filename);
  }
}

// Record>  st_put: time.sec srcNodeId dstNodeId commTaskId addr raddr elemsize 
//                  length lineNumber fileName

void cb_comm_put_strd (const chpl_comm_cb_info_t *info) {
    if (chpl_vdebug) {
    struct timeval tv;
    size_t length;
    const struct chpl_comm_info_comm_strd *cm = &info->iu.comm_strd;
    chpl_taskID_t commTask = chpl_task_getId();
    char buff[CHPL_TASK_ID_STRING_MAX_LEN];
    (void) gettimeofday (&tv, NULL);

    length = 1;
    for (int32_t i = 0; i < cm->stridelevels; i++) {
      length *= cm->count[i];
    }

    chpl_dprintf (chpl_vdebug_fd,
                  VDEBUG_GETPUT_FORMAT_STRING, "st_put",
                  (long long) tv.tv_sec, (long) tv.tv_usec,  info->localNodeID, 
                  info->remoteNodeID, TID_STRING(buff, commTask),
                  (unsigned long) cm->srcaddr, (unsigned long) cm->dstaddr, cm->elemSize,
                  length, cm->commID, cm->lineno, cm->filename);
    // printout srcstrides and dststrides and stridelevels and count?
  }

}

// Record>  st_get: time.sec dstNodeId srcNodeId commTaskId addr raddr elemsize 
//                  length lineNumber fileName
//
// Note:  dstNode is node making request for get

void cb_comm_get_strd (const chpl_comm_cb_info_t *info) {
  if (chpl_vdebug) {
    struct timeval tv;
    size_t length;
    const struct chpl_comm_info_comm_strd *cm = &info->iu.comm_strd;
    chpl_taskID_t commTask = chpl_task_getId();
    char buff[CHPL_TASK_ID_STRING_MAX_LEN];
    (void) gettimeofday (&tv, NULL);

    length = 1;
    for (int32_t i = 0; i < cm->stridelevels; i++) {
      length *= cm->count[i];
    }

    chpl_dprintf (chpl_vdebug_fd,
                  VDEBUG_GETPUT_FORMAT_STRING, "st_get",
                  (long long) tv.tv_sec, (long) tv.tv_usec, info->localNodeID,
                  info->remoteNodeID, TID_STRING(buff, commTask),
                  (unsigned long) cm->dstaddr, (unsigned long) cm->srcaddr, cm->elemSize,
                  length, cm->commID, cm->lineno, cm->filename);
    // print out the srcstrides and dststrides and stridelevels and count?
  }
}

// Record>  fork: time.sec nodeId forkNodeId subLoc funcId arg argSize forkTaskId lineNumber fileName

void cb_comm_executeOn (const chpl_comm_cb_info_t *info) {

  // Visual Debug Support
  if (chpl_vdebug) {
    const struct chpl_comm_info_comm_executeOn *cm = &info->iu.executeOn;
    chpl_taskID_t executeOnTask = chpl_task_getId();
    char buff[CHPL_TASK_ID_STRING_MAX_LEN];
    struct timeval tv;
    (void) gettimeofday (&tv, NULL);
    chpl_dprintf (chpl_vdebug_fd,
                  "fork: %lld.%06ld %d %d %d %d %#lx %zd %s %d %d\n",
                  (long long) tv.tv_sec, (long) tv.tv_usec, info->localNodeID,
                  info->remoteNodeID, cm->subloc, cm->fid, (unsigned long) cm->arg,
                  cm->arg_size, TID_STRING(buff, executeOnTask),
                  cm->lineno, cm->filename);
  }
}

// Record>  fork_nb: time.sec nodeId forkNodeId subLoc funcId arg argSize forkTaskId lineNumber fileName


void  cb_comm_executeOn_nb (const chpl_comm_cb_info_t *info) {
  if (chpl_vdebug) {
    const struct chpl_comm_info_comm_executeOn *cm = &info->iu.executeOn;
    chpl_taskID_t executeOnTask = chpl_task_getId();
    char buff[CHPL_TASK_ID_STRING_MAX_LEN];
    struct timeval tv;
    (void) gettimeofday (&tv, NULL);
    chpl_dprintf (chpl_vdebug_fd, "fork_nb: %lld.%06ld %d %d %d %d %#lx %zd %s %d %d\n",
                  (long long) tv.tv_sec, (long) tv.tv_usec, info->localNodeID,
                  info->remoteNodeID, cm->subloc, cm->fid, (unsigned long) cm->arg, 
                  cm->arg_size, TID_STRING(buff, executeOnTask),
                  cm->lineno, cm->filename);
  }
}

// Record>  f_fork: time.sec nodeId forkNodeId subLoc funcId arg argSize forkTaskId lineNumber fileName

void cb_comm_executeOn_fast (const chpl_comm_cb_info_t *info) {
  if (chpl_vdebug) {
    const struct chpl_comm_info_comm_executeOn *cm = &info->iu.executeOn;
    chpl_taskID_t executeOnTask = chpl_task_getId();
    char buff[CHPL_TASK_ID_STRING_MAX_LEN];
    struct timeval tv;
    (void) gettimeofday (&tv, NULL);
    chpl_dprintf (chpl_vdebug_fd,
                  "f_fork: %lld.%06ld %d %d %d %d %#lx %zd %s %d %d\n",
                  (long long) tv.tv_sec, (long) tv.tv_usec, info->localNodeID,
                  info->remoteNodeID, cm->subloc, cm->fid, (unsigned long)cm->arg, 
                  cm->arg_size, TID_STRING(buff, executeOnTask),
                  cm->lineno, cm->filename);
  }
}


// Task layer callbacks

int install_callbacks (void) {
  if (chpl_task_install_callback(chpl_task_cb_event_kind_create, 
                                 chpl_task_cb_info_kind_full, cb_task_create) != 0)
    return 1;
  if (chpl_task_install_callback(chpl_task_cb_event_kind_begin, 
                                 chpl_task_cb_info_kind_full, cb_task_begin) != 0) {
    (void) uninstall_callbacks();
    return 1;
  }
  if (chpl_task_install_callback(chpl_task_cb_event_kind_end,
                                 chpl_task_cb_info_kind_id_only, cb_task_end) != 0) {
    (void) uninstall_callbacks();
    return 1;
  }
  if (chpl_comm_install_callback(chpl_comm_cb_event_kind_put_nb,
                                 cb_comm_put_nb)) {
    (void) uninstall_callbacks();
    return 1;
  }
  if (chpl_comm_install_callback(chpl_comm_cb_event_kind_get_nb,
                                 cb_comm_get_nb)) {
    (void) uninstall_callbacks();
    return 1;
  }
  if (chpl_comm_install_callback(chpl_comm_cb_event_kind_put,
                                 cb_comm_put)) {
    (void) uninstall_callbacks();
    return 1;
  }
  if (chpl_comm_install_callback(chpl_comm_cb_event_kind_get,
                                 cb_comm_get)) {
    (void) uninstall_callbacks();
    return 1;
  }
   if (chpl_comm_install_callback(chpl_comm_cb_event_kind_put_strd,
                                 cb_comm_put_strd)) {
    (void) uninstall_callbacks();
    return 1;
  }
  if (chpl_comm_install_callback(chpl_comm_cb_event_kind_get_strd,
                                 cb_comm_get_strd)) {
    (void) uninstall_callbacks();
    return 1;
  }
  if (chpl_comm_install_callback(chpl_comm_cb_event_kind_executeOn,
                                 cb_comm_executeOn)) {
    (void) uninstall_callbacks();
    return 1;
  }
  if (chpl_comm_install_callback(chpl_comm_cb_event_kind_executeOn_nb,
                                 cb_comm_executeOn_nb)) {
    (void) uninstall_callbacks();
    return 1;
  }
  if (chpl_comm_install_callback(chpl_comm_cb_event_kind_executeOn_fast,
                                 cb_comm_executeOn_fast)) {
    (void) uninstall_callbacks();
    return 1;
  }

  return 0;
}


int uninstall_callbacks (void) {
  int rv = 0;
  rv  = chpl_task_uninstall_callback(chpl_task_cb_event_kind_create,
                                     cb_task_create);
  rv += chpl_task_uninstall_callback(chpl_task_cb_event_kind_begin,
                                     cb_task_begin);
  rv += chpl_task_uninstall_callback(chpl_task_cb_event_kind_end,
                                     cb_task_end);
  rv += chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_put_nb,
                                     cb_comm_put_nb);
  rv += chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_get_nb,
                                     cb_comm_get_nb);
  rv += chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_put,
                                     cb_comm_put);
  rv += chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_get,
                                     cb_comm_get);
  rv += chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_put_strd,
                                     cb_comm_put_strd);
  rv += chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_get_strd,
                                     cb_comm_get_strd);
  rv += chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_executeOn,
                                     cb_comm_executeOn);
  rv += chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_executeOn_nb,
                                     cb_comm_executeOn_nb);
  rv += chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_executeOn_fast,
                                     cb_comm_executeOn_fast);
  return rv;
}

// Record>  task: time.sec nodeId taskId parentTaskId On/Local lineNum srcName fid

void cb_task_create (const chpl_task_cb_info_t *info) {
  struct timeval tv;
  if (!chpl_vdebug) return;
  if (chpl_vdebug_fd >= 0) {
    chpl_taskID_t taskId = chpl_task_getId();
    char buff[CHPL_TASK_ID_STRING_MAX_LEN];
    //printf ("taskCB: event: %d, node %d proc %s task id: %llu, new task id: %llu\n",
    //         (int)info->event_kind, (int)info->nodeID,
    //        (info->iu.full.is_executeOn ? "O" : "L"), taskId, info->iu.full.id);
    (void)gettimeofday(&tv, NULL);
    chpl_dprintf (chpl_vdebug_fd, "task: %lld.%06ld %lld %ld %s %s %ld %d %d\n",
                  (long long) tv.tv_sec, (long) tv.tv_usec,
                  (long long) info->nodeID, (long int) info->iu.full.id,
                  TID_STRING(buff, taskId),
                  (info->iu.full.is_executeOn ? "O" : "L"),
                  (long int) info->iu.full.lineno,
                  info->iu.full.filename,
                  info->iu.full.fid);
   }
}

// Record>  Btask: time.sec nodeId taskId

void cb_task_begin (const chpl_task_cb_info_t *info) {
  struct timeval tv;
  if (!chpl_vdebug) return;
  if (chpl_vdebug_fd >= 0) {
    (void)gettimeofday(&tv, NULL);
    chpl_dprintf (chpl_vdebug_fd, "Btask: %lld.%06ld %lld %lu\n",
                  (long long) tv.tv_sec, (long) tv.tv_usec,
                  (long long) info->nodeID, (unsigned long) info->iu.full.id);
 
  }
}

// Record>  Etask: time.sec nodeId taskId

void cb_task_end (const chpl_task_cb_info_t *info) {
  struct timeval tv;
  if (!chpl_vdebug) return;
  if (chpl_vdebug_fd >= 0) {
    (void)gettimeofday(&tv, NULL);
    chpl_dprintf (chpl_vdebug_fd, "Etask: %lld.%06ld %lld %lu\n",
                  (long long) tv.tv_sec, (long) tv.tv_usec,
                  (long long) info->nodeID, (unsigned long) info->iu.id_only.id);
 
  }
}
