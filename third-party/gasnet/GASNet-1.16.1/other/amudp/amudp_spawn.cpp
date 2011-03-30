/*   $Source: /var/local/cvs/gasnet/other/amudp/amudp_spawn.cpp,v $
 *     $Date: 2006/05/23 12:42:29 $
 * $Revision: 1.15 $
 * Description: AMUDP Implementations of SPMD spawn functions for various environments
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include <amudp_internal.h>
#include <amudp_spmd.h>

#include <errno.h>
#if PLATFORM_OS_MSWINDOWS
  #include <winsock2.h>
  #include <windows.h>  
  #define sleep(x) Sleep(1000*x)
  #include <process.h>
  #include <io.h>
  #include <direct.h>
#endif
#if PLATFORM_ARCH_CRAYX1
  #include <unistd.h>
  #include <sys/prctl.h>
  extern char **environ; 
#endif

amudp_spawnfn_desc_t const AMUDP_Spawnfn_Desc[] = {
  { 'S',  "Spawn jobs using ssh remote shells", 
    (amudp_spawnfn_t)AMUDP_SPMDSshSpawn },
  { 'L',  "Spawn jobs using fork()/exec() on the local machine (good for SMP's)", 
    (amudp_spawnfn_t)AMUDP_SPMDLocalSpawn },
#ifdef REXEC
  { 'R',  "Spawn jobs using rexec (Berkeley Millennium only)", 
    (amudp_spawnfn_t)AMUDP_SPMDRexecSpawn },
#endif
#ifdef GLUNIX
  { 'G',  "Spawn jobs using GLUNIX (Berkeley NOW only)", 
    (amudp_spawnfn_t)AMUDP_SPMDGlunixSpawn },
#endif
  { 'C',  "Spawn jobs using custom job spawner ("AMUDP_ENV_PREFIX_STR"_CSPAWN_CMD)", 
    (amudp_spawnfn_t)AMUDP_SPMDCustomSpawn },
  { '\0', NULL, (amudp_spawnfn_t)NULL }
};

/* ------------------------------------------------------------------------------------ 
 *  spawn jobs on local machine
 * ------------------------------------------------------------------------------------ */
extern int AMUDP_SPMDLocalSpawn(int nproc, int argc, char **argv) {
  /* just a simple fork/exec */
  int i;

  if (!AMUDP_SPMDSpawnRunning) {
    AMUDP_Err("Spawn functions should never be run directly - only passed to AMUDP_SPMDStartup()"); 
    return FALSE;
  }

  for (i = 0; i < nproc; i++) {
    #if PLATFORM_OS_MSWINDOWS && !PLATFORM_OS_CYGWIN
      if (_spawnv(_P_NOWAIT, argv[0], argv) == -1)
        AMUDP_FatalErr("failed _spawnv()");
    #elif PLATFORM_ARCH_CRAYX1
      { char **nargv = (char **)AMUDP_malloc(sizeof(char *)*(argc+2));
        nargv[0] = argv[0];
        memcpy(nargv+1,argv,argc*sizeof(char *));
        nargv[argc+1] = NULL;
        if (execsp(nargv, environ, NULL) == -1)
          AMUDP_FatalErr("failed execsp()");
      }
    #else
      int forkRet = fork();
      if (forkRet == -1) {
        perror("fork");
        return FALSE;
      } else if (forkRet != 0) continue;  /*  this is the parent, will go back to the top of the loop */
      else {  /*  this is the child - exec the new process */
        /*  could close some resources here (like AMUDP_SPMDListenSocket) but not strictly necessary */

        #if 0
          /*  put new process in a separate process group */
          if (setsid() == -1) perror("setsid"); 
        #endif

        /*  exec the program, with the given arguments  */
        execv(argv[0], argv);

        /*  if execv returns, an error occurred */
        perror("execv");
        _exit(1); /*  use _exit() to prevent corrupting parent's io buffers */
      } /*  child */
    #endif
  }
  return TRUE;
}
/* ------------------------------------------------------------------------------------ 
 *  spawn jobs using Glunix (requires GLUNIX be defined when library is built)
 * ------------------------------------------------------------------------------------ */
