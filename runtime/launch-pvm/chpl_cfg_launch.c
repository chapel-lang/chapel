#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"
#include <stdlib.h>

// TODO: Un-hard-code this stuff:
static const char* pvmrunPath = "/tmp/Chapel/pvm3/lib/";
static const char* hostfile = "/tmp/Chapel/pvm3/hostfile";

char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i;
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

  // Add nodes to PVM configuration.
  FILE* nodelistfile;
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
  addsize += (i * strlen("add \n")) + strlen("echo \"");
  basesize += (i * strlen(" ")) + strlen("spawn -> \n") + strlen(argv[0]);
  deletesize += (i * strlen("delete \n")) + strlen("\"");
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
  addCommand = chpl_malloc(addsize, sizeof(char*), "first part pvm command buffer", -1, "");
  baseCommand = chpl_malloc(basesize, sizeof(char*), "actual command to spawn", -1, "");
  deleteCommand = chpl_malloc(deletesize, sizeof(char*), "last part pvm command buffer", -1, "");
  topvmCommand = chpl_malloc(topvmsize, sizeof(char*), "send to pvm via pipe command buffer", -1, "");
  *addCommand = '\0';
  *baseCommand = '\0';
  *deleteCommand = '\0';
  *topvmCommand = '\0';
  strcat(addCommand, "echo \"");
  strcat(baseCommand, "spawn -> ");
  strcat(baseCommand, argv[0]);
  while (i < numLocales) {
    strcat(addCommand, "add ");
    strcat(addCommand, nodestoadd[i]);
    strcat(addCommand, "\n");
    strcat(baseCommand, " ");
    strcat(baseCommand, nodestoadd[i]);
    strcat(deleteCommand, "delete ");
    strcat(deleteCommand, nodestoadd[i]);
    strcat(deleteCommand, "\n");
    i++;
  }
  strcat(baseCommand, "\n");
  strcat(deleteCommand, "\"");
  strcat(topvmCommand, " | ");
  strcat(topvmCommand, pvmrunPath);
  strcat(topvmCommand, "pvm");

  size = strlen(addCommand) + strlen(deleteCommand) + strlen(baseCommand) + strlen(topvmCommand) + 1;
  command = chpl_malloc(size, sizeof(char*), "pvm command buffer", -1, "");
  *command = '\0';

  strcat(command, addCommand);
  strcat(command, baseCommand);
  strcat(command, deleteCommand);
  strcat(command, topvmCommand);

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  while (i >= 0) {
    free(nodestoadd[i]);
    i--;
  }
  chpl_free(addCommand, -1, "");
  chpl_free(deleteCommand, -1, "");
  chpl_free(topvmCommand, -1, "");
  return command;
}

void chpl_launch_sanity_checks(int argc, char* argv[], const char* cmd) {
  // Do sanity checks just before launching.
  struct stat statBuf;
  char realName[256];
  int retVal;

  retVal = snprintf(realName, 256, "%s", argv[0]);
  if (retVal < 0 || retVal >= 256) {
    chpl_internal_error("error generating back-end filename");
  }

  // Make sure the _real binary exists
  if (stat(realName, &statBuf) != 0) {
    char errorMsg[256];
    sprintf(errorMsg, "unable to locate file: %s", realName);
    chpl_error(errorMsg, -1, "<internal>");
  }
}

void chpl_launch_cleanup(void) {
}
