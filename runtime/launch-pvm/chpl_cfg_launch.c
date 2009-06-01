#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "pvm3.h"

#include "chplrt.h"
#include "chplcomm.h"
#include "chpl_mem.h"
#include "chplsys.h"
#include "chplthreads.h"
#include "error.h"

#include "chpllaunch.h"
/*#include "chplmem.h"
#include "error.h"
*/
#define NOTIFYTAG 4194295
#define PRINTF_BUFF_LEN 1024

extern int pvm_addhosts(char **hosts, int nhost, int *infos);

static char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i;

  int info;
  int infos[256];
  char pvmnodetoadd[256];
  char* pvmnodestoadd[2048];
  int pvmsize = 0;
  int signal = 0;
  char* commandtopvm;
  static char *hosts2[] = {};
  int numt;
  int tids[32];
  static char *argtostart[] = {};
  int bufid;
  char buffer[PRINTF_BUFF_LEN];

  char* argv2[] = { (char *)argv[1], (char *)argv[2], (char *)"", };

  // Add nodes to PVM configuration.
  FILE* nodelistfile;

  char* hostfile;
  hostfile = chpl_malloc(1024, sizeof(char*), "PVM hostfile", -1, "");
  sprintf(hostfile, "%s%s", getenv((char *)"CHPL_HOME"), "/hostfile");

  if ((nodelistfile = fopen(hostfile, "r")) == NULL) {
    fprintf(stderr, "Make sure %s is present and readable.\n", hostfile);
    chpl_free(hostfile, -1, "");
    chpl_internal_error("Exiting.");
  }
  chpl_free(hostfile, -1, "");
  i = 0;
  while (((fscanf(nodelistfile, "%s", pvmnodetoadd)) == 1) && (i < numLocales)) {
    pvmnodestoadd[i] = chpl_malloc((strlen(pvmnodetoadd)+1), sizeof(char *), "create a list of nodes", -1, "");
    strcpy(pvmnodestoadd[i], pvmnodetoadd);
    i++;
  }
  if (i < numLocales) {
    fclose(nodelistfile);
    while (i >= 0) {
      chpl_free(pvmnodestoadd[i], -1, "");
      i--;
    }
    chpl_internal_error("Number of locales specified is greater than what's known in PVM hostfile.");
  }
  fclose(nodelistfile);

  info = pvm_start_pvmd(0, argtostart, 1);
  if (info != 0) {
    if (info == PvmDupHost) {
      fprintf(stderr, "Duplicate host. Shutting down host.\n");
      pvm_halt();
    }
    fprintf(stderr, "Exiting.\n");
    chpl_internal_error("Problem starting PVM daemon.");
  }

  for (i = 0; i < numLocales; i++) {
    hosts2[i] = (char *)pvmnodestoadd[i];
  }

  info = pvm_addhosts( (char **)hosts2, i, infos );

  pvmsize += strlen(getenv((char *)"CHPL_HOME")) + strlen("/_real") + strlen(argv[0]);

  commandtopvm = chpl_malloc(pvmsize, sizeof(char*), "thing to execute via PVM spawn", -1, "");
  *commandtopvm = '\0';
  strcat(commandtopvm, getenv((char *)"CHPL_HOME"));
  strcat(commandtopvm, "/");
  strcat(commandtopvm, argv[0]);
  strcat(commandtopvm, "_real");

  for (i = 0; i < numLocales; i++) {
    numt = pvm_spawn( (char *)commandtopvm, argv2, 1, (char *)pvmnodestoadd[i], 1, &tids[i] );
    if (numt == 0) {
      info = pvm_delhosts( (char **)hosts2, numLocales, infos );
      i = 0;
      while (i < numLocales) {
        chpl_free(pvmnodestoadd[i], -1, "");
        i++;
      }
      chpl_free(commandtopvm, -1, "");
      chpl_internal_error("Trouble spawning slave.");
    }
  }

  info = pvm_mytid();

  while (signal == 0) {
    bufid = pvm_recv(-1, NOTIFYTAG);
    pvm_upkint(&signal, 1, 1);
    if (signal == 2) {
      pvm_upkstr(buffer);
      fprintf(stderr, "%s", buffer);
      fflush(stderr);
      signal = 0;
    }
    if (signal == 3) {
      pvm_upkstr(buffer);
      printf("%s", buffer);
      fflush(stderr);
      signal = 0;
    }
  }

  info = pvm_delhosts( (char **)hosts2, i, infos );

  while (i >= 0) {
    chpl_free(pvmnodestoadd[i], -1, "");
    i--;
  }
  chpl_free(commandtopvm, -1, "");

  pvm_halt();
  return (char *)"";
}

void chpl_launch(int argc, char* argv[], int32_t numLocales) {
  chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                           argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  return 0;
}
