#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"
#include <stdlib.h>

// TODO: Un-hard-code this stuff:
static const char* pvmrunPath = "$CHPL_HOME/third-party/pvm/pvm-3.4.6/pvm3/lib/";
static const char* hostfile = "$CHPL_HOME/third-party/pvm/pvm-3.4.6/pvm3/hostfile";

static char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i;
  char i_str[128];
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
  addsize += (i * strlen("add \n")) + strlen("echo \"\"");
  basesize += strlen("echo \"\" | pvm && sleep 5 && ") + strlen(pvmrunPath) + (i * strlen("spawn - -> _real   \n")) + (i * strlen(argv[0])) + (i * strlen(argv[1])) + (i * strlen(argv[2])) + (i * strlen(i_str));
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
  strcat(baseCommand, "pvm && sleep 5 && ");
  strcat(deleteCommand, "\"");
  strcat(topvmCommand, " | ");
  strcat(topvmCommand, pvmrunPath);
  strcat(topvmCommand, "pvm");

  size = strlen(addCommand) + strlen(topvmCommand) + strlen(" && sleep 3 && ") + strlen(deleteCommand) + strlen(baseCommand) + strlen(topvmCommand) + 1;
  command = chpl_malloc(size, sizeof(char*), "pvm command buffer", false, -1, "");
  *command = '\0';

  strcat(command, addCommand);
  strcat(command, topvmCommand);
  strcat(command, " && sleep 3 && ");
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
  chpl_free(addCommand, false, -1, "");
  chpl_free(deleteCommand, false, -1, "");
  chpl_free(topvmCommand, false, -1, "");
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
