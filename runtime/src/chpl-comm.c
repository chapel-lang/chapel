/*
 * Copyright 2004-2015 Cray Inc.
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
// Shared code for different comm implementations in
//  comm/<commlayer>/comm-<commlayer>.c
//
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chpl-mem-consistency.h"

#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <sys/resource.h>

int32_t chpl_nodeID = -1;
int32_t chpl_numNodes = -1;

int chpl_verbose_comm;
int chpl_comm_diagnostics;
int chpl_verbose_mem;

// Visual Debug -- should be protected by a #ifdef?

#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>

int chpl_vdebug_fd = -1;
int chpl_vdebug = 0;

// End Visual Debug

void chpl_startCommDiagnostics(void); // this one implemented by comm layers
void chpl_gen_startCommDiagnostics(void); // this one implemented in chpl-comm.c
void chpl_stopCommDiagnostics(void);
void chpl_gen_stopCommDiagnostics(void);
void chpl_startCommDiagnosticsHere(void);
void chpl_gen_startCommDiagnosticsHere(void);
void chpl_stopCommDiagnosticsHere(void);
void chpl_gen_stopCommDiagnosticsHere(void);
void chpl_resetCommDiagnosticsHere(void);
void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd);

void chpl_gen_startCommDiagnostics(void) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(__LINE__, __FILE__);
  // And then start the comm diagnostics as usual.
  chpl_startCommDiagnostics();
}

void chpl_gen_stopCommDiagnostics(void) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(__LINE__, __FILE__);
  // And then stop the comm diagnostics as usual.
  chpl_stopCommDiagnostics();
}

void chpl_gen_startCommDiagnosticsHere(void) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(__LINE__, __FILE__);
  // And then start the comm diagnostics as usual.
  chpl_startCommDiagnosticsHere();
}

void chpl_gen_stopCommDiagnosticsHere(void) {
  // Make sure that there are no pending communication operations.
  chpl_rmem_consist_release(__LINE__, __FILE__);
  // And then stop the comm diagnostics as usual.
  chpl_stopCommDiagnosticsHere();
}


size_t chpl_comm_getenvMaxHeapSize(void)
{
  char*  p;
  static int    env_checked = 0;
  static size_t size = 0;

  if (env_checked)
    return size;

  if ((p = getenv("CHPL_RT_MAX_HEAP_SIZE")) != NULL) {
    //
    // The user specified a maximum size, so start with that.
    //
    int  num_scanned;
    char units;

    if ((num_scanned = sscanf(p, "%zi%c", &size, &units)) != 1) {
      if (num_scanned == 2 && strchr("kKmMgG", units) != NULL) {
        switch (units) {
        case 'k' : case 'K': size <<= 10; break;
        case 'm' : case 'M': size <<= 20; break;
        case 'g' : case 'G': size <<= 30; break;
        }
      }
      else {
        chpl_warning("Cannot parse CHPL_RT_MAX_HEAP_SIZE environment "
                     "variable; assuming 1g", 0, NULL);
        size = ((size_t) 1) << 30;
      }
    }
  }

  env_checked = 1;

  return size;
}

// Visual Debug support

int chpl_dprintf (int fd, const char * format, ...)
{
  char buffer[2048];
  va_list ap;
  int wrv;
  int retval;

  va_start (ap, format);
  retval = vsnprintf (buffer, 2048, format, ap);
  va_end(ap);
  if (retval > 0) {
    wrv = write (fd, buffer,retval);
    if (wrv < 0) return -1;
    return retval;
  }
  return -1;
}

static int chpl_make_vdebug_file (const char *rootname, int namelen) {
    char fname[namelen];
    
    snprintf (fname, namelen, "%s-%d", rootname, chpl_nodeID);
    chpl_vdebug_fd = open (fname, O_WRONLY|O_CREAT|O_TRUNC|O_APPEND, 0666);
    if (chpl_vdebug_fd < 0) {
      fprintf (stderr, "Visual Debug failed to open %s: %s\n",
               fname, strerror (errno));
      chpl_vdebug = 0;
      chpl_vdebug_fd = -1;
      return -1;
    }
    return 0;
}

void chpl_vdebug_start (const char *fileroot, double now) {
  const char * rootname;
  int  namelen;
  int  temp;
  struct rusage ru;

  chpl_vdebug = 0;

  // Close any open files.
  if (chpl_vdebug_fd >= 0)
    chpl_vdebug_stop ();
    
  // Initial call, open file and write initialization information
  
  // Get the root of the file name.
  rootname = (fileroot == NULL || fileroot[0] == 0) ? ".Vdebug" : fileroot; 
  namelen = strlen (rootname)+3;
  temp =  chpl_numNodes;
  while (temp > 1) { 
    namelen ++;
    temp /= 2;
  }
  
  // In case of an error, just return
  if (chpl_make_vdebug_file (rootname, namelen) < 0)
    return;
  
  // Write initial information to the file, including resource time
  if ( getrusage (RUSAGE_SELF, &ru) < 0) {
    ru.ru_utime.tv_sec = 0;
    ru.ru_utime.tv_usec = 0;
    ru.ru_stime.tv_sec = 0;
    ru.ru_stime.tv_usec = 0;
  }
  chpl_dprintf (chpl_vdebug_fd,
		"ChplVdebug: nodes %d id %d seq %.3lf ru %ld.%06ld %ld.%06ld \n",
		chpl_numNodes, chpl_nodeID, now,
		(long)ru.ru_utime.tv_sec, (long)ru.ru_utime.tv_usec,
		(long)ru.ru_stime.tv_sec, (long)ru.ru_stime.tv_usec  );
  chpl_vdebug = 1;
}

void chpl_vdebug_stop (void) {
  struct rusage ru;  
  if (chpl_vdebug_fd >= 0) {
    if ( getrusage (RUSAGE_SELF, &ru) < 0) {
      ru.ru_utime.tv_sec = 0;
      ru.ru_utime.tv_usec = 0;
      ru.ru_stime.tv_sec = 0;
      ru.ru_stime.tv_usec = 0;
    }
    // Generate the End record
    chpl_dprintf (chpl_vdebug_fd, "End: %ld.%06ld %ld.%06ld %d\n",
		  (long)ru.ru_utime.tv_sec, (long)ru.ru_utime.tv_usec,
		  (long)ru.ru_stime.tv_sec, (long)ru.ru_stime.tv_usec,
		  chpl_nodeID);
    close (chpl_vdebug_fd);
  }
  chpl_vdebug = 0;
}

static int do_tag;
void chpl_vdebug_starttag(void)
{
  // Don't record the messages for the tag
  chpl_vdebug = 0;
  do_tag = 1;
}

static int tag_no = 0;  // A unique tag number for sorting tags ...
void chpl_vdebug_tag (const char *str, int pause)
{
  struct rusage ru;

  if (chpl_vdebug || do_tag) {
    if ( getrusage (RUSAGE_SELF, &ru) < 0) {
      ru.ru_utime.tv_sec = 0;
      ru.ru_utime.tv_usec = 0;
      ru.ru_stime.tv_sec = 0;
      ru.ru_stime.tv_usec = 0;
    }
    chpl_dprintf (chpl_vdebug_fd, "Tag: %ld.%06ld %ld.%06ld %d %d %c %s\n",
		  (long)ru.ru_utime.tv_sec, (long)ru.ru_utime.tv_usec,
		  (long)ru.ru_stime.tv_sec, (long)ru.ru_stime.tv_usec,
		  chpl_nodeID, ++tag_no, (pause ? 'p' : 't'), str);

    // Stop collection of data for a pause
    chpl_vdebug = !pause;
    do_tag = 0;
  }
}

void chpl_vdebug_resume (void) {
  struct rusage ru;
  if (chpl_vdebug_fd >=0 && chpl_vdebug == 0) {
    if ( getrusage (RUSAGE_SELF, &ru) < 0) {
      ru.ru_utime.tv_sec = 0;
      ru.ru_utime.tv_usec = 0;
      ru.ru_stime.tv_sec = 0;
      ru.ru_stime.tv_usec = 0;
    }
    chpl_dprintf (chpl_vdebug_fd, "Resume: %ld.%06ld %ld.%06ld %d %d\n",
		  (long)ru.ru_utime.tv_sec, (long)ru.ru_utime.tv_usec,
		  (long)ru.ru_stime.tv_sec, (long)ru.ru_stime.tv_usec,
		  chpl_nodeID, tag_no);
    chpl_vdebug = 1;
  }
}

// End Visual Debug Support
