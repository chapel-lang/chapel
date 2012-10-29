#ifndef MYTH_IO_FUNC_H_
#define MYTH_IO_FUNC_H_

#ifdef MYTH_WRAP_SOCKIO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>

#include "myth_wsqueue.h"
#include "myth_worker.h"
#include "myth_io.h"
#include "myth_sched_proto.h"
#include "myth_wsqueue_proto.h"

#include "myth_io_proto.h"

#include "myth_io_struct.h"

#include "myth_sched_func.h"

// Workaround for FX10 glibc bug
#ifndef EPOLLRDHUP
#define EPOLLRDHUP 0x2000
#endif

#define MYTH_IO_SIGNAL_NO SIGRTMIN

#define MYTH_IO_EPOLL_FLAG (EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLET)

static inline void myth_io_init(void)
{
	g_fd_map=myth_fd_map_init();
}

#ifdef MYTH_USE_SIGIO
static void myth_io_sighandler(int signum,siginfo_t *sinfo,void* ctx);
#endif

#ifdef MYTH_USE_IO_THREAD
static inline myth_thread_t myth_io_polling_thread(struct myth_running_env *env);
static void *myth_io_thread_func(void* args)
{
	myth_running_env_t env;
	myth_io_struct_perenv_t io;
	env=(myth_running_env_t)args;
	io=&env->io_struct;
	//change scheduling policy
	struct sched_param param;
	param.sched_priority=1;
	if (pthread_setschedparam(real_pthread_self(),SCHED_RR,&param)!=0){
		perror(NULL);
	}
	while (!io->exit_flag)
	{
		myth_io_polling_thread(env);
	}
	return NULL;
}
#endif

static void myth_io_worker_init(myth_running_env_t env,myth_io_struct_perenv_t io)
{
	//Each worker thread has an epoll instance
	io->epfd=epoll_create(MYTH_EPOLL_SIZE);
	io->epnum=0;
	io->fd_map=g_fd_map;
	io->fd_count=0;
	io->cs_flag=0;
	io->sig_count=0;
#ifdef MYTH_USE_SIGIO
	struct sigaction newact;
	memset(&newact,0,sizeof(newact));
	newact.sa_sigaction=myth_io_sighandler;
	newact.sa_flags=SA_SIGINFO;
	sigaction(MYTH_IO_SIGNAL_NO,&newact,NULL);
#endif
#ifdef MYTH_USE_IO_THREAD
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_ERRORCHECK);
	real_pthread_mutex_init(&io->mtx,&attr);
	io->exit_flag=0;
	real_pthread_create(&io->thread,NULL,myth_io_thread_func,(void*)env);
	pthread_mutexattr_destroy(&attr);
#endif
	myth_io_fd_list_init(env,&io->close_list);
	myth_io_wait_list_init(&io->rd_reserve_list);
	myth_io_wait_list_init(&io->wr_reserve_list);
}

static void myth_io_worker_fini(myth_running_env_t env,myth_io_struct_perenv_t io)
{
#ifdef MYTH_USE_IO_THREAD
	io->exit_flag=1;
	real_pthread_join(io->thread,NULL);
#endif
	real_close(io->epfd);
	myth_io_fd_list_destroy(env,&io->close_list);
}

static inline void myth_io_fini(void)
{
	myth_fd_map_destroy(g_fd_map);
}

MYTH_CTX_CALLBACK void myth_wait_for_read_1(void *arg1,void *arg2,void *arg3)
{
	//Add current thread to blocked list after context switch finishes
	myth_running_env_t env;
	myth_io_op_t op;
	myth_io_struct_perfd_t fd_data;
	myth_io_wait_list_t wl;
	env=arg1;
	op=arg2;
	fd_data=arg3;
	if (env==fd_data->env)wl=&fd_data->rd_list;
	else wl=fd_data->rd_reserve_list_ptr;
	myth_io_wait_list_push(env,wl,op);
	myth_io_cs_exit(env);
	//myth_io_fd_list_push(env,&fd_data->env->io_struct.notify_list,fd_data);
}

