#include "armci_vampir.h"

void armci_vampir_init() {
    vampir_symdef(ARMCI_INIT,           "ARMCI_Init",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_FINALIZE,       "ARMCI_Finalize",       "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_PUTV,           "ARMCI_PutV",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_NBPUTV,         "ARMCI_NbPutV",         "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_GETV,           "ARMCI_GetV",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_NBGETV,         "ARMCI_NbGetV",         "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_PUTS,           "ARMCI_PutS",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_NBPUTS,         "ARMCI_NbPutS",         "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_GETS,           "ARMCI_GetS",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_NBGETS,         "ARMCI_NbGetS",         "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_WAIT,           "ARMCI_Wait",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_FENCE,          "ARMCI_Fence",          "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_ALLFENCE,       "ARMCI_AllFence",       "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_POLL,           "ARMCI_Poll",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_MALLOC,         "ARMCI_Malloc",         "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_FREE,           "ARMCI_Free",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_CLEANUP,        "ARMCI_Cleanup",        "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_ERROR,          "ARMCI_Error",          "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_RMW,            "ARMCI_Rmw",            "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_LOCK,           "ARMCI_Lock",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_UNLOCK,         "ARMCI_Unlock",         "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_ACCV,           "ARMCI_AccV",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_ACCS,           "ARMCI_AccS",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_COPY,           "ARMCI_Copy",           "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_SET_SHMEM_LIMIT,"ARMCI_Set_shmem_limit","ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_USES_SHM,       "ARMCI_Uses_shm",       "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_CREATE_MUTEXES, "ARMCI_Create_mutexes", "ARMCI",__FILE__,__LINE__);
    vampir_symdef(ARMCI_DESTROY_MUTEXES,"ARMCI_Destroy_mutexes","ARMCI",__FILE__,__LINE__);
}

