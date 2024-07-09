#ifndef _SYS_USER_H_
#define _SYS_USER_H_

typedef	__int32_t lwpid_t;

struct kinfo_proc {
    pid_t ki_pid;
    int	ki_oncpu;
    int	ki_lastcpu;
    lwpid_t	ki_tid;
};

#endif /* _SYS_USER_H_ */