static inline void myth_wait_for_read(int fd,myth_running_env_t env,myth_io_op_t op)
{
	//Add current thread to blocked list
	myth_fd_map_t fd_map;
	myth_io_struct_perfd_t fd_data;
	myth_io_wait_list_t wl;
	fd_map=env->io_struct.fd_map;
	//Lookup blocked list from fd_map
	fd_data=myth_fd_map_lookup(fd_map,fd);
	if (!fd_data){
		while (1){
			myth_yield_body(0);
			if (myth_io_execute(op))break;
		}
		return;
	}
	wl=&(fd_data->rd_list);
	op->th=env->this_thread;
	op->wl_ptr=wl;
	//Switch to next thread
	myth_thread_t this_thread,next;
	this_thread=op->th;
	next=myth_queue_pop(&env->runnable_q);
	if (next){
		assert(next->status==MYTH_STATUS_READY);//sanity check
		//Switch context
		next->env=env;
		env->this_thread=next;
		myth_swap_context_withcall(&this_thread->context,&next->context,
				myth_wait_for_read_1,(void*)env,(void*)op,(void*)fd_data);
	}
	else{
		//Switch to scheduler for work stealing
		myth_swap_context_withcall(&this_thread->context,&env->sched.context,
				myth_wait_for_read_1,(void*)env,(void*)op,(void*)fd_data);
	}
}

MYTH_CTX_CALLBACK void myth_wait_for_write_1(void *arg1,void *arg2,void *arg3)
{
	//Add current thread to blocked list after context switch finishes
	myth_running_env_t env;
	myth_io_op_t op;
	myth_io_struct_perfd_t fd_data;
	myth_io_wait_list_t wl;
	env=arg1;
	op=arg2;
	fd_data=arg3;
	if (env==fd_data->env)wl=&fd_data->wr_list;
	else wl=fd_data->wr_reserve_list_ptr;
	myth_io_wait_list_push(env,wl,op);
	myth_io_cs_exit(env);
	//myth_io_fd_list_push(env,&fd_data->env->io_struct.notify_list,fd_data);
}

static inline void myth_wait_for_write(int fd,myth_running_env_t env,myth_io_op_t op)
{
	//Add current thread to blocked list
	myth_fd_map_t fd_map;
	myth_io_struct_perfd_t fd_data;
	myth_io_wait_list_t wl;
	fd_map=env->io_struct.fd_map;
	assert(fd_map);
	//Lookup blocked list from fd_map
	fd_data=myth_fd_map_lookup(fd_map,fd);
	if (!fd_data){
		while (1){
			myth_yield_body(0);
			if (myth_io_execute(op))break;
		}
		return;
	}
	wl=&(fd_data->wr_list);
	assert(wl);
	op->th=env->this_thread;
	op->wl_ptr=wl;
	//Switch to next thread
	myth_thread_t this_thread,next;
	this_thread=op->th;
	next=myth_queue_pop(&env->runnable_q);
	if (next){
		assert(next->status==MYTH_STATUS_READY);//sanity check
		//Switch context
		next->env=env;
		env->this_thread=next;
		myth_swap_context_withcall(&this_thread->context,&next->context,
				myth_wait_for_write_1,(void*)env,(void*)op,(void*)fd_data);
	}
	else{
		//Switch to scheduler for work stealing
		myth_swap_context_withcall(&this_thread->context,&env->sched.context,
				myth_wait_for_write_1,(void*)env,(void*)op,(void*)fd_data);
	}
}

extern myth_running_env_t g_envs;
extern int g_sched_prof;
extern int g_worker_thread_num;

