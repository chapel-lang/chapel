/* $Id: signaltrap.c,v 1.28 2005-05-13 19:06:40 vinod Exp $ */
 /******************************************************\
 * Signal handler functions for the following signals:  *
 *        SIGINT, SIGCHLD, SIGBUS, SIGFPE, SIGILL,      *
 *        SIGSEGV, SIGSYS, SIGTRAP, SIGHUP, SIGTERM     *
 * Used to call armci_error that frees up IPC resources *
 \******************************************************/


#include <signal.h>
#include <stdio.h>
#ifndef WIN32
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#endif

#define PAUSE_ON_ERROR__

#define  Error armci_die 

#if (defined(ENCORE) || defined(SEQUENT) || defined(ARDENT))
#   define SigType  int
#else
#   define SigType  void
#endif

#ifndef SIG_ERR
#   define SIG_ERR         (SigType (*)())-1
#endif

#if defined(SUN) || defined(ALLIANT) || defined(ENCORE) || defined(SEQUENT) || \
    defined(AIX) || defined(NEXT)
#include <sys/wait.h>
#endif

extern void Error();
extern int armci_me;

int AR_caught_sigint=0;
int AR_caught_sigterm=0;
int AR_caught_sigchld=0;
int AR_caught_sigsegv=0;
int AR_caught_sig=0;

SigType (*SigChldOrig)(), (*SigIntOrig)(), (*SigHupOrig)(), (*SigTermOrig)();
SigType (*SigSegvOrig)();


/*********************** SIGINT *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigIntHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigIntHandler(sig)
#endif
     int sig;
{
  AR_caught_sigint = 1;
  AR_caught_sig= sig;
  Error("SigIntHandler: interrupt signal was caught",(int) sig);
}

void TrapSigInt()
/*
  Trap the signal SIGINT so that we can propagate error
  conditions and also tidy up shared system resources in a
  manner not possible just by killing everyone
*/
{
  if ( (SigIntOrig = signal(SIGINT, SigIntHandler)) == SIG_ERR)
    Error("TrapSigInt: error from signal setting SIGINT",0);
}

void RestoreSigInt()
/*
 Restore the original signal handler
*/
{
  if(AR_caught_sigint) SigIntOrig(SIGINT);
  if ( signal(SIGINT, SigIntOrig) == SIG_ERR)
    Error("RestoreSigInt: error from restoring signal SIGINT",0);
}


/*********************** SIGABORT *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigAbortHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigAbortHandler(sig)
#endif
     int sig;
{
  AR_caught_sig= sig;
  Error("SigIntHandler: abort signal was caught: cleaning up",(int) sig);
}

void TrapSigAbort()
/*
  Trap the signal SIGINT so that we can propagate error
  conditions and also tidy up shared system resources in a
  manner not possible just by killing everyone
*/
{
  if (  signal(SIGINT, SigAbortHandler) == SIG_ERR)
    Error("TrapSigAbort: error from signal setting SIGABORT",0);
}



/*********************** SIGCHLD *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigChldHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigChldHandler(sig)
#endif
     int sig;
{
  int status;
#if defined(ALLIANT) || defined(ENCORE) || defined(SEQUENT) || defined(NEXT)
  union wait ustatus;
#endif
  
#if defined(LINUX)
  pid_t ret;
  /* Trap signal as soon as possible to avoid race */
  if ( (SigChldOrig = signal(SIGCHLD, SigChldHandler)) == SIG_ERR)
    Error("SigChldHandler: error from signal setting SIGCHLD",0);
#endif

#if defined(ALLIANT) || defined(ENCORE) || defined(SEQUENT) || defined(NEXT)

# if defined(LINUX)
  ret = wait(&ustatus);
  if((ret == 0) || ((ret == -1) && (errno == ECHILD))) { return; }
# else
  (void) wait(&ustatus); 
# endif  
  status = ustatus.w_status;

#else

# if defined(LINUX)
  ret = waitpid(0, &status, WNOHANG);
  if((ret == 0) || ((ret == -1) && (errno == ECHILD))) { return; }
# else
  (void)wait(&status);
# endif

#endif
      AR_caught_sigchld=1;
      AR_caught_sig= sig;
      Error("Child process terminated prematurely, status=",(int) status);
}

void TrapSigChld()
/*
  Trap SIGCHLD so that can tell if children die unexpectedly.
*/
{
  if ( (SigChldOrig = signal(SIGCHLD, SigChldHandler)) == SIG_ERR)
    Error("TrapSigChld: error from signal setting SIGCHLD",0);
}