#ifdef GLUNIX
  #include <glib.h>
  extern int AMUDP_SPMDGlunixSpawn(int nproc, int argc, char **argv) {
    /* GLUNIX has good built-in facilities for remote spawn */
    int forkRet;

    if (!AMUDP_SPMDSpawnRunning) {
      AMUDP_Err("Spawn functions should never be run directly - only passed to AMUDP_SPMDStartup()"); 
      return FALSE;
    }

    forkRet = fork();
    if (forkRet == -1) {
      perror("fork");
      return FALSE;
    } else if (forkRet != 0) {
      AMUDP_SPMDRedirectStdsockets = FALSE; /* GLUNIX has it's own console redirection facilities */
      return TRUE;  
    } else {  /*  this is the child - exec the new process */
      if (!Glib_Initialize())
        AMUDP_FatalErr("failed to Glib_Initialize()")
      /*AMUDP_assert(Glib_AmIStartup() > 0);*/
      Glib_Spawn(nproc, argv[0], argv); /* should never return */
      AMUDP_FatalErr("failed to Glib_Spawn()")
      return FALSE;
    }
  }
#else
  extern int AMUDP_SPMDGlunixSpawn(int nproc, int argc, char **argv) {
    AMUDP_Err("AMUDP_SPMDGlunixSpawn() cannot be called because the AMUDP library was compiled without -DGLUNIX.\n"
             "  Please recompile libAMUDP.a with -DGLUNIX, relink your application and try again.");
    return FALSE;
  }
#endif
/* ------------------------------------------------------------------------------------ 
 *  spawn jobs using REXEC utility (Berkeley Millennium project)
 * ------------------------------------------------------------------------------------ */
/* rexec provides decent support for remote spawn
 * see http://www.millennium.berkeley.edu/rexec/ for instructions on setting up your environment
 */
/* rexec spawn uses the following environment variables:
 *
 *  option         default       description
 * --------------------------------------------------
 * REXEC_SVRS      none          list of servers to use
 * REXEC_CMD       "rexec"       rexec command to use
 * REXEC_OPTIONS   "-q"            additional options to give rexec 
 * (any environment variable may be specified with an optional prefix 
 *  of AMUDP_ or AMUDP_ENV_PREFIX##_)
 */
#ifdef REXEC
  int AMUDP_SPMDRexecSpawn(int nproc, int argc, char **argv) {
    int i;
    char *rexec_cmd;
    char *rexec_options;
    char cmd1[1024],cmd2[1024];
    int pid;

    if (!AMUDP_SPMDSpawnRunning) {
      AMUDP_Err("Spawn functions should never be run directly - only passed to AMUDP_SPMDStartup()"); 
      return FALSE;
    }

    pid = getpid();

    rexec_cmd = AMUDP_getenv_prefixed_withdefault("REXEC_CMD", "rexec");
    rexec_options = AMUDP_getenv_prefixed_withdefault("REXEC_OPTIONS", "-q");

    cmd1[0] = '\0';
    for (i = 0; i < argc; i++) {
      AMUDP_assert(argv[i]);
      strcat(cmd1,"'");
      strcat(cmd1,argv[i]);
      strcat(cmd1,"' ");
    }
    AMUDP_assert(!argv[i]);

    /* build the rexec command */
    sprintf(cmd2, "%s %s -n %i /bin/sh -c \"%s%s\" " /* shell wrapper required because of crappy rexec implementation */
     " || ( echo \"rexec spawn failed.\" ; kill %i ) ",
      rexec_cmd, rexec_options, nproc,

      (AMUDP_SilentMode?"":"echo connected to \\`uname -n\\`... ; "),

      cmd1, pid

    );

    { int forkRet;
      forkRet = fork(); /* fork a new process to hold rexec master */

      if (forkRet == -1) {
        perror("fork");
        return FALSE;
      } else if (forkRet != 0) {
        AMUDP_SPMDRedirectStdsockets = FALSE; /* REXEC has it's own console redirection facilities */
        return TRUE;  
      } else {  /*  this is the child - exec the new process */
        if (!AMUDP_SilentMode) 
          printf("system(%s)\n", cmd2); fflush(stdout);
        if (system(cmd2) == -1)
           AMUDP_FatalErr("Failed to call system() to spawn rexec");
        exit(0);
      }
    }
    return TRUE;
}
#else
  extern int AMUDP_SPMDRexecSpawn(int nproc, int argc, char **argv) {
    AMUDP_Err("AMUDP_SPMDRexecSpawn() cannot be called because the AMUDP library was compiled without -DREXEC.\n"
             "  Please recompile libAMUDP.a with -DREXEC, relink your application and try again.");
    return FALSE;
  }