static inline void myth_io_register_fd(int fd)
{
	//register a file descriptor to the epoll instance in a worker thread
	myth_running_env_t env,w_env;
	myth_io_struct_perfd *fd_data;
	struct epoll_event ee;
	env=myth_get_current_env();
	memset(&ee,0,sizeof(ee));
	fd_data=myth_flmalloc(env->rank,sizeof(myth_io_struct_perfd));
	//fd_data->fd=sock;
	myth_io_wait_list_init(&fd_data->rd_list);
	myth_io_wait_list_init(&fd_data->wr_list);
	fd_data->closed=0;fd_data->fd=fd;
	fd_data->tmp_for_poll=0;
	myth_fd_map_set(env,env->io_struct.fd_map,fd,fd_data);
	ee.events=MYTH_IO_EPOLL_FLAG;
	ee.data.ptr=fd_data;
#ifdef MYTH_ONE_IO_WORKER
	//Register all the file descriptors to worker thread #0
	w_env=&g_envs[0];
#elif defined MYTH_RANDOM_IO_WORKER
	//The worker thread is chosen randomly
	{
		int worker_id;
		worker_id=myth_random(0,g_worker_thread_num);
		w_env=&g_envs[worker_id];
	}
#else
	//Register to the worker thread that issued the call
	w_env=env;
#endif
	fd_data->env=w_env;
	fd_data->rd_reserve_list_ptr=&(w_env->io_struct.rd_reserve_list);
	fd_data->wr_reserve_list_ptr=&(w_env->io_struct.wr_reserve_list);
	epoll_ctl(w_env->io_struct.epfd,EPOLL_CTL_ADD,fd,&ee);
	//g_envs[worker_id].io_struct.fd_count++;
	__sync_fetch_and_add(&(w_env->io_struct.fd_count),1);
	//Set to non-blocking mode
	long fl;
	int ret;
	fl=real_fcntl(fd,F_GETFL);assert(fl!=-1);
#ifdef MYTH_USE_SIGIO
	//Set to send signals
	ret=real_fcntl(fd,F_SETSIG,MYTH_IO_SIGNAL_NO);assert(ret!=-1);
#if 0
	struct f_owner_ex owner;
	owner.type=F_OWNER_TID;
	owner.pid=w_env->tid;
	ret=real_fcntl(fd,F_SETOWN_EX,&owner);assert(ret!=-1);
#else
	ret=real_fcntl(fd,F_SETOWN,getpid());assert(ret!=-1);
#endif
	ret=real_fcntl(fd,F_SETFL,fl | O_NONBLOCK | O_ASYNC);assert(ret!=-1);
#else
	//Do not use sinal
	ret=real_fcntl(fd,F_SETFL,fl | O_NONBLOCK);assert(ret!=-1);
#endif
}

static inline int myth_socket_body (int domain, int type, int protocol)
{
	int sock;
	//TODO:Handle SOCK_NONBLOCK option(after 2.6.27)
	sock=real_socket(domain,type,protocol);
	if (sock==-1)return -1;
	//Register a new file descriptor
	myth_io_register_fd(sock);
	return sock;
}

static inline int myth_connect_body (int fd, const struct sockaddr *addr, socklen_t len)
{
	//Cnrrently connect() is not multiplexed
	int ret;
	socklen_t ret_len;
	int errcode;
	ret=real_connect(fd,addr,len);
	if (ret==-1){
		MAY_BE_UNUSED myth_running_env_t env;
		env=myth_get_current_env();
		if (errno!=EINPROGRESS)return -1;
		//Connection is in progress, wait for write ready
#if 0
		myth_io_op op;
		op.type=MYTH_IO_CONNECT;
		op.u.c.fd=fd;
		myth_wait_for_write(fd,env,&op);
		errno=op.errcode;
		return op.ret;
#else
		struct pollfd pf;
		pf.fd=fd;
		pf.events=POLLOUT;
		pf.revents=0;
		MAY_BE_UNUSED int ready;
		ready=poll(&pf,1,-1);
		myth_assert(ready==1);
		myth_assert(pf.revents & POLLOUT);
		ret_len=sizeof(int);
		getsockopt(fd,SOL_SOCKET,SO_ERROR,&errcode,&ret_len);
		errno=errcode;
		ret=(errcode==0)?0:-1;
#endif
	}
	return ret;
}
static inline int myth_accept_body (int fd, struct sockaddr* addr,
		   socklen_t * addr_len)
{
	//fprintf(stderr,"accept\n");
	int sock;
	myth_running_env_t env;
	env=myth_get_current_env();
	//try to accept
	myth_io_cs_enter(env);
	sock=real_accept(fd,addr,addr_len);
	if (sock==-1){
		myth_io_op op;
		if (errno!=EAGAIN && errno!=EWOULDBLOCK){
			//error
			myth_io_cs_exit(env);
			return -1;
		}
		//Wait for the socket ready to read
		op.type=MYTH_IO_ACCEPT;
		op.u.a.addr=addr;
		op.u.a.fd=fd;
		op.u.a.len=addr_len;
		//fprintf(stderr,"%d %p\n",fd,env);
		myth_wait_for_read(fd,env,&op);
		//fprintf(stderr,"%d %p\n",fd,myth_get_current_env());
		if (op.ret==-1){
			errno=op.errcode;
			return -1;
		}
		sock=op.ret;
	}
	else{myth_io_cs_exit(env);}
	//Done
	myth_io_register_fd(sock);
	return sock;
}

