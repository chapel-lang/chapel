/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amudp/amudp_spawn.cpp $
 * Description: AMUDP Implementations of SPMD spawn functions for various environments
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#undef _PORTABLE_PLATFORM_H
#include <amx_portable_platform.h>

#include <errno.h>
#if PLATFORM_OS_CYGWIN
  #include <windows.h>  // CreateProcess
#endif
#if PLATFORM_ARCH_CRAYX1
  #include <unistd.h>
  #include <sys/prctl.h>
  extern char **environ; 
#endif
#include <string.h>
#include <signal.h>

#include <amudp_spmd.h>

#include "amudp_internal.h" // must come after any other headers

amudp_spawnfn_desc_t const AMUDP_Spawnfn_Desc[] = {
  { 'S',  "Spawn jobs using ssh remote shells", 
    (amudp_spawnfn_t)AMUDP_SPMDSshSpawn },
  { 'L',  "Spawn jobs using fork()/exec() on the local machine (good for SMP's)", 
    (amudp_spawnfn_t)AMUDP_SPMDLocalSpawn },
  { 'C',  "Spawn jobs using custom job spawner (" AMX_ENV_PREFIX_STR"_CSPAWN_CMD)", 
    (amudp_spawnfn_t)AMUDP_SPMDCustomSpawn },
  { '\0', NULL, (amudp_spawnfn_t)NULL }
};

/* Utility functions */
// Apply single quotes to prevent shell from evaluating special chars
char *quote_for_remote(const char *arg) {
  const char *p;
  int c;

  for (p=arg, c=0; NULL != (p = strchr(p, '\'')); ++c, ++p) { /*empty*/ }

  size_t old_len = strlen(arg);
  size_t new_len = old_len + 2 + 3 * c;
  char *result = (char *)AMX_malloc(1 + new_len);
  result[0] = '\'';
  result[1] = '\0';
  char *end = result+1;

  if (c) {
    char *dup = (char *)AMX_malloc(1 + old_len);
    p = strcpy(dup, arg);
    while (c--) {
      char *q = strchr((char *)p, '\'');
      *q = '\0';
      strcpy(end, p);       end += q-p;
      strcpy(end, "'\\''"); end += 4;
      p = q + 1;
    }
    AMX_free(dup);
  } else {
    p = arg;
  }
  strcpy(end, p);

  AMX_assert(strlen(result) == new_len-1);
  result[new_len-1] = '\'';
  result[new_len]   = '\0';

  return result;
}
// Apply quotes to protect against the shell spawned by system()
// Note this is for use *inside* the double quotes we place around a command run by system.
// shell docs tells us:
//   Inside double-quotes, '$' and '`' remain special unconditionally,
//   and '\' remains special only if followed by another special
char *quote_for_local(const char *arg) {
  const char specials[] = "$`\\\"";

  char *tmp = quote_for_remote(arg);

  // count specials (assume every backslash is special)
  const char *p;
  int c;
  for (p=tmp, c=0; NULL != (p = strpbrk(p, specials)); ++c, ++p) { /*empty*/ }

  if (!c) return tmp; // Nothing to protect from local shell

  size_t old_len = strlen(tmp);
  size_t new_len = old_len + c;
  char *result = (char *)AMX_malloc(1 + new_len);
  char *end = result;

  char *q, *dup = (char *)AMX_malloc(1 + old_len);
  p = strcpy(dup, tmp);
  while (NULL != (q = strpbrk((char *)p, specials))) {
    size_t len = q-p;
    strncpy(end, p, len);   end += len;
    end[0] = '\\';          end += 1;
    if (q[0] != '\\' || strchr(specials, q[1])) {
      end[0] = q[0];        end += 1;
    }
    p = q + 1;
  }
  strcpy(end, p);
  AMX_free(dup);
  AMX_free(tmp);

  AMX_assert(strlen(result) <= new_len);

  return result;
}

/* ------------------------------------------------------------------------------------ 
 *  spawn jobs on local machine
 * ------------------------------------------------------------------------------------ */