#endif
/* ------------------------------------------------------------------------------------ 
 *  spawn jobs using ssh remote shell
 * ------------------------------------------------------------------------------------ */
/* ssh requires some massaging to provide reasonable behavior for spawning parallel jobs
 * the best way to use this is to setup an identity file in ~/.ssh so you don't get 
 * prompted for passwords. You also need to set the environment variables below
 */
/* this implementation of ssh spawn has the following compile-time constants: */
#define SSH_PARALLELSPAWN        1        /* spawn jobs in parallel for faster startups */
#define SSH_SUPRESSNEWKEYPROMPT  1        /* suppress the trust-first-time connection prompt */
#define SSH_PREVENTRSHFALLBACK   1        /* prevent falling back on rsh if ssh connection fails */
#define SSH_SERVERS_DELIM_CHARS  " ,/;:"  /* legal delimiter characters for SSH_SERVERS */

/* also has the following environment variables:
 *
 *  option      default                     description
 * ----------------------------------------------------------------
 * SSH_SERVERS  none - must be provided     list of servers to use
 * SSH_CMD      "ssh"                       ssh command to use
 * SSH_OPTIONS  ""                          additional options to give ssh
 * SSH_REMOTE_PATH  current working dir.    the directory to use on remote machine
 * (any environment variable may be specified with an optional prefix 
 *  of AMUDP_ or AMUDP_ENV_PREFIX##_)
 */