static inline int myth_bind_body(int fd, const struct sockaddr *addr, socklen_t len)
{
	//Have to do nothing extra
	return real_bind(fd,addr,len);
}

static inline int myth_listen_body (int fd, int n)
{
	//Have to do nothing extra
	return real_listen(fd,n);
}

static inline int myth_select_body(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout)
{
	//get current time
	struct timeval tv={0,0};
	int64_t t_start,t_goal,t_current,t_rest;
	t_current=t_start=tv.tv_sec*1000*1000+tv.tv_usec;
	if (timeout){
		gettimeofday(&tv,NULL);
		t_goal=t_start+timeout->tv_sec*1000*1000+timeout->tv_usec;
	}
	else t_goal=0;
	int ret=0;
	struct timeval tv_immediate;
	while (1){
		tv_immediate.tv_sec=0;tv_immediate.tv_usec=0;
		ret=real_select(nfds,readfds,writefds,exceptfds,&tv_immediate);
#ifdef SELECT_ALWAYS_RETURN_IMMEDIATELY
		if (ret!=0)myth_yield_body(0);
		break;
#endif
		if (ret!=0)break;
		if (timeout){
			int errno_save=errno;
			gettimeofday(&tv,NULL);
			errno=errno_save;
			t_current=tv.tv_sec*1000*1000+tv.tv_usec;
			if (t_goal<=t_current)break;
		}
		myth_yield_body(0);
	}
	if (timeout){
		//rewrite timeval
		t_rest=t_goal-t_current;
		if (t_rest<0)t_rest=0;
		timeout->tv_sec=t_rest/(1000*1000);
		timeout->tv_usec=t_rest%(1000*1000);
	}
	return ret;
}

static inline ssize_t myth_sendto_body(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
	MAY_BE_UNUSED uint64_t t0,t1,t2,t3;
	ssize_t ret;
	t0=0;t1=0;t2=0;t3=0;
	myth_running_env_t env;
#ifdef MYTH_IO_PROF_DETAIL
	t0=myth_get_rdtsc();
#endif
	env=myth_get_current_env();
	myth_io_cs_enter(env);
	//Perform non-blocking send
	ret=real_sendto(sockfd,buf,len,flags,dest_addr,addrlen);
#ifdef MYTH_IO_PROF_DETAIL
	t1=myth_get_rdtsc();
#endif
	if (ret==-1){
		myth_io_op op;
		if (errno!=EAGAIN && errno!=EWOULDBLOCK){
			//error
			myth_io_cs_exit(env);
			return -1;
		}
		//Wait for I/O ready
#ifdef MYTH_IO_PROF_DETAIL
		t2=myth_get_rdtsc();
#endif
		op.type=MYTH_IO_SENDTO;
		op.u.st.fd=sockfd;
		op.u.st.buf=buf;
		op.u.st.n=len;
		op.u.st.flags=flags;
		op.u.st.addr=dest_addr;
		op.u.st.addr_len=addrlen;
#ifdef MYTH_IO_PROF_DETAIL
		t3=myth_get_rdtsc();
		env->prof_data.io_block_send_cycles+=t1-t0;
		env->prof_data.io_block_send_cnt++;
#endif
		myth_wait_for_write(sockfd,env,&op);
		errno=op.errcode;
		ret=op.ret;
	}
	else{
		myth_io_cs_exit(env);
#ifdef MYTH_IO_PROF_DETAIL
		env->prof_data.io_succ_send_cycles+=t1-t0;
		env->prof_data.io_succ_send_cnt++;
#endif
	}
	//Done
	return ret;
}