void RestoreSigChld()
/*
 Restore the original signal handler
*/
{
  if(AR_caught_sigchld) SigChldOrig(SIGCHLD);
  if ( signal(SIGCHLD, SigChldOrig) == SIG_ERR)
    Error("RestoreSigChld: error from restoring signal SIGChld",0);
}


void RestoreSigChldDfl()
{
(void) signal(SIGCHLD, SIG_DFL);
}


/*********************** SIGBUS *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigBusHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigBusHandler(sig)
#endif
     int sig;
{
  AR_caught_sig= sig;
#ifdef PAUSE_ON_ERROR
  fprintf(stderr,"%d(%d): Bus Error ... pausing\n",
          armci_me, getpid() );pause();
#endif
  Error("Bus error, status=",(int) sig);
}

void TrapSigBus()
/*
  Trap SIGBUS 
*/
{
  if ( signal(SIGBUS, SigBusHandler) == SIG_ERR)
    Error("TrapSigBus: error from signal setting SIGBUS", 0);
}




/*********************** SIGFPE *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigFpeHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigFpeHandler(sig)
#endif
     int sig;
{
  AR_caught_sig= sig;
  Error("Floating Point Exception error, status=",(int) sig);
}

void TrapSigFpe()
/*
  Trap SIGFPE
*/
{
  if ( signal(SIGFPE, SigFpeHandler) == SIG_ERR)
    Error("TrapSigFpe: error from signal setting SIGFPE", 0);
}




/*********************** SIGILL *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigIllHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigIllHandler(sig)
#endif
     int sig;
{
  AR_caught_sig= sig;
  Error("Illegal Instruction error, status=",(int) sig);
}

void TrapSigIll()
/*
  Trap SIGILL
*/
{
  if ( signal(SIGILL, SigIllHandler) == SIG_ERR)
    Error("TrapSigIll: error from signal setting SIGILL", 0);
}




/*********************** SIGSEGV *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigSegvHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigSegvHandler(sig)
#endif
     int sig;
{
  AR_caught_sig= sig;
  AR_caught_sigsegv=1;
#ifdef PAUSE_ON_ERROR
  fprintf(stderr,"%d(%d): Segmentation Violation ... pausing\n",
          armci_me, getpid() );pause(); 
#endif

  Error("Segmentation Violation error, status=",(int) sig);
}
#ifdef DO_CKPT
static void * signal_arr[100];
SigType SigSegvActionSa(int sig,siginfo_t *sinfo, void *ptr)
{
  int (*func)();      
  AR_caught_sig= sig;
  AR_caught_sigsegv=1;
  func = signal_arr[sig];
  /*printf("\n%d:in sigaction %p, %d\n",armci_me,sinfo->si_addr,sinfo->si_errno);fflush(stdout);*/

  if(func(sinfo->si_addr,sinfo->si_errno,sinfo->si_fd))
     Error("Segmentation Violation error, status=",(int) SIGSEGV);
}

void TrapSigSegvSigaction()
{
  struct sigaction sa;
    sa.sa_sigaction = (void *)SigSegvActionSa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGSEGV, &sa, NULL);
}
#endif

void TrapSigSegv()
/*
  Trap SIGSEGV
*/
{
  if ( (SigSegvOrig=signal(SIGSEGV, SigSegvHandler)) == SIG_ERR)
    Error("TrapSigSegv: error from signal setting SIGSEGV", 0);
}


void RestoreSigSegv()
/*
 Restore the original signal handler
*/
{
/*
  if(AR_caught_sigsegv) SigSegvOrig(SIGSEGV);
*/
#ifdef DO_CKPT__
  struct sigaction sa;
  sa.sa_handler = (void *)SigSegvOrig;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction(SIGSEGV, &sa, NULL);
  sigaction(SIGSEGV,&sa,NULL);
#else
  if ( signal(SIGSEGV,SigSegvOrig) == SIG_ERR)
    Error("RestoreSigSegv: error from restoring signal SIGSEGV",0);
#endif
}


/*********************** SIGSYS *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigSysHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigSysHandler(sig)
#endif
     int sig;
{
  AR_caught_sig= sig;
  Error("Bad Argument To System Call error, status=",(int) sig);
}

void TrapSigSys()
/*
  Trap SIGSYS
*/
{
#ifndef LINUX
  if ( signal(SIGSYS, SigSysHandler) == SIG_ERR)
    Error("TrapSigSys: error from signal setting SIGSYS", 0);
#endif
}



/*********************** SIGTRAP *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigTrapHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigTrapHandler(sig)
#endif
     int sig;
{
  AR_caught_sig= sig;
  Error("Trace Trap error, status=",(int) sig);
}

void TrapSigTrap()
/*
  Trap SIGTRAP
*/
{
  if ( signal(SIGTRAP, SigTrapHandler) == SIG_ERR)
    Error("TrapSigTrap: error from signal setting SIGTRAP", 0);
}