int AMUDP_SPMDSshSpawn(int nproc, int argc, char **argv) {
  int i;
  const char *ssh_servers;
  const char *ssh_cmd;
  const char *ssh_options;
  const char *ssh_remote_path;
  char cwd[1024];
  const char *p;
  char cmd1[1024],cmd2[1024];
  int pid;

  if (!AMUDP_SPMDSpawnRunning) {
    AMUDP_Err("Spawn functions should never be run directly - only passed to AMUDP_SPMDStartup()"); 
    return FALSE;
  }

  pid = getpid();

  ssh_servers = AMUDP_getenv_prefixed_withdefault("SSH_SERVERS","");
  if (!strlen(ssh_servers)) {
    printf("Environment variable SSH_SERVERS is missing.\n");
    return FALSE;
  }


  if (!getcwd(cwd, 1024)) {
    printf("Error calling getcwd()\n");
    return FALSE;
  }
  ssh_remote_path = AMUDP_getenv_prefixed_withdefault("SSH_REMOTE_PATH", cwd);
  ssh_cmd = AMUDP_getenv_prefixed_withdefault("SSH_CMD", "ssh");

  int isOpenSSH = 0; /* figure out if we're using OpenSSH */
  { char cmdtmp[1024];
    sprintf(cmdtmp,"%s -v 2>&1 | grep OpenSSH", ssh_cmd);
    FILE *pop = popen(cmdtmp,"r");
    while (!feof(pop) && !ferror(pop)) {
      int next = fgetc(pop);
      if (next != EOF && !isspace(next)) {
        isOpenSSH = 1;
        break;
      }
    }
    pclose(pop);
  }

  ssh_options = AMUDP_getenv_prefixed_withdefault("SSH_OPTIONS","");

  cmd1[0] = '\0';
  for (i = 0; i < argc; i++) {
    AMUDP_assert(argv[i] != NULL);
    strcat(cmd1,"'");
    strcat(cmd1,argv[i]);
    strcat(cmd1,"' ");
  }
  AMUDP_assert(!argv[i]);

  p = ssh_servers;
  for (i = 0; i < nproc; i++) { /* check we have enough servers */
    const char *end;
    while (*p && strchr(SSH_SERVERS_DELIM_CHARS, *p)) p++;
    end = p + strcspn(p, SSH_SERVERS_DELIM_CHARS);
    if (p == end) {
      printf("Not enough machines in environment variable SSH_SERVERS to satisfy request for (%i).\n"
       "Only (%i) machines available: %s\n", nproc, i, ssh_servers);
      return FALSE;
    }
    if (*end) p = end+1;
    else p = end;
  } 

  p = ssh_servers;
  for (i = 0; i < nproc; i++) {
    char ssh_server[255];
    const char *end;
    while (*p && strchr(SSH_SERVERS_DELIM_CHARS, *p)) p++;
    end = p + strcspn(p, SSH_SERVERS_DELIM_CHARS);
    AMUDP_assert(p != end);

    strncpy(ssh_server, p, (end-p));
    ssh_server[end-p] = '\0'; 

    /* build the ssh command */
    sprintf(cmd2, "%s %s %s %s %s %s \" %s cd '%s' ; %s\" "
      " || ( echo \"connection to %s failed.\" ; kill %i ) "
      "%s", 
      ssh_cmd,

      (isOpenSSH?"-f":""),    /* go into background and nullify stdin */

      #if SSH_SUPRESSNEWKEYPROMPT
        (isOpenSSH?"-o 'StrictHostKeyChecking no'":""),
      #else 
        "",
      #endif

      #if SSH_PREVENTRSHFALLBACK
        (isOpenSSH?"-o 'FallBackToRsh no'":""),
      #else 
        "",
      #endif

      ssh_options, ssh_server, 
      
      (AMUDP_SilentMode?"":"echo connected to \\$HOST... ;"),

      ssh_remote_path, cmd1, ssh_server, pid,

      #if SSH_PARALLELSPAWN
        "&"
      #else
        ""
      #endif
    );

    if (!AMUDP_SilentMode) 
      printf("system(%s)\n", cmd2); fflush(stdout);
    if (system(cmd2) == -1) {
      printf("Failed to call system() to spawn");
      return FALSE;
    }
    if (*end) p = end+1;
    else p = end;
  } 

  return TRUE;
}
/* ------------------------------------------------------------------------------------ 
 *  spawn jobs using a user-defined command
 * ------------------------------------------------------------------------------------ */
/* AMUDP custom spawn uses the following environment variables:
 *
 *  option           default    description
 * --------------------------------------------------
 * CSPAWN_SERVERS      none     list of servers to use - only required if %M is used below
 * CSPAWN_CMD          none     command to call for spawning - the following replacements will be made:
 *                              %M => list of servers taken from AMUDP_CSPAWN_SERVERS (the first nproc are taken)
 *                              %N => number of worker nodes requested
 *                              %C => AMUDP command that should be run by each worker node
 *                              %D => the current working directory
 *                              %% => %
 * CSPAWN_ROUTE_OUTPUT <not set>   set this variable to request stdout/stderr routing of workers
 * (any environment variable may be specified with an optional prefix 
 *  of AMUDP_ or AMUDP_ENV_PREFIX##_)
 */

