#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <chplio_md.h>

#include "pvm3.h"

#include "chplcgfns.h"
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
char *replace_str(char *str, char *orig, char *rep);

// Helper function
char *replace_str(char *str, char *orig, char *rep)
{
  static char buffer[4096];
  char *p;

  if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
    return str;

  strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

  return buffer;
}



static char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i, j;                           // j acts as an iterator for
                                      // pvm_addhosts and pvm_delhosts
                                      // if this node (running launcher) is
                                      // in hostfile, adding or deleting it
                                      // can result in error

  int info;
  int infos[256];
  int infos2[256];
  char myhostname[256];
  char pvmnodetoadd[256];
  char* pvmnodestoadd[2048];
  int pvmsize = 0;
  int commsig = 0;
  char* commandtopvm;
  char* environment;
  static char *hosts2[2048];
  static char *hosts2redo[2048];
  int numt;
  int tids[32];
  static char *argtostart[] = {(char*)""};
  int bufid;
  char debugMsg[4096];

  // These are for receiving singals from slaves
  int fdnum;
  char buffer[PRINTF_BUFF_LEN];
  char description[PRINTF_BUFF_LEN];  // gdb specific
  int ignorestatus;                   // gdb specific
  int who;                            // gdb specific

  char** argv2;
  char* argv0rep;
  char* nameofbin;
  char numlocstr[128];

  // Add nodes to PVM configuration.
  FILE* nodelistfile;

  int k;                              // k iterates over chpl_numRealms
  int lpr;                            // locales per realm
  char* realmtype;
  char* realmtoadd[2048];
  char* multirealmenvname;
  char* multirealmenv;
  char* multirealmpathtoadd[2048];
  int baserealm;
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
  i = 0;
  for (k = 0; k < chpl_numRealms; k++) {
    if (chpl_numRealms != 1) {
      lpr = chpl_localesPerRealm(k);
      if (lpr == 0) {
        continue;
      }
    } else {
      lpr = numLocales;
    }
    hostfile = chpl_malloc(1024, sizeof(char*), CHPL_RT_MD_PVM_LIST_OF_NODES, -1, "");
    realmtype = chpl_malloc(1024, sizeof(char*), CHPL_RT_MD_PVM_LIST_OF_NODES, -1, "");
    multirealmenvname = chpl_malloc(1024, sizeof(char*), CHPL_RT_MD_PVM_LIST_OF_NODES, -1, "");
    if (chpl_numRealms != 1) {
      sprintf(realmtype, "%s", chpl_realmType(k));
    } else {
      sprintf(realmtype, "%s", getenv((char *)"CHPL_HOST_PLATFORM"));
    }
    sprintf(multirealmenvname, "CHPL_MULTIREALM_LAUNCH_DIR_%s", realmtype);
    multirealmenv = getenv(multirealmenvname);
    if (multirealmenv == NULL) {
      multirealmenv = getenv((char *)"CHPL_HOME");
    }
    chpl_free(multirealmenvname, -1, "");
    sprintf(hostfile, "%s%s%s", getenv((char *)"CHPL_HOME"), "/hostfile.", realmtype);
    
    if ((nodelistfile = fopen(hostfile, "r")) == NULL) {
      fprintf(stderr, "Make sure %s is present and readable.\n", hostfile);
      chpl_free(hostfile, -1, "");
      chpl_free(realmtype, -1, "");
      chpl_free(argv2, -1, "");
      chpl_internal_error("Exiting.");
    }
    chpl_free(hostfile, -1, "");
    j = 0;
    while (((fscanf(nodelistfile, "%s", pvmnodetoadd)) == 1) && (j < lpr)) {
      pvmnodestoadd[i] = chpl_malloc((strlen(pvmnodetoadd)+1), sizeof(char *), CHPL_RT_MD_PVM_LIST_OF_NODES, -1, "");
      realmtoadd[i] = chpl_malloc((strlen(realmtype)+1), sizeof(char *), CHPL_RT_MD_PVM_LIST_OF_NODES, -1, "");
      multirealmpathtoadd[i] = chpl_malloc((strlen(multirealmenv)+1), sizeof(char *), CHPL_RT_MD_PVM_LIST_OF_NODES, -1, "");
      strcpy(pvmnodestoadd[i], pvmnodetoadd);
      strcpy(realmtoadd[i], realmtype);
      strcpy(multirealmpathtoadd[i], multirealmenv);
      //      fprintf(stderr, "Adding pvmnodestoadd[%d], realm %s of j iter %d on directory %s: %s\n", i, realmtoadd[i], j, multirealmpathtoadd[i], pvmnodestoadd[i]);
      i++;
      j++;
    }
    chpl_free(realmtype, -1, "");
    // Check to make sure user hasn't specified more nodes (-nl <n>) than
    // what's included in the hostfile.
    if (j < lpr) {
      fclose(nodelistfile);
      while (i >= 0) {
        chpl_free(pvmnodestoadd[i], -1, "");
        chpl_free(realmtoadd[i], -1, "");
        chpl_free(multirealmpathtoadd[i], -1, "");
        i--;
      }
      chpl_free(argv2, -1, "");
      chpl_internal_error("Number of locales specified is greater than what's known in PVM hostfile.");
    }
    fclose(nodelistfile);
  }

  sprintf(buffer, "touch /tmp/Chplpvmtmp && rm -rf /tmp/*pvm* && killall -q -9 pvmd3");
  system(buffer);
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
      baserealm = j;
      j++;
    }
  }

  if (i != 0) {
    info = pvm_addhosts( (char **)hosts2, i, infos );
  }
  j = i;

  // Something happened on addhosts -- likely old pvmd running
  for (i = 0; i < j; i++) {
    if ((infos[i] < 0) && (infos[i] != -28)) {
      sprintf(buffer, "ssh -q %s \"touch /tmp/Chplpvmtmp && rm -rf /tmp/*pvm* && killall -q -9 pvmd3\"", hosts2[i]);
      system(buffer);
      hosts2redo[0] = hosts2[i];
      info = pvm_addhosts( (char **)hosts2redo, 1, infos2);
      if (infos2[0] < 0) {
        fprintf(stderr, "Remote error on %s: %d\n", hosts2redo[0], infos2[0]);
        fprintf(stderr, "Shutting down host.\n");
        pvm_halt();
        chpl_internal_error("Exiting");
      }
    }
  }

  argv0rep = chpl_malloc(1024, sizeof(char*), CHPL_RT_MD_PVM_SPAWN_THING, -1, "");
  nameofbin = chpl_malloc(1024, sizeof(char*), CHPL_RT_MD_PVM_SPAWN_THING, -1, "");
  strcpy(argv0rep, argv[0]);
  nameofbin = strrchr(argv[0], '/');
  // Build the command to send to pvm_spawn.
  // First, try the command built from CHPL_MULTIREALM_LAUNCH_DIR_<realm>
  //      and the executable_real. Replace architecture strings with target
  //      architecture names.
  // If this doesn't work, store the name of the file tried with the node
  //      into a debug message. Then try just what was passed on the command
  //      line.
  // Failing that, try the current working directory with executable_real.
  // If this doesn't work, error out with the debug message.
  for (i = 0; i < numLocales; i++) {
    //    fprintf(stderr, "Loop i=%d (iteration %d of %d)\n", i, i+1, numLocales);
    pvmsize += strlen(multirealmpathtoadd[i]) + strlen("_real") + strlen(nameofbin);

    commandtopvm = chpl_malloc(pvmsize, sizeof(char*), CHPL_RT_MD_PVM_SPAWN_THING, -1, "");
    *commandtopvm = '\0';
    environment = chpl_malloc(1024, sizeof(char*), CHPL_RT_MD_PVM_SPAWN_THING, -1, "");
    *environment = '\0';
    strcat(environment, multirealmpathtoadd[i]);
    strcat(commandtopvm, environment);
    chpl_free(environment, -1, "");
    strcat(commandtopvm, nameofbin);
    strcat(commandtopvm, "_real");

    while (strstr(commandtopvm, realmtoadd[baserealm]) && 
           (chpl_numRealms != 1) &&
           (strcmp(realmtoadd[baserealm], realmtoadd[i]))) {
      commandtopvm = replace_str(commandtopvm, realmtoadd[baserealm], realmtoadd[i]);
    }

    //    fprintf(stderr, "spawning %s on %s\n", commandtopvm, pvmnodestoadd[i]);
    numt = pvm_spawn( (char *)commandtopvm, argv2, 1, (char *)pvmnodestoadd[i], 1, &tids[i] );
    //    fprintf(stderr, "numt was %d, tids[%d] was %d\n", numt, i, tids[i]);
    if (numt == 0) {
      sprintf(debugMsg, "Make sure %s is present and readable on %s.", commandtopvm, pvmnodestoadd[i]);
      if (tids[i] == PvmNoFile) {
        chpl_free(commandtopvm, -1, "");
        pvmsize = strlen(argv0rep) + strlen("_real");
        commandtopvm = chpl_malloc(pvmsize, sizeof(char*), CHPL_RT_MD_PVM_SPAWN_THING, -1, "");
        *commandtopvm = '\0';
        strcat(commandtopvm, argv0rep);
        strcat(commandtopvm, "_real");
        while (strstr(commandtopvm, realmtoadd[baserealm]) && 
               (chpl_numRealms != 1) &&
               (strcmp(realmtoadd[baserealm], realmtoadd[i]))) {
          commandtopvm = replace_str(commandtopvm, realmtoadd[baserealm], realmtoadd[i]);
        }
        //        fprintf(stderr, "trying again to spawn %s on %s\n", commandtopvm, pvmnodestoadd[i]);
        numt = pvm_spawn( (char *)commandtopvm, argv2, 1, (char *)pvmnodestoadd[i], 1, &tids[i] );
        //        fprintf(stderr, "numt was %d, tids[%d] was %d\n", numt, i, tids[i]);
        if (numt == 0) {
          if (tids[i] == PvmNoFile) {
            chpl_free(commandtopvm, -1, "");
            commandtopvm = chpl_malloc(1024, sizeof(char*), CHPL_RT_MD_PVM_SPAWN_THING, -1, "");
            *commandtopvm = '\0';
            sprintf(commandtopvm, "%s%s%s", getenv((char *)"PWD"), nameofbin, "_real");
            //            fprintf(stderr, "try 3 to spawn %s on %s\n", commandtopvm, pvmnodestoadd[i]);
            numt = pvm_spawn( (char *)commandtopvm, argv2, 1, (char *)pvmnodestoadd[i], 1, &tids[i] );
            //            fprintf(stderr, "numt was %d, tids[%d] was %d\n", numt, i, tids[i]);
            if (numt == 0) {
              if (j != 0) {
                info = pvm_delhosts( (char **)hosts2, j, infos );
              }
              chpl_internal_error(debugMsg);
            }
          }
        }
      }
    }
  }

  // We have a working configuration. What follows is the communication
  // between the slaves and the parent (this process).
  info = pvm_mytid();

  while (commsig == 0) {
    bufid = pvm_recv(-1, NOTIFYTAG);
    pvm_upkint(&commsig, 1, 1);
    // fprintf case
    if (commsig == 2) {
      pvm_upkint(&fdnum, 1, 1);
      pvm_upkstr(buffer);
      if (fdnum == 0) {
        fprintf(stdin, "%s", buffer);
      } else if (fdnum == 1) {
        fprintf(stdout, "%s", buffer);
      } else {
        fprintf(stderr, "%s", buffer);
      }
      fflush(stderr);
      commsig = 0;
    }
    // printf case
    if (commsig == 3) {
      pvm_upkstr(buffer);
      printf("%s", buffer);
      fflush(stderr);
      commsig = 0;
    }
    // Run in gdb mode
    if (commsig == 4) {
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