static inline ssize_t myth_recvfrom_body(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
	MAY_BE_UNUSED uint64_t t0,t1,t2,t3;
	ssize_t ret;
	t0=0;t1=0;t2=0;t3=0;
#ifdef MYTH_IO_PROF_DETAIL
	t0=myth_get_rdtsc();
#endif
	myth_running_env_t env;
	env=myth_get_current_env();
	myth_io_cs_enter(env);
	//Perform non-blocking recv
	ret=real_recvfrom(sockfd,buf,len,flags,src_addr,addrlen);
#ifdef MYTH_IO_PROF_DETAIL
	t1=myth_get_rdtsc();
#endif
	if (ret==-1){
		myth_io_op op;
		if (errno!=EAGAIN && errno!=EWOULDBLOCK){
			//error
			myth_io_cs_exit(env);
			return -1;
		}
#ifdef MYTH_IO_PROF_DETAIL
		t2=myth_get_rdtsc();
#endif
		//Wait for I/O ready
		op.type=MYTH_IO_RECVFROM;
		op.u.rf.fd=sockfd;
		op.u.rf.buf=buf;
		op.u.rf.n=len;
		op.u.rf.flags=flags;
		op.u.rf.addr=src_addr;
		op.u.rf.addr_len=addrlen;
#ifdef MYTH_IO_PROF_DETAIL
		t3=myth_get_rdtsc();
		env->prof_data.io_block_recv_cycles+=t1-t0;
		env->prof_data.io_block_recv_cnt++;
#endif
		myth_wait_for_read(sockfd,env,&op);
		errno=op.errcode;
		ret=op.ret;
	}
	else{
		myth_io_cs_exit(env);
#ifdef MYTH_IO_PROF_DETAIL
		env->prof_data.io_succ_recv_cycles+=t1-t0;
		env->prof_data.io_succ_recv_cnt++;
#endif
	}
	//Done
	return ret;
}

static inline ssize_t myth_send_body (int fd, const void *buf, size_t n, int flags)
{
	MAY_BE_UNUSED uint64_t t0,t1,t2,t3;
	ssize_t ret;
	t0=0;t1=0;t2=0;t3=0;
	myth_running_env_t env;
#ifdef MYTH_IO_PROF_DETAIL
	t0=myth_get_rdtsc();
#endif
	env=myth_get_current_env();
	myth_io_cs_enter(env);
	//Perform non-blocking send
	ret=real_send(fd,buf,n,flags);
#ifdef MYTH_IO_PROF_DETAIL
	t1=myth_get_rdtsc();
#endif
	if (ret==-1){
		myth_io_op op;
		if (errno!=EAGAIN && errno!=EWOULDBLOCK){
			//error
			myth_io_cs_exit(env);
			return -1;
		}
		//Wait for I/O ready
#ifdef MYTH_IO_PROF_DETAIL
		t2=myth_get_rdtsc();
#endif
		op.type=MYTH_IO_SEND;
		op.u.s.fd=fd;
		op.u.s.buf=buf;
		op.u.s.n=n;
		op.u.s.flags=flags;
#ifdef MYTH_IO_PROF_DETAIL
		t3=myth_get_rdtsc();
		env->prof_data.io_block_send_cycles+=t1-t0;
		env->prof_data.io_block_send_cnt++;
#endif
		//fprintf(stderr,"SB\n");
		myth_wait_for_write(fd,env,&op);
		errno=op.errcode;
		ret=op.ret;
	}
	else{
		myth_io_cs_exit(env);
#ifdef MYTH_IO_PROF_DETAIL
		env->prof_data.io_succ_send_cycles+=t1-t0;
		env->prof_data.io_succ_send_cnt++;
#endif
	}
	//Done
	return ret;
}
static inline ssize_t myth_recv_body (int fd, void *buf, size_t n, int flags)
{
	MAY_BE_UNUSED uint64_t t0,t1,t2,t3;
	ssize_t ret;
	t0=0;t1=0;t2=0;t3=0;
#ifdef MYTH_IO_PROF_DETAIL
	t0=myth_get_rdtsc();
#endif
	myth_running_env_t env;
	env=myth_get_current_env();
	myth_io_cs_enter(env);
	//Perform non-blocking recv
	ret=real_recv(fd,buf,n,flags);
#ifdef MYTH_IO_PROF_DETAIL
	t1=myth_get_rdtsc();
#endif
	if (ret==-1){
		myth_io_op op;
		if (errno!=EAGAIN && errno!=EWOULDBLOCK){
			//error
			myth_io_cs_exit(env);
			return -1;
		}
#ifdef MYTH_IO_PROF_DETAIL
		t2=myth_get_rdtsc();
#endif
		//Wait for I/O ready
		op.type=MYTH_IO_RECV;
		op.u.r.fd=fd;
		op.u.r.buf=buf;
		op.u.r.n=n;
		op.u.r.flags=flags;
#ifdef MYTH_IO_PROF_DETAIL
		t3=myth_get_rdtsc();
		env->prof_data.io_block_recv_cycles+=t1-t0;
		env->prof_data.io_block_recv_cnt++;
#endif
		myth_wait_for_read(fd,env,&op);
		errno=op.errcode;
		ret=op.ret;
	}
	else{
		myth_io_cs_exit(env);
#ifdef MYTH_IO_PROF_DETAIL
		env->prof_data.io_succ_recv_cycles+=t1-t0;
		env->prof_data.io_succ_recv_cnt++;
#endif
	}
	//Done
	return ret;
}

