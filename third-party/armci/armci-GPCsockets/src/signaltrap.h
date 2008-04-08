#ifndef _SIGNALTRAP_H_
#define _SIGNALTRAP_H_

#ifdef SYSV
  extern void ARMCI_ChildrenTrapSignals();
  extern void ARMCI_ParentTrapSignals();
  extern void ARMCI_ParentRestoreSignals();
  extern void ARMCI_RestoreSignals();
#else
# define ARMCI_ChildrenTrapSignals() 
# define ARMCI_ParentTrapSignals() 
# define ARMCI_ParentRestoreSignals()
#endif

#endif
