#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"
#include <stdlib.h>
#include "pvm3.h"

#define NOTIFYTAG 4194295

extern int pvm_addhosts(char **hosts, int nhost, int *infos);

// TODO: Un-hard-code this stuff:
//static const char* pvmrunPath = "/users/ljprokow/Projects/Chapel/Chapel/third-party/pvm/pvm-3.4.6/pvm3/lib/";
static const char* hostfile = "/users/ljprokow/Projects/Chapel/Chapel/third-party/pvm/pvm-3.4.6/pvm3/hostfile";

static char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i;
  char i_str[128];
  /*
  int size;
  int addsize = 0;
  int basesize = 0;
  int deletesize = 0;
  int topvmsize = 0;
  char* addCommand;
  char* baseCommand;
  char* deleteCommand;
  char* topvmCommand;
  char* command;
  */

  int info;
  int infos[256];
  char pvmnodetoadd[256];
  char* pvmnodestoadd[2048];
  int pvmsize = 0;
  int pvmsize2 = 0;
  int end = 0;
  //  char* hosts;
  char* commandtopvm;
  char* argtopvm;
  static char *hosts2[] = {};
  int numt;
  int tids[32];
  static char *argtostart[] = {};
  char* argtostart2[2];
  int bufid;
  int bytes, pvmtype, source;

  // Add nodes to PVM configuration.
  FILE* nodelistfile;
  /*
  char nodetoadd[256];
  char* nodestoadd[2048];
  if ((nodelistfile = fopen(hostfile, "r")) == NULL) {
    fprintf(stderr, "Make sure %s is present and readable.\n", hostfile);
    chpl_internal_error("Exiting.");
  }
  i = 0;
  while (((fscanf(nodelistfile, "%s", nodetoadd)) == 1) && (i < numLocales)) {
    if ((nodestoadd[i] = malloc(strlen(nodetoadd)+1)) == NULL) {
      chpl_internal_error("Memory allocation error.");
    }
    strcpy(nodestoadd[i], nodetoadd);
    addsize += strlen(nodestoadd[i]);
    i++;
  }
  basesize = addsize;
  deletesize = addsize;
  addsize += (i * strlen("add \n")) + strlen("echo \"\"");
  basesize += strlen("echo \"\" | pvm && ") + strlen(pvmrunPath) + (i * strlen("spawn - -> _real   \n")) + (i * strlen(argv[0])) + (i * strlen(argv[1])) + (i * strlen(argv[2])) + (i * strlen(i_str));
  deletesize += (i * strlen("delete \n")) + strlen("echo \"\"");
  topvmsize = strlen(" | pvm") + strlen(pvmrunPath);
  if (i < numLocales) {
    fclose(nodelistfile);
    while (i >= 0) {
      free(nodestoadd[i]);
      i--;
    }
    chpl_internal_error("Number of locales specified is greater than what's known in PVM hostfile.");
  }
  fclose(nodelistfile);

  // Build the command for use in four parts
  // The first part is the nodes to add to the PVM config in addCommand
  // The second part is the spawn command in PVM
  // The third part (built with the first) is the deleteCommand for PVM
  // Finally, this needs to be passed to PVM with a pipe
  i = 0;
  addCommand = chpl_malloc(addsize, sizeof(char*), "first part pvm command buffer", false, -1, "");
  baseCommand = chpl_malloc(basesize, sizeof(char*), "actual command to spawn", false, -1, "");
  deleteCommand = chpl_malloc(deletesize, sizeof(char*), "last part pvm command buffer", false, -1, "");
  topvmCommand = chpl_malloc(topvmsize, sizeof(char*), "send to pvm via pipe command buffer", false, -1, "");
  *addCommand = '\0';
  *baseCommand = '\0';
  *deleteCommand = '\0';
  *topvmCommand = '\0';
  strcat(addCommand, "echo \"");
  strcat(deleteCommand, "echo \"");
  strcat(baseCommand, "echo \"");
  while (i < numLocales) {
    sprintf(i_str, "%d", i);
    strcat(addCommand, "add ");
    strcat(addCommand, nodestoadd[i]);
    strcat(addCommand, "\n");
    strcat(baseCommand, "spawn -");
    strcat(baseCommand, nodestoadd[i]);
    strcat(baseCommand, " -> ");
    strcat(baseCommand, argv[0]);
    strcat(baseCommand, "_real ");
    strcat(baseCommand, argv[1]);
    strcat(baseCommand, " ");
    strcat(baseCommand, argv[2]);
    strcat(baseCommand, " ");
    strcat(baseCommand, i_str);
    strcat(baseCommand, "\n");
    strcat(deleteCommand, "delete ");
    strcat(deleteCommand, nodestoadd[i]);
    strcat(deleteCommand, "\n");
    i++;
  }
  strcat(addCommand, "\"");
  strcat(baseCommand, "\" | ");
  strcat(baseCommand, pvmrunPath);
  strcat(baseCommand, "pvm && ");
  strcat(deleteCommand, "\"");
  strcat(topvmCommand, " | ");
  strcat(topvmCommand, pvmrunPath);
  strcat(topvmCommand, "pvm");

  size = strlen(addCommand) + strlen(topvmCommand) + strlen(" && ") + strlen(deleteCommand) + strlen(baseCommand) + strlen(topvmCommand) + 1;
  command = chpl_malloc(size, sizeof(char*), "pvm command buffer", false, -1, "");
  *command = '\0';

  strcat(command, addCommand);
  strcat(command, topvmCommand);
  strcat(command, " && ");
  strcat(command, baseCommand);
  strcat(command, deleteCommand);
  strcat(command, topvmCommand);

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  fprintf(stderr, "addCommand: %s\n", addCommand);
  fprintf(stderr, "topvmCommand: %s\n", topvmCommand);
  fprintf(stderr, "baseCommand: %s\n", baseCommand);
  fprintf(stderr, "deleteCommand: %s\n", deleteCommand);
  fprintf(stderr, "command: %s\n", command);

  while (i >= 0) {
    free(nodestoadd[i]);
    i--;
  }
  chpl_free(addCommand, false, -1, "");
  chpl_free(deleteCommand, false, -1, "");
  chpl_free(topvmCommand, false, -1, "");

  fprintf(stderr, "\n\n\nStarting again.\n\n\n");
  */
  if ((nodelistfile = fopen(hostfile, "r")) == NULL) {
    fprintf(stderr, "Make sure %s is present and readable.\n", hostfile);
    chpl_internal_error("Exiting.");
  }
  i = 0;
  while (((fscanf(nodelistfile, "%s", pvmnodetoadd)) == 1) && (i < numLocales)) {
    if ((pvmnodestoadd[i] = malloc(strlen(pvmnodetoadd)+1)) == NULL) {
      chpl_internal_error("Memory allocation error.");
    }
    strcpy(pvmnodestoadd[i], pvmnodetoadd);
    i++;
  }
  if (i < numLocales) {
    fclose(nodelistfile);
    while (i >= 0) {
      free(pvmnodestoadd[i]);
      i--;
    }
    chpl_internal_error("Number of locales specified is greater than what's known in PVM hostfile.");
  }
  fclose(nodelistfile);

  info = pvm_start_pvmd(0, argtostart, 1);
  if (info != 0) {
    chpl_internal_error("Problem starting PVM daemon.");
  }

  for (i = 0; i < numLocales; i++) {
    hosts2[i] = (char *)pvmnodestoadd[i];
  }

  fprintf(stderr, "Got here.\n");
  info = pvm_addhosts( (char **)hosts2, i, infos );
  fprintf(stderr, "info=%d\n", info);

  for (i = 0; i < numLocales; i++) {
    hosts2[i] = (char *)pvmnodestoadd[i];
    fprintf(stderr, "hosts2[%d]=%s\n", i, hosts2[i]);
  }

  pvmsize += strlen("_real") + strlen(argv[0]);
  pvmsize2 += strlen("  ") + strlen(argv[1]) + strlen(argv[2]) + strlen(i_str);
  commandtopvm = chpl_malloc(pvmsize, sizeof(char*), "thing to execute via PVM spawn", false, -1, "");
  argtopvm = chpl_malloc(pvmsize, sizeof(char*), "arguments to commandtopvm", false, -1, "");
  for (i = 0; i < numLocales; i++) {
    *commandtopvm = '\0';
    *argtopvm = '\0';
    sprintf(i_str, "%d", i);
    strcat(commandtopvm, argv[0]);
    strcat(commandtopvm, "_real");

    strcat(argtopvm, argv[1]);
    strcat(argtopvm, " ");
    strcat(argtopvm, argv[2]);
    strcat(argtopvm, " ");
    strcat(argtopvm, i_str);
    fprintf(stderr, "String is: %s %s\n", commandtopvm, argtopvm);
    argtostart2[0] = argtopvm;
    numt = pvm_spawn( (char *)"hello_real", argtostart2, 1, (char *)pvmnodestoadd[i], 1, &tids[i] );
    fprintf(stderr, "numt=%d on %s with tids[%d]=%d of numLocales=%d\n", numt, pvmnodestoadd[i], i, tids[i], numLocales);
  }

  info = pvm_mytid();
  fprintf(stderr, "I am tid=%d\n", info);

  bufid = pvm_recv(-1, NOTIFYTAG);
  pvm_bufinfo(bufid, &bytes, &pvmtype, &source);
  fprintf(stderr, "Received bytes=%d, pvmtype=%d, source=%d\n", bytes, pvmtype, source);
  pvm_upkint(&end, 1, 1);
  if (end == 1) {
    fprintf(stderr, "Got call to end.\n");
  }

  //  sleep(20);

  fprintf(stderr, "Got here to delete.\n");
  info = pvm_delhosts( (char **)hosts2, i, infos );
  fprintf(stderr, "info=%d\n", info);

  while (i >= 0) {
    free(pvmnodestoadd[i]);
    i--;
  }
  //  chpl_free(hosts, false, -1, "");
  chpl_free(commandtopvm, false, -1, "");
  chpl_free(argtopvm, false, -1, "");
  pvm_halt();
  return (char *)"date";
}

void chpl_launch(int argc, char* argv[], int32_t numLocales) {
  chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                           argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  return 0;
}