static inline int myth_close_body (int fd)
{
	int ret;
	myth_running_env_t env,dest_env;
	env=myth_get_current_env();
	myth_io_struct_perfd_t fd_data;
	fd_data=myth_fd_map_lookup(env->io_struct.fd_map,fd);
	if (fd_data){
		myth_assert(fd_data);
		//Remove entry from fd_map
		myth_fd_map_delete(env->io_struct.fd_map,fd);
		//unregister from epoll instance
		struct epoll_event ee;
		memset(&ee,0,sizeof(ee));
		epoll_ctl(fd_data->env->io_struct.epfd,EPOLL_CTL_DEL,fd,&ee);
		//Close file descriptor
		ret=real_close(fd);
		//Tell the worker thread which manages a fd to close data structures
		dest_env=fd_data->env;
		myth_io_fd_list_push(env,&dest_env->io_struct.close_list,fd_data);
	}
	else{
		//simply close if fd is not registered
		ret=real_close(fd);
	}
	return ret;
}
static inline int myth_fcntl_body (int fd, int cmd,va_list vl)
{
	switch (cmd){
	//long
	case F_DUPFD:
	case F_DUPFD_CLOEXEC:
	case F_SETFD:
	case F_SETFL:
	case F_SETOWN:
	case F_SETSIG:
	case F_SETLEASE:
	case F_NOTIFY:
		return real_fcntl(fd,cmd,va_arg(vl,long));
		break;
	//struct flock*
	case F_SETLK:
	case F_SETLKW:
	case F_GETLK:
		return real_fcntl(fd,cmd,va_arg(vl,struct flock*));
		break;
#if 0
	//struct f_owner_ex*
	case F_GETOWN_EX:
	case F_SETOWN_EX:
		return real_fcntl(fd,cmd,va_arg(vl,struct f_owner_ex*));
		break;
#endif
	//No argument required
	case F_GETFD:
	case F_GETFL:
	case F_GETOWN:
	case F_GETSIG:
	case F_GETLEASE:
		return real_fcntl(fd,cmd);
		break;
	}
	//Invalid parameter
	errno=EPERM;
	return -1;
}

#include "myth_io_poll.h"

#endif //MYTH_WRAP_SOCKIO

#endif /* MYTH_IO_FUNC_H_ */