extern int AMUDP_SPMDLocalSpawn(int nproc, int argc, char **argv, char **extra_env) {
  /* just a simple fork/exec */
  int i;

  if (!AMUDP_SPMDSpawnRunning) {
    AMX_Err("Spawn functions should never be run directly - only passed to AMUDP_SPMDStartup()"); 
    return FALSE;
  }

  /* Temporarily assume values from extra_env[] (which we modify in-place) */
  char **save_env = NULL;
  int envc = 0;
  if (extra_env && extra_env[0]) {
    for (envc=0; extra_env[envc]; ++envc) {/*empty*/}
    save_env = (char **)AMX_malloc(sizeof(char *)*envc);
    for (i=0;i<envc;++i) {
      char *var = extra_env[i];
      char *delim = strchr(var,'=');
      AMX_assert(delim);
      *delim = '\0';
      save_env[i] = getenv(var);
      setenv(var,delim+1,1);
    }
  }

  for (i = 0; i < nproc; i++) {
    #if PLATFORM_ARCH_CRAYX1
      { char **nargv = (char **)AMX_malloc(sizeof(char *)*(argc+2));
        nargv[0] = argv[0];
        memcpy(nargv+1,argv,argc*sizeof(char *));
        nargv[argc+1] = NULL;
        if (execsp(nargv, environ, NULL) == -1)
          AMX_FatalErr("failed execsp()");
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
        execvp(argv[0], argv);

        /*  if execv returns, an error occurred */
        perror("execvp");
        _exit(1); /*  use _exit() to prevent corrupting parent's io buffers */
      } /*  child */
    #endif
  }

  /* Restore saved environment var(s) */
  for (i=0;i<envc;++i) {
    char *name = extra_env[i];
    if (save_env[i]) {
      setenv(name,save_env[i],1);
    } else {
      unsetenv(name);
    }
    /* Revert our in-place modification of extra_env[]: */
    name[strlen(name)] = '=';
  }
  AMX_free(save_env);

  return TRUE;
}
/* ------------------------------------------------------------------------------------ 
 *  spawn jobs using ssh remote shell
 * ------------------------------------------------------------------------------------ */
/* ssh requires some massaging to provide reasonable behavior for spawning parallel jobs
 * the best way to use this is to setup an identity file in ~/.ssh so you don't get 
 * prompted for passwords. You also need to set the environment variables below
 */
/* this implementation of ssh spawn has the following compile-time constants: */
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
 *  of AMUDP_ or AMX_ENV_PREFIX##_)
 */

