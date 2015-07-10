/*
 * Copyright 2015 Cray Inc.
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
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int chpl_vdebug_fd = -1;
int chpl_vdebug = 0;

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

static int chpl_make_vdebug_file (const char *rootname) {
    char fname[2048]; // fixed size ..
    struct stat sb;

    chpl_vdebug = 0;
    chpl_vdebug_fd = -1;

    // Make sure the directory is made.
    if (stat(rootname, &sb) < 0) {
      if (errno == ENOENT) {
        if (mkdir(rootname,0777) < 0 && (errno != EEXIST)) {
          fprintf (stderr, "Can not make Visual Debug directory %s.\n", rootname);
          return -1;
        }
      } else {
        fprintf (stderr, "Can not make Visual Debug directory %s.\n", rootname);
        return -1;
      }
    } else {
      if ((sb.st_mode & S_IFMT) != S_IFDIR) {
        fprintf (stderr, "%s: not a directory.\n", rootname);
        return -1;
      }
    }
    
    snprintf (fname, 2048, "%s/%s-%d", rootname, rootname, chpl_nodeID);
    chpl_vdebug_fd = open (fname, O_WRONLY|O_CREAT|O_TRUNC|O_APPEND, 0666);
    if (chpl_vdebug_fd < 0) {
      fprintf (stderr, "Visual Debug failed to open %s: %s\n",
               fname, strerror (errno));
      chpl_vdebug_fd = -1;
      return -1;
    }
    chpl_vdebug = 1;
    return 0;
}

void chpl_vdebug_nolog () {
  chpl_vdebug = 0;
}

void chpl_vdebug_start (const char *fileroot, double now) {
  const char * rootname;
  struct rusage ru;
  struct timeval tv;
  struct timezone tz = {0,0};
  (void)gettimeofday(&tv, &tz);

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
                "ChplVdebug: nodes %d id %d seq %.3lf %lld.%06ld %ld.%06ld %ld.%06ld \n",
                chpl_numNodes, chpl_nodeID, now,
                (long long) tv.tv_sec, (long) tv.tv_usec,
                (long)ru.ru_utime.tv_sec, (long)ru.ru_utime.tv_usec,
                (long)ru.ru_stime.tv_sec, (long)ru.ru_stime.tv_usec  );
  chpl_vdebug = 1;
}

void chpl_vdebug_stop (void) {
  struct rusage ru;  
  struct timeval tv;
  struct timezone tz = {0,0};
  if (chpl_vdebug_fd >= 0) {
    (void)gettimeofday(&tv, &tz);
    if ( getrusage (RUSAGE_SELF, &ru) < 0) {
      ru.ru_utime.tv_sec = 0;
      ru.ru_utime.tv_usec = 0;
      ru.ru_stime.tv_sec = 0;
      ru.ru_stime.tv_usec = 0;
    }
    // Generate the End record
    chpl_dprintf (chpl_vdebug_fd, "End: %lld.%06ld %ld.%06ld %ld.%06ld %d\n",
                  (long long) tv.tv_sec, (long) tv.tv_usec,
                  (long)ru.ru_utime.tv_sec, (long)ru.ru_utime.tv_usec,
                  (long)ru.ru_stime.tv_sec, (long)ru.ru_stime.tv_usec,
                  chpl_nodeID);
    close (chpl_vdebug_fd);
  }
  chpl_vdebug = 0;
}

static int tag_no = 0;  // A unique tag number for sorting tags ...
void chpl_vdebug_tag (const char *str)
{
  struct rusage ru;
  struct timeval tv;
  struct timezone tz = {0,0};

  (void)gettimeofday(&tv, &tz);
  if ( getrusage (RUSAGE_SELF, &ru) < 0) {
    ru.ru_utime.tv_sec = 0;
    ru.ru_utime.tv_usec = 0;
    ru.ru_stime.tv_sec = 0;
    ru.ru_stime.tv_usec = 0;
  }
  chpl_dprintf (chpl_vdebug_fd, "Tag: %lld.%06ld %ld.%06ld %ld.%06ld %d %d %s\n",
                (long long) tv.tv_sec, (long) tv.tv_usec,
                (long)ru.ru_utime.tv_sec, (long)ru.ru_utime.tv_usec,
                (long)ru.ru_stime.tv_sec, (long)ru.ru_stime.tv_usec,
                chpl_nodeID, tag_no++, str);
  chpl_vdebug = 1;
}

void chpl_vdebug_pause (void) {
  struct rusage ru;
  struct timeval tv;
  struct timezone tz = {0,0};
  if (chpl_vdebug_fd >=0 && chpl_vdebug == 0) {
    (void)gettimeofday(&tv, &tz);
    if ( getrusage (RUSAGE_SELF, &ru) < 0) {
      ru.ru_utime.tv_sec = 0;
      ru.ru_utime.tv_usec = 0;
      ru.ru_stime.tv_sec = 0;
      ru.ru_stime.tv_usec = 0;
    }
    chpl_dprintf (chpl_vdebug_fd, "Pause: %lld.%06ld %ld.%06ld %ld.%06ld %d %d\n",
                  (long long) tv.tv_sec, (long) tv.tv_usec,
                  (long)ru.ru_utime.tv_sec, (long)ru.ru_utime.tv_usec,
                  (long)ru.ru_stime.tv_sec, (long)ru.ru_stime.tv_usec,
                  chpl_nodeID, tag_no-1);
    chpl_vdebug = 0;
  }
}

// End Visual Debug Support
