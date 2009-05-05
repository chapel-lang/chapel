
/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */

/*
*	pvm_fork.c
*
*	In the spirit of fork(), fork a process and enroll it as a new PVM task.
*	If parent, returns task id (>0) of child, or error (<0).
*	If child, returns 0.
*/


#ifdef	IMA_TITN
#include <sys/socket.h>
#endif
#include <pvm3.h>

int
pvm_fork(cpid)
	int *cpid;		/* cpid returns child pid if nonnull */
{
	int tid;		/* tid to return (child tid or 0) */
	int pid;
	int pfd[2];

	if ((tid = pvm_mytid()) > 0) {
		if (
#ifdef	IMA_TITN
		socketpair(AF_UNIX, SOCK_STREAM, 0, pfd)
#else
		pipe(pfd)
#endif
		== -1)
			tid = PvmOutOfRes;
		else
			if (pid = fork()) {				/* parent */
				close(pfd[1]);
				if (cpid)
					*cpid = pid;
				if (pid == -1)
					tid = PvmOutOfRes;
				else
					if (read(pfd[0], (char *)&tid, sizeof(tid)) != sizeof(tid))
						tid = PvmOutOfRes;
				close(pfd[0]);

			} else {						/* child */
				close(pfd[0]);
				pvmendtask();
				tid = pvm_mytid();
				write(pfd[1], (char *)&tid, sizeof(tid));
				close(pfd[1]);
				tid = 0;
			}
	}
	return tid;
}