int AMUDP_SPMDSshSpawn(int nproc, int argc, char **argv, char **extra_env) {
  int i;
  const char *ssh_servers;
  const char *ssh_cmd;
  const char *ssh_options;
  const char *ssh_remote_path;
  char cwd[1024];
  const char *p;
  char *cmd1, *cmd2;
  size_t cmd1_sz, cmd2_sz;
  int pid;

  if (!AMUDP_SPMDSpawnRunning) {
    AMX_Err("Spawn functions should never be run directly - only passed to AMUDP_SPMDStartup()"); 
    return FALSE;
  }

  pid = getpid();

  ssh_servers = AMUDP_getenv_prefixed_withdefault("SSH_SERVERS","");
  if (!strlen(ssh_servers)) {
    AMX_Err("Environment variable SSH_SERVERS is missing.");
    return FALSE;
  }


  if (!getcwd(cwd, 1024)) {
    AMX_Err("Error calling getcwd()");
    return FALSE;
  }
  ssh_remote_path = quote_for_local(AMUDP_getenv_prefixed_withdefault("SSH_REMOTE_PATH", cwd));
  ssh_cmd = AMUDP_getenv_prefixed_withdefault("SSH_CMD", "ssh");

  int isOpenSSH = 0; /* figure out if we're using OpenSSH */
  { char cmdtmp[1024];
    sprintf(cmdtmp,"%s -V 2>&1 | grep OpenSSH", ssh_cmd);
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

  p = ssh_servers;
  for (i = 0; i < nproc; i++) { /* check we have enough servers */
    const char *end;
    while (*p && strchr(SSH_SERVERS_DELIM_CHARS, *p)) p++;
    end = p + strcspn(p, SSH_SERVERS_DELIM_CHARS);
    if (p == end) {
      AMX_Err("Not enough machines in environment variable SSH_SERVERS to satisfy request for (%i).\n"
       "Only (%i) machines available: %s", nproc, i, ssh_servers);
      return FALSE;
    }
    if (*end) p = end+1;
    else p = end;
  } 

  const char *envcmd = AMUDP_getenv_prefixed_withdefault("ENV_CMD", "env");

  cmd1_sz = 1; /* terminating nul */
  if (extra_env && extra_env[0]) {
    cmd1_sz += 1 + strlen(envcmd); // "env "
    for (i=0; extra_env[i]; ++i) {
      cmd1_sz += strlen(extra_env[i]) + 3; // "'%s' "
    }
  }
  char **quoted_args = (char**)AMX_malloc(argc * sizeof(char*));
  for (i = 0; i < argc; i++) {
    AMX_assert(argv[i]);
    quoted_args[i] = quote_for_local(argv[i]);
    cmd1_sz += strlen(quoted_args[i]) + 1; // +1 for trailing space
  }
  cmd1 = (char *)AMX_malloc(cmd1_sz);
  { char *tmp = cmd1;
    if (extra_env && extra_env[0]) {
      tmp += sprintf(tmp, "%s ", envcmd);
      for (i=0; extra_env[i]; ++i) {
        tmp += sprintf(tmp, "'%s' ", extra_env[i]);
      }
    }
    for (i = 0; i < argc; i++) {
      AMX_assert(quoted_args[i]);
      tmp += sprintf(tmp, "%s ", quoted_args[i]);
      AMX_free(quoted_args[i]);
    }
    AMX_free(quoted_args);
    AMX_assert(!argv[i]);
    *tmp = '\0';
    AMX_assert(strlen(cmd1) == cmd1_sz - 1);
  }
  cmd2_sz = cmd1_sz + strlen(ssh_remote_path) + 1024; /* estimated */
  cmd2 =  (char *)AMX_malloc(cmd2_sz);

  p = ssh_servers;
  for (i = 0; i < nproc; i++) {
   char ssh_server[255];
   const char *end;
   while (*p && strchr(SSH_SERVERS_DELIM_CHARS, *p)) p++;
   end = p + strcspn(p, SSH_SERVERS_DELIM_CHARS);
   AMX_assert(p != end);
   strncpy(ssh_server, p, (end-p));
   ssh_server[end-p] = '\0'; 
   if (*end) p = end+1;
   else p = end;

   int forkRet = fork(); // fork a new process to hold ssh command

   if (forkRet == -1) {
      perror("fork");
      return FALSE;
   } else if (forkRet != 0) { // parent
      continue;
   } else {  // this is the child
    /* build the ssh command */
    snprintf(cmd2, cmd2_sz, "%s %s%s%s %s \"%scd %s ; %s\" "
      " < /dev/null || kill %i 2>/dev/null",
      ssh_cmd,

      #if SSH_SUPRESSNEWKEYPROMPT
        (isOpenSSH?"-o 'StrictHostKeyChecking no' ":""),
      #else 
        "",
      #endif

      #if SSH_PREVENTRSHFALLBACK
        (isOpenSSH?"-o 'FallBackToRsh no' ":""),
      #else 
        "",
      #endif

      ssh_options, ssh_server,
      
      (AMX_SilentMode?"":"echo connected to \\$HOST... ; "),

      ssh_remote_path, cmd1, pid
    );

    // This is currently written to keep a local shell process alive as a parent for the ssh command,
    // mostly to ensure that ssh connection/authentication failure results in immediate job teardown.
    // Keeping our process tree intact simplifies clean exit synchronization via wait().
    // On modern Linux we could potentially background the ssh process (openssh -f),
    // allow the intermediate shell process to exit, and preserve our process tree using a
    // prctl(PR_SET_CHILD_SUBREAPER) call, but that approach is not portable.
    if (!AMX_SilentMode) AMX_Info("exec(/bin/sh -c '%s')", cmd2);
    int status = execlp("/bin/sh","/bin/sh", "-c", cmd2, NULL);
    AMX_Err("Failed to call exec() to spawn, status=%i, error=%s",status,strerror(errno));
    kill(pid,15);
    _exit(127);
   } // child process
  } 

  AMX_free(cmd1);
  AMX_free(cmd2);
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
 * CSPAWN_ROUTE_OUTPUT 0   set this variable to request stdout/stderr routing of workers
 * (any environment variable may be specified with an optional prefix 
 *  of AMUDP_ or AMX_ENV_PREFIX##_)
 */

int AMUDP_SPMDCustomSpawn(int nproc, int argc, char **argv, char **extra_env) {
  int i;
  char *spawn_cmd = NULL;
  char *spawn_servers = NULL;
  char workerservers[1024];
  char workercmd[1024];
  char nproc_str[10];
  char cwd[1024];
  char cmd[1024];
#if PLATFORM_OS_CYGWIN
  int spawn_use_create_process;
#endif
  int spawn_route_output;
  int pid;

  if (!AMUDP_SPMDSpawnRunning) {
    AMX_Err("Spawn functions should never be run directly - only passed to AMUDP_SPMDStartup()"); 
    return FALSE;
  }

  pid = getpid();

  spawn_cmd = AMUDP_getenv_prefixed_withdefault("CSPAWN_CMD","");
  if (!strlen(spawn_cmd)) {
    AMX_Err("You must set the " AMX_ENV_PREFIX_STR"_CSPAWN_CMD environment variable to use the custom spawn function"); 
    return FALSE;
  }
  spawn_servers = AMUDP_getenv_prefixed_withdefault("CSPAWN_SERVERS","");
  if (!strlen(spawn_servers)) spawn_servers = NULL;
  spawn_route_output = 
    strcmp(AMUDP_getenv_prefixed_withdefault("CSPAWN_ROUTE_OUTPUT","0"),"0");
#if PLATFORM_OS_CYGWIN
  spawn_use_create_process =
    strcmp(AMUDP_getenv_prefixed_withdefault("CSPAWN_USE_CREATE_PROCESS","0"),"0");
#endif


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
        AMX_Err("Not enough machines in environment variable " AMX_ENV_PREFIX_STR"_CSPAWN_SERVERS to satisfy request for (%i).\n"
          "Only (%i) machines available: %s", nproc, i, spawn_servers);
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
    AMX_Err("Error calling getcwd()");
    return FALSE;
  }

    /* build the worker command */
  { char temp[2048]; // TODO: allocate dynamically
    temp[0] = '\0';
    if (extra_env && extra_env[0]) {
      const char *envcmd = AMUDP_getenv_prefixed_withdefault("ENV_CMD", "env");
      strcat(temp,envcmd);
      strcat(temp," ");
      for (i=0; extra_env[i]; ++i) {
        strcat(temp,"'");
        strcat(temp,extra_env[i]);
        strcat(temp,"' ");
      }
    }
    for (i = 0; i < argc; i++) {
      AMX_assert(argv[i] != NULL);
      char *qarg = quote_for_local(argv[i]);
      strcat(temp,qarg);
      strcat(temp," ");
      AMX_free(qarg);
    }
    AMX_assert(!argv[i]);

  #if PLATFORM_OS_CYGWIN
    if (spawn_use_create_process) strcpy(workercmd, temp);
    else
  #endif
   {
    sprintf(workercmd, "/bin/sh -c \"%s%s\" || ( echo \"spawn failed.\" ; kill %i ) ",
      (AMX_SilentMode?"":"echo connected to `uname -n`... ; "),
      temp, pid
    );
   }
  }

  strcpy(cmd, spawn_cmd);
  { char tmp[1024];
    char *p = cmd;
    while ((p = strchr(p, '%'))) {
      const char *replacement;
      switch (*(p+1)) {
        case 'M': case 'm': 
          if (!spawn_servers) { /* user failed to provide servers and now is asking for them */
            AMX_Err("You must set the " AMX_ENV_PREFIX_STR"_CSPAWN_SERVERS environment "
                       "variable to use the %%M option in " AMX_ENV_PREFIX_STR"_CSPAWN_CMD");
          }
          replacement = workerservers; 
          break;
        case 'N': case 'n': replacement = nproc_str; break;
        case 'C': case 'c': replacement = workercmd; break;
        case 'D': case 'd': replacement = cwd; break;
        case 'P': case 'p': replacement = argv[0]; break;
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
    {  /*  this is the child - exec the new process */
      if (!AMX_SilentMode) 
        AMX_Info("system(%s)\n", cmd);
    #if PLATFORM_OS_CYGWIN
      if (spawn_use_create_process) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        DWORD code;

        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        si.hStdError = GetStdHandle(STD_OUTPUT_HANDLE); 
        si.hStdOutput = GetStdHandle(STD_ERROR_HANDLE); 
        si.dwFlags |= STARTF_USESTDHANDLES;

        if (   TRUE != CreateProcess(0, AMX_strdup(cmd), 0, 0, TRUE, 0, 0, 0, &si, &pi)
            || WAIT_FAILED == WaitForSingleObject(pi.hProcess, INFINITE)
            || 0 == GetExitCodeProcess(pi.hProcess, &code)
            || 0 != code)
          AMX_FatalErr("Failed while calling CreateProcess() with custom spawn command:\n%s", cmd);
      } else
    #endif
      if (system(cmd) != 0)
         AMX_FatalErr("Failed while calling system() with custom spawn command:\n%s", cmd);
      exit(0);
    }
  }

  return TRUE;
}
/* ------------------------------------------------------------------------------------ */