int AMUDP_SPMDCustomSpawn(int nproc, int argc, char **argv) {
  int i;
  char *spawn_cmd = NULL;
  char *spawn_servers = NULL;
  char workerservers[1024];
  char workercmd[1024];
  char nproc_str[10];
  char cwd[1024];
  char cmd[1024];
  int spawn_route_output;
  int pid;

  if (!AMUDP_SPMDSpawnRunning) {
    AMUDP_Err("Spawn functions should never be run directly - only passed to AMUDP_SPMDStartup()"); 
    return FALSE;
  }

  pid = getpid();

  spawn_cmd = AMUDP_getenv_prefixed_withdefault("CSPAWN_CMD","");
  if (!strlen(spawn_cmd)) {
    AMUDP_Err("You must set the "AMUDP_ENV_PREFIX_STR"_CSPAWN_CMD environment variable to use the custom spawn function"); 
    return FALSE;
  }
  spawn_servers = AMUDP_getenv_prefixed_withdefault("CSPAWN_SERVERS","");
  if (!strlen(spawn_servers)) spawn_servers = NULL;
  spawn_route_output = 
    strcmp(AMUDP_getenv_prefixed_withdefault("CSPAWN_ROUTE_OUTPUT","0"),"0");


  if (spawn_servers) { /* build server list */
    char *p = spawn_servers;
    char servername[255];
    char serverDelim[2];
    strcpy(serverDelim," "); /* default to space */
    workerservers[0] = '\0';
    for (i = 0; i < nproc; i++) { /* check we have enough servers & copy the right number */
      char *end;
      while (*p && strchr(SSH_SERVERS_DELIM_CHARS, *p)) p++;
      end = p + strcspn(p, SSH_SERVERS_DELIM_CHARS);
      if (p == end) {
        printf("Not enough machines in environment variable "AMUDP_ENV_PREFIX_STR"_CSPAWN_SERVERS to satisfy request for (%i).\n"
          "Only (%i) machines available: %s\n", nproc, i, spawn_servers);
        return FALSE;
      }
      strncpy(servername, p, end-p);
      servername[end-p] = '\0';
      if (workerservers[0]) strcat(workerservers, serverDelim);
      if (*end) serverDelim[0] = *end;
      strcat(workerservers, servername);
      if (*end) p = end+1;
      else p = end;
    }
  } 

  sprintf(nproc_str, "%i", nproc);

  if (!getcwd(cwd, 1024)) {
    printf("Error calling getcwd()\n");
    return FALSE;
  }

    /* build the worker command */
  { char temp[1024];
    temp[0] = '\0';
    for (i = 0; i < argc; i++) {
      AMUDP_assert(argv[i] != NULL);
      strcat(temp,"'");
      strcat(temp,argv[i]);
      strcat(temp,"' ");
    }
    AMUDP_assert(!argv[i]);

    sprintf(workercmd, "/bin/sh -c \"%s%s\" || ( echo \"spawn failed.\" ; kill %i ) ",
      (AMUDP_SilentMode?"":"echo connected to `uname -n`... ; "),
      temp, pid
    );
  }

  strcpy(cmd, spawn_cmd);
  { char tmp[1024];
    char *p = cmd;
    while ((p = strchr(p, '%'))) {
      const char *replacement;
      switch (*(p+1)) {
        case 'M': case 'm': 
          if (!spawn_servers) { /* user failed to provide servers and now is asking for them */
            AMUDP_Err("You must set the "AMUDP_ENV_PREFIX_STR"_CSPAWN_SERVERS environment "
                       "variable to use the %%M option in "AMUDP_ENV_PREFIX_STR"_CSPAWN_CMD");
          }
          replacement = workerservers; 
          break;
        case 'N': case 'n': replacement = nproc_str; break;
        case 'C': case 'c': replacement = workercmd; break;
        case 'D': case 'd': replacement = cwd; break;
        case '%': replacement = "%"; break;
        default: replacement = "";
      }
      strcpy(tmp, cmd);
      tmp[p-cmd] = '\0';
      strcat(tmp, replacement);
      if (*(p+1)) strcat(tmp, p+2);
      p += strlen(replacement);
      strcpy(cmd, tmp);
    }
  }

  AMUDP_SPMDRedirectStdsockets = spawn_route_output; 

  {
  #if !PLATFORM_OS_MSWINDOWS
    int forkRet;
    forkRet = fork(); /* fork a new process to hold cmd master */

    if (forkRet == -1) {
      perror("fork");
      return FALSE;
    }
    else if (forkRet != 0) {
      return TRUE;  
    }
    else 
  #endif
    {  /*  this is the child - exec the new process */
      if (!AMUDP_SilentMode) 
        printf("system(%s)\n", cmd); fflush(stdout);
      if (system(cmd) == -1)
         AMUDP_FatalErr("Failed while calling system() with custom spawn command:\n%s", cmd);
      exit(0);
    }
  }

  return TRUE;
}
/* ------------------------------------------------------------------------------------ */
