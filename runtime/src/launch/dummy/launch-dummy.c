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

#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "error.h"


static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  if (numLocales != 1) {
    chpl_error("dummy launcher only supports numLocales==1", 0, "<command-line>");
  }

  chpl_compute_real_binary_name(argv[0]);

  sprintf(baseCommand, "%s", chpl_get_real_binary_name());

  size = strlen(baseCommand) + 1;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }

  command = chpl_mem_allocMany(size, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  
  sprintf(command, "%s", baseCommand);
  for (i=1; i<argc; i++) {
    strcat(command, " '");
    strcat(command, argv[i]);
    strcat(command, "'");
  }

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  return chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                                  argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, int32_t filename) {
  if (strcmp(argv[argNum], "--dummy") == 0) {
    printf("the dummy launcher is handling an extra argument: %s\n", 
           argv[argNum]);
    return 1;
  } else if (strcmp(argv[argNum], "--queue") == 0) {
    printf("the dummy launcher is handling two extra arguments: %s %s\n",
           argv[argNum], argv[argNum+1]);
    return 2;
  } else {
    return 0;
  }
}


void chpl_launch_print_help(void) {
}
