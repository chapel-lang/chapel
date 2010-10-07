#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "chpllaunch.h"
#include "chpl_mem.h"
#include "error.h"

// from ../gasnetrun_ibv/launch-gasnetrun_ibv.c:
#define WRAP_TO_STR(x) TO_STR(x)
#define TO_STR(x) #x

//
// we will build this command:
//   bsub ... LAUNCH_PATH/gasnetrun_ibv ... REAL_BINNAME ...
//
// bsub options used:
//  -R "span[ptile=1]" seems to request just 1 process per node
//  -n NN              request NN processors (i.e. nodes?)
//  -Ip                requests an interactive session with a pseudo-terminal
//
// bsub other options:
//  -K  wait for job completion - could use instead of -Ip,
//      but stdin/out/err are disconnected (use -i/-o/-e to direct to files)
//  there does not seem to be a "quiet" option
//

#define BSUB_BASE       "bsub -Ip -R \"span[ptile=1]\" -n %d"
#define GASNETRUN_PATH  WRAP_TO_STR(LAUNCH_PATH)
#define GASNETRUN_BASE  "gasnetrun_ibv -n %d"

#define GRAND_BASE \
  BSUB_BASE \
  " '" GASNETRUN_PATH "'" GASNETRUN_BASE \
  " '%s'"

static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char* command;

  char* real_binname;
  int nlLen = 5; // safe estimate

  chpl_compute_real_binary_name(argv[0]);
  real_binname = chpl_get_real_binary_name();

  size = sizeof(GRAND_BASE) + 2 * nlLen + strlen(real_binname) + 1;
  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }
  command = chpl_malloc(size, sizeof(char*), CHPL_RT_MD_COMMAND_BUFFER, -1, "");

  sprintf(command, GRAND_BASE, numLocales, numLocales, real_binname);
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


void chpl_launch(int argc, char* argv[], int32_t numLocales) {
  chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                           argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  return 0;
}