/*********************** SIGHUP *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigHupHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigHupHandler(sig)
#endif
     int sig;
{
  AR_caught_sig= sig;
  Error("Hangup error, status=",(int) sig);
}

void TrapSigHup()
/*
  Trap SIGHUP
*/
{
  if ( (SigHupOrig = signal(SIGHUP, SigHupHandler)) == SIG_ERR)
    Error("TrapSigHup: error from signal setting SIGHUP", 0);
}


void RestoreSigHup()
/*
 Restore the original signal handler
*/
{
  if(AR_caught_sig== SIGHUP) SigHupOrig(SIGHUP);
  if ( signal(SIGHUP, SigHupOrig) == SIG_ERR)
    Error("RestoreSigHUP: error from restoring signal SIGHUP",0);
}



/*********************** SIGTERM *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigTermHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigTermHandler(sig)
#endif
     int sig;
{
  AR_caught_sigterm = 1;
  AR_caught_sig= sig;
  Error("Terminate signal was sent, status=",(int) sig);
}

void TrapSigTerm()
/*
  Trap SIGTERM
*/
{
  if ( (SigTermOrig = signal(SIGTERM, SigTermHandler)) == SIG_ERR)
    Error("TrapSigTerm: error from signal setting SIGTERM", 0);
}

void RestoreSigTerm()
/*
 Restore the original signal handler
*/
{
  if(AR_caught_sigterm && (SigTermOrig != SIG_DFL) ) SigTermOrig(SIGTERM);
  if ( signal(SIGTERM, SigTermOrig) == SIG_ERR)
    Error("RestoreSigTerm: error from restoring signal SIGTerm",0);
}


/*********************** SIGIOT *************************************/
#ifdef SIGIOT
#if defined(SUN) && !defined(SOLARIS)
SigType SigIotHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigIotHandler(sig)
#endif
     int sig;
{
  AR_caught_sig= sig;
  Error("IOT signal was sent, status=",(int) sig);
}

void TrapSigIot()
/*
  Trap SIGIOT
*/
{
      if ( signal(SIGIOT, SigIotHandler) == SIG_ERR)
          Error("TrapSigIot: error from signal setting SIGIOT", 0);
}
#endif



/*********************** SIGCONT *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigContHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigContHandler(sig)
#endif
     int sig;
{
/*  Error("Trace Cont error, status=",(int) sig);*/
  AR_caught_sig= sig;
}

void TrapSigCont()
/*
  Trap SIGCONT
*/
{
  if ( signal(SIGCONT, SigContHandler) == SIG_ERR)
    Error("TrapSigCont: error from signal setting SIGCONT", 0);
}

/*********************** SIGXCPU *************************************/
#if defined(SUN) && !defined(SOLARIS)
SigType SigXcpuHandler(sig, code, scp, addr)
     int code;
     struct sigcontext *scp;
     char *addr;
#else
SigType SigXcpuHandler(sig)
#endif
     int sig;
{
  AR_caught_sig= sig;
  Error("Terminate signal was sent, status=",(int) sig);
}

void TrapSigXcpu()
/*
  Trap SIGXCPU
*/
{
  if ( signal(SIGXCPU, SigXcpuHandler) == SIG_ERR)
    Error("TrapSigXcpu: error from signal setting SIGXCPU", 0);
}

/******************* external API *********************************/

void ARMCI_ChildrenTrapSignals()
{
#ifndef LAPI
     TrapSigBus();
#endif
     TrapSigFpe();
     TrapSigIll();
#ifdef DO_CKPT
     TrapSigSegvSigaction();
#else
     TrapSigSegv(); 
#endif
     TrapSigSys();
     TrapSigTrap();
     TrapSigAbort();
     TrapSigTerm();
     TrapSigInt();

#if defined(LAPI) || defined(SGI)
     TrapSigIot();
#endif

#ifdef SGI
     TrapSigXcpu();
#endif

}


void ARMCI_ParentTrapSignals()
{
#ifndef LAPI
     TrapSigChld();
#endif
     TrapSigHup();
}


void ARMCI_RestoreSignals()
{
     RestoreSigTerm();
     RestoreSigInt();
     RestoreSigSegv();
}


void ARMCI_ParentRestoreSignals()
{
#ifndef LAPI
     RestoreSigChld();
#endif
     ARMCI_RestoreSignals();
     RestoreSigHup();
}

#ifdef DO_CKPT
/*user can register a function with 3 parameters, 1st offending address
 * 2nd err number and third file descriptor*/
void ARMCI_Register_Signal_Handler(int sig, void  (*func)())
{
    signal_arr[sig]=func;
}
#endif
