//   $Source: /var/local/cvs/gasnet/other/amudp/sig.cpp,v $
//     $Date: 2006/04/10 04:20:12 $
// $Revision: 1.6 $
// Description: signal handling module
// Copyright 1999, Dan Bonachea

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "sig.h"

//------------------------------------------------------------------------------------
LPSIGHANDLER reghandler(int sigtocatch, LPSIGHANDLER fp) {
  LPSIGHANDLER fpret = signal(sigtocatch, fp); 
  if (fpret == (LPSIGHANDLER)SIG_ERR) {
    printf("Got a SIG_ERR while registering handler for signal %s. Errno = %i\n", 
    sigstr(sigtocatch), errno);
    return NULL;
  }
#ifdef SIG_HOLD
  else if (fpret == (LPSIGHANDLER)SIG_HOLD) {
    printf("Got a SIG_HOLD while registering handler for signal %s(%i).\n", 
    sigstr(sigtocatch), errno);
    return NULL;
  }
#endif
  return fpret;
}
//------------------------------------------------------------------------------------
static struct {
  int sig;
  const char* desc;
  SIGTYPE sigtype; 
  } sigdesctable[] = {
  #ifdef SIGABRT
    {SIGABRT, "SIGABRT: Process abort signal.", ST_PROGRAM_ERROR}, // (abort())
  #endif
  #ifdef SIGFPE
    {SIGFPE,  "SIGFPE: Erroneous arithmetic operation.", ST_PROGRAM_ERROR}, // (FP error)
  #endif
  #ifdef SIGILL
    {SIGILL,  "SIGILL: Illegal instruction.", ST_PROGRAM_ERROR}, // (bad instruction)
  #endif
  #ifdef SIGINT
    {SIGINT,  "SIGINT: Terminal interrupt signal.", ST_TERM_INT}, // (control-c)
  #endif
  #ifdef SIGSEGV
    {SIGSEGV, "SIGSEGV: Invalid memory reference.", ST_PROGRAM_ERROR}, // (seg fault)
  #endif
  #ifdef SIGTERM
    {SIGTERM, "SIGTERM: Termination signal.", ST_SYS_INT}, // (kill command)
  #endif
  #ifdef SIGALRM
    {SIGALRM, "SIGALRM: Alarm clock.", ST_OTHER},
  #endif
  #ifdef SIGHUP
    {SIGHUP,  "SIGHUP: Hangup.", ST_SYS_INT},
  #endif
  #ifdef SIGKILL
    {SIGKILL, "SIGKILL: Kill (cannot be caught or ignored).", ST_FATAL}, // (kill -9 command)
  #endif
  #ifdef SIGPIPE
    {SIGPIPE, "SIGPIPE: Write on a pipe with no one to read it.", ST_OTHER}, // (send() after close)
  #endif
  #ifdef SIGQUIT
    {SIGQUIT, "SIGQUIT: Terminal quit signal.", ST_TERM_INT}, // (control-\)
  #endif
  #ifdef SIGUSR1
    {SIGUSR1, "SIGUSR1: User-defined signal 1.", ST_OTHER},
  #endif
  #ifdef SIGUSR2
    {SIGUSR2, "SIGUSR2: User-defined signal 2.", ST_OTHER},
  #endif
  #ifdef SIGCHLD
    {SIGCHLD, "SIGCHLD: Child process terminated or stopped.", ST_OTHER}, // (sent to parent proc)
  #endif
  #ifdef SIGCONT
    {SIGCONT, "SIGCONT: Continue executing, if stopped.", ST_OTHER}, // (also sent by kill command)
  #endif
  #ifdef SIGSTOP
    {SIGSTOP, "SIGSTOP: Stop executing (cannot be caught or ignored).", ST_FATAL},
  #endif
  #ifdef SIGTSTP
    {SIGTSTP, "SIGTSTP: Terminal stop signal.", ST_TERM_INT}, // (control-z)
  #endif
  #ifdef SIGTTIN
    {SIGTTIN, "SIGTTIN: Background process attempting read.", ST_OTHER},
  #endif
  #ifdef SIGTTOU
    {SIGTTOU, "SIGTTOU: Background process attempting write.", ST_OTHER},
  #endif
  #ifdef SIGBUS
    {SIGBUS,  "SIGBUS: Bus error.", ST_PROGRAM_ERROR}, // (alignment error)
  #endif
  #ifdef SIGPOLL
    {SIGPOLL, "SIGPOLL: Pollable event.", ST_OTHER},
  #endif
  #ifdef SIGXFSZ
    {SIGXFSZ, "SIGXFSZ:  File size limit exceeded.", ST_PROGRAM_ERROR},
  #endif
  #ifdef SIGPROF
    {SIGPROF, "SIGPROF: Profiling timer expired.", ST_OTHER},
  #endif
  #ifdef SIGSYS
    {SIGSYS,  "SIGSYS: Bad system call.", ST_PROGRAM_ERROR},
  #endif
  #ifdef SIGTRAP
    {SIGTRAP, "SIGTRAP: Trace/breakpoint trap.", ST_PROGRAM_ERROR},
  #endif
  #ifdef SIGURG
    {SIGURG,  "SIGURG: High bandwidth data is available at a socket.", ST_OTHER},
  #endif
  #ifdef SIGVTALRM
    {SIGVTALRM,"SIGVTALRM: Virtual timer expired.", ST_OTHER},
  #endif
  #ifdef SIGXCPU
    {SIGXCPU, "SIGXCPU: CPU time limit exceeded.", ST_PROGRAM_ERROR},
  #endif
  #ifdef SIGEMT
    {SIGEMT,     "SIGEMT: Emulation Trap", ST_OTHER},
  #endif
  #ifdef SIGPWR
    {SIGPWR,     "SIGPWR: Power Fail or Restart", ST_OTHER},
  #endif
  #ifdef SIGWINCH
    {SIGWINCH,   "SIGWINCH: Window Size Change", ST_OTHER},
  #endif
  #ifdef SIGWAITING
    {SIGWAITING, "SIGWAITING: Concurrency signal reserved  by threads library", ST_OTHER},
  #endif
  #ifdef SIGLWP
    {SIGLWP,     "SIGLWP: Inter-LWP  signal  reserved  by threads library", ST_OTHER},
  #endif
  #ifdef SIGFREEZE
    {SIGFREEZE,  "SIGFREEZE: Check point Freeze", ST_OTHER},
  #endif
  #ifdef SIGTHAW
    {SIGTHAW,    "SIGTHAW: Check point Thaw", ST_OTHER},
  #endif
  #ifdef SIGCANCEL
    {SIGCANCEL,  "SIGCANCEL: Cancellation signal reserved by threads library", ST_OTHER},
  #endif
// SIGRTMIN ... SIGRTMAX (real-time signals) ignored
    {0, NULL, ST_OTHER}
  };
//------------------------------------------------------------------------------------
const char* sigstr(int sig) {
  for (int i=0; sigdesctable[i].desc; i++) {
    if (sigdesctable[i].sig == sig) return sigdesctable[i].desc;
  }
  return "Unknown Signal";
}
//------------------------------------------------------------------------------------
void regallhandler(LPSIGHANDLER fp, SIGTYPE sigtype) {
  for (int i=0; sigdesctable[i].desc; i++) {
    if (sigdesctable[i].sigtype == sigtype || 
        (sigtype == ST_ALL_CATCHABLE && sigdesctable[i].sigtype != ST_FATAL))
        reghandler(sigdesctable[i].sig, fp);
  }
  return;
}
//------------------------------------------------------------------------------------
SIGTYPE getsigtype(int sig) {
  for (int i=0; sigdesctable[i].desc; i++) {
    if (sigdesctable[i].sig == sig) return sigdesctable[i].sigtype;
  }
  return ST_OTHER;
}
//------------------------------------------------------------------------------------
