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

extern int gethostname(char *name, size_t namelen);

static char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i, j;                           // j acts as an interator for
                                      // pvm_addhosts and pvm_delhosts
                                      // if this node (running launcher) is
                                      // in hostfile, adding or deleting it
                                      // can result in error

  int info;
  int infos[256];
  char myhostname[256];
  char pvmnodetoadd[256];
  char* pvmnodestoadd[2048];
  int pvmsize = 0;
  int signal = 0;
  char* commandtopvm;
  char* environment;
  static char *hosts2[] = {};
  int numt;
  int tids[32];
  static char *argtostart[] = {};
  int bufid;

  // These are for receiving singals from slaves
  char buffer[PRINTF_BUFF_LEN];
  char description[PRINTF_BUFF_LEN];  // gdb specific
  int ignorestatus;                   // gdb specific
  int who;                            // gdb specific

  char** argv2;
  char* numlocstr;

  // Add nodes to PVM configuration.
  FILE* nodelistfile;

  char* hostfile;

  // Get a new argument list for PVM spawn.
  // The last argument needs to be the number of locations for the PVM
  // comm layer to use it. The comm layer strips this off.
  argv2 = chpl_malloc(((argc+1) * sizeof(char *)), sizeof(char*), CHPL_RT_MD_PVM_SPAWN_THING, -1, "");
  for (i=0; i < (argc-1); i++) {
    argv2[i] = argv[i+1];
  }
  sprintf(numlocstr, "%d", numLocales);
  argv2[argc-1] = numlocstr;
  argv2[argc] = NULL;

  // Add nodes to PVM configuration.
  hostfile = chpl_malloc(1024, sizeof(char*), CHPL_RT_MD_PVM_LIST_OF_NODES, -1, "");
  sprintf(hostfile, "%s%s", getenv((char *)"CHPL_HOME"), "/hostfile");

  if ((nodelistfile = fopen(hostfile, "r")) == NULL) {
    fprintf(stderr, "Make sure %s is present and readable.\n", hostfile);
    chpl_free(hostfile, -1, "");
    chpl_free(argv2, -1, "");
    chpl_internal_error("Exiting.");
  }
  chpl_free(hostfile, -1, "");
  i = 0;
  while (((fscanf(nodelistfile, "%s", pvmnodetoadd)) == 1) && (i < numLocales)) {
    pvmnodestoadd[i] = chpl_malloc((strlen(pvmnodetoadd)+1), sizeof(char *), CHPL_RT_MD_PVM_LIST_OF_NODES, -1, "");
    strcpy(pvmnodestoadd[i], pvmnodetoadd);
    i++;
  }
  if (i < numLocales) {
    fclose(nodelistfile);
    while (i >= 0) {
      chpl_free(pvmnodestoadd[i], -1, "");
      i--;
    }
    chpl_free(argv2, -1, "");
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

  // Building the hosts2 list to pass to pvm_addhosts and pvm_deletehosts
  gethostname(myhostname, 256);
  for (i = 0, j = 0; j < numLocales; ) {
    if (strcmp((char *)pvmnodestoadd[j], myhostname)) {
      hosts2[i] = (char *)pvmnodestoadd[j];
      i++;
      j++;
    } else {
      j++;
    }
  }

  if (i != 0) {
    info = pvm_addhosts( (char **)hosts2, i, infos );
  }
  j = i;

  pvmsize += strlen(getenv((char *)"PWD")) + strlen("/_real") + strlen(argv[0]);

  commandtopvm = chpl_malloc(pvmsize, sizeof(char*), CHPL_RT_MD_PVM_SPAWN_THING, -1, "");
  *commandtopvm = '\0';
  environment = chpl_malloc(1024, sizeof(char*), CHPL_RT_MD_PVM_SPAWN_THING, -1, "");
  *environment = '\0';
  getcwd(environment, 1024);
  strcat(commandtopvm, environment);
  chpl_free(environment, -1, "");
  strcat(commandtopvm, "/");
  strcat(commandtopvm, argv[0]);
  strcat(commandtopvm, "_real");
  if ((nodelistfile = fopen(commandtopvm, "r")) == NULL) {
    chpl_free(commandtopvm, -1, "");
    pvmsize = strlen(argv[0]) + strlen("_real");
    commandtopvm = chpl_malloc(pvmsize, sizeof(char*), CHPL_RT_MD_PVM_SPAWN_THING, -1, "");
    *commandtopvm = '\0';
    strcat(commandtopvm, argv[0]);
    strcat(commandtopvm, "_real");
    if ((nodelistfile = fopen(commandtopvm, "r")) == NULL) {
      fprintf(stderr, "Make sure %s is present and readable.\n", commandtopvm);
      i = 0;
      if (j != 0) {
        info = pvm_delhosts( (char **)hosts2, j, infos );
      }
      while (i < numLocales) {
        chpl_free(pvmnodestoadd[i], -1, "");
        i++;
      }
      chpl_free(commandtopvm, -1, "");
      chpl_free(environment, -1, "");
      chpl_free(argv2, -1, "");
      chpl_internal_error("Unknown executable.");
    }
  }
  fclose(nodelistfile);

  for (i = 0; i < numLocales; i++) {
    numt = pvm_spawn( (char *)commandtopvm, argv2, 1, (char *)pvmnodestoadd[i], 1, &tids[i] );
    if (numt == 0) {
      if (j != 0) {
        info = pvm_delhosts( (char **)hosts2, j, infos );
      }
      i = 0;
      while (i < numLocales) {
        chpl_free(pvmnodestoadd[i], -1, "");
        i++;
      }
      chpl_free(commandtopvm, -1, "");
      chpl_free(environment, -1, "");
      chpl_free(argv2, -1, "");
      chpl_internal_error("Trouble spawning slave.");
    }
  }

  info = pvm_mytid();

  while (signal == 0) {
    bufid = pvm_recv(-1, NOTIFYTAG);
    pvm_upkint(&signal, 1, 1);
    // fprintf case
    // TODO: Right now, it only goes to stderr
    if (signal == 2) {
      pvm_upkstr(buffer);
      fprintf(stderr, "%s", buffer);
      fflush(stderr);
      signal = 0;
    }
    // printf case
    if (signal == 3) {
      pvm_upkstr(buffer);
      printf("%s", buffer);
      fflush(stderr);
      signal = 0;
    }
    // Run in gdb mode
    if (signal == 4) {
      pvm_upkint(&who, 1, 1);
      pvm_upkstr(buffer);
      pvm_upkstr(description);
      pvm_upkint(&ignorestatus, 1, 1);
      info = system(buffer);
      pvm_initsend(PvmDataDefault);
      pvm_pkint(&info, 1, 1);
      pvm_send(who, NOTIFYTAG);
      if (info == -1) {
        chpl_error("system() fork failed", 0, "(command-line)");
      } else if (info != 0 && !ignorestatus) {
        chpl_error(description, 0, "(command-line)");
      }
    }
  }

  if (j != 0) {
    info = pvm_delhosts( (char **)hosts2, j, infos );
  }

  while (i >= 0) {
    chpl_free(pvmnodestoadd[i], -1, "");
    i--;
  }
  chpl_free(argv2, -1, "");
  chpl_free(commandtopvm, -1, "");
  chpl_free(environment, -1, "");

  return (char *)"killall -9 pvmd3 && rm -rf /tmp/*pvm*";
}

void chpl_launch(int argc, char* argv[], int32_t numLocales) {
  chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                           argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  return 0;
}
