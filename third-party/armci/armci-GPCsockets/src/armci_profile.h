/* $Id: armci_profile.h,v 1.5 2005-11-30 10:20:53 vinod Exp $ */

enum events {ARMCI_PROF_GET=0, ARMCI_PROF_PUT, ARMCI_PROF_ACC, /* get/put/acc */
	     ARMCI_PROF_GETS, ARMCI_PROF_PUTS, ARMCI_PROF_ACCS, /* strided */
	     ARMCI_PROF_GETV, ARMCI_PROF_PUTV, ARMCI_PROF_ACCV, /* vector */
	     ARMCI_PROF_NBGET, ARMCI_PROF_NBPUT, ARMCI_PROF_NBACC,/*Non-block*/
             ARMCI_PROF_NBGETS, ARMCI_PROF_NBPUTS, ARMCI_PROF_NBACCS,
             ARMCI_PROF_NBGETV, ARMCI_PROF_NBPUTV, ARMCI_PROF_NBACCV,
	     ARMCI_PROF_BARRIER, ARMCI_PROF_WAIT, ARMCI_PROF_NOTIFY, /*misc*/
	     ARMCI_PROF_FENCE, ARMCI_PROF_ALLFENCE, ARMCI_PROF_RMW
};

extern void armci_profile_init();
extern void armci_profile_terminate();
extern void armci_profile_start_strided(int count[], int stride_levels, 
					int proc, int event_type);
extern void armci_profile_stop_strided(int event_type);
extern void armci_profile_start_vector(armci_giov_t darr[], int len, int proc,
				       int event_type);
extern void armci_profile_stop_vector(int event_type);
extern void armci_profile_start(int event_type);
extern void armci_profile_stop(int event_type);
