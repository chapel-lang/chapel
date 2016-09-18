/* 
 * myth_io_func.h
 */
#pragma once
#ifndef MYTH_IO_FUNC_H_
#define MYTH_IO_FUNC_H_

#if MYTH_WRAP_SOCKIO

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

#include "myth_config.h"
#include "myth_worker.h"
#include "myth_io.h"
#include "myth_init.h"

#include "myth_misc_func.h"
#include "myth_context_func.h"
#include "myth_spinlock_func.h"
#include "myth_wsqueue_func.h"
#include "myth_worker_func.h"

/* used to be in myth_io_proto.h */

#define MYTH_FD_MAP_SIZE 65536
#define MYTH_FD_MAP_KEY(fd) ((fd)%65536)
//#define MYTH_FD_MAP_KEY(fd) 0

//Map structure for searching a blocked list associated with a fd
static inline myth_fd_map_t myth_fd_map_init() {
  myth_fd_map_t ret;
  ret=myth_malloc(sizeof(myth_fd_map));
  ret->entry=myth_malloc(sizeof(myth_fd_map_entry)*MYTH_FD_MAP_SIZE);
  int i;
  for (i=0;i<MYTH_FD_MAP_SIZE;i++){
    ret->entry[i].data=myth_malloc(sizeof(void*));
    ret->entry[i].key=myth_malloc(sizeof(int));
    ret->entry[i].size=0;
    ret->entry[i].bufsize=1;
    myth_spin_init_body(&ret->entry[i].lock);
  }
  //ret->size=0;
  myth_spin_init_body(&ret->lock);
  return ret;
}

static inline void myth_fd_map_set(myth_running_env_t env,myth_fd_map_t fm,int fd,myth_io_struct_perfd_t data) {//Set fd_map to entry
  //Setting NULL as value is forbidden because NULL is returned when lookup fails
  myth_assert(data);
  int i;
  myth_fd_map_entry_t fe;
  myth_spin_lock_body(&fm->lock);
  fe=&fm->entry[MYTH_FD_MAP_KEY(fd)];
  myth_spin_lock_body(&fe->lock);
  for (i=0;i<fe->size;i++){
    if (fe->key[i]==fd){
      fe->data[i]=data;
      myth_spin_unlock_body(&fe->lock);
      myth_spin_unlock_body(&fm->lock);
      return;
    }
  }
  fe->size++;
  if (fe->size>fe->bufsize){
    //If list size is full, reallocate
    int new_bufsize=fe->bufsize*2;
    fe->key=myth_flrealloc(env->rank,sizeof(int)*fe->bufsize,fe->key,sizeof(int)*new_bufsize);
    fe->data=myth_flrealloc(env->rank,sizeof(void*)*fe->bufsize,fe->data,sizeof(void*)*new_bufsize);
    fe->bufsize=new_bufsize;
  }
  fe->key[fe->size-1]=fd;
  fe->data[fe->size-1]=data;
  myth_spin_unlock_body(&fe->lock);
  //fm->size++;
  myth_spin_unlock_body(&fm->lock);
}

static inline void myth_fd_map_delete(myth_fd_map_t fm,int fd) {
  int i,j;
  myth_fd_map_entry_t fe;
  myth_spin_lock_body(&fm->lock);
  fe=&fm->entry[MYTH_FD_MAP_KEY(fd)];
  myth_spin_lock_body(&fe->lock);
  for (i=0;i<fe->size;i++){
    if (fe->key[i]==fd){
      for (j=i+1;j<fe->size;j++){
	fe->key[j-1]=fe->key[j];
	fe->data[j-1]=fe->data[j];
      }
      fe->size--;
      myth_spin_unlock_body(&fe->lock);
      //fm->size--;
      myth_spin_unlock_body(&fm->lock);
      return;
    }
  }
  //simply ignore
  //assert(0);
}

static inline myth_io_struct_perfd_t myth_fd_map_lookup(myth_fd_map_t fm,int fd) {//Lookup blocked list from fd, return NULL if lookup fails
  int i;
  myth_fd_map_entry_t fe;
  myth_spin_lock_body(&fm->lock);
  fe=&fm->entry[MYTH_FD_MAP_KEY(fd)];
  myth_spin_lock_body(&fe->lock);
  for (i=0;i<fe->size;i++){
    if (fe->key[i]==fd){
      assert(fe->data[i]);
      myth_io_struct_perfd_t ret=fe->data[i];
      myth_spin_unlock_body(&fe->lock);
      myth_spin_unlock_body(&fm->lock);
      return ret;
    }
  }
  myth_spin_unlock_body(&fe->lock);
  myth_spin_unlock_body(&fm->lock);
  return NULL;
}

static inline myth_io_struct_perfd_t myth_fd_map_trylookup(myth_fd_map_t fm,int fd) {//Lookup blocked list from fd, return NULL if lookup fails
  int i;
  myth_fd_map_entry_t fe;
  if (!myth_spin_trylock_body(&fm->lock))return NULL;
  fe=&fm->entry[MYTH_FD_MAP_KEY(fd)];
  if (!myth_spin_trylock_body(&fe->lock)){
    myth_spin_unlock_body(&fm->lock);
    return NULL;
  }
  for (i=0;i<fe->size;i++){
    if (fe->key[i]==fd){
      assert(fe->data[i]);
      myth_io_struct_perfd_t ret=fe->data[i];
      myth_spin_unlock_body(&fe->lock);
      myth_spin_unlock_body(&fm->lock);
      return ret;
    }
  }
  myth_spin_unlock_body(&fe->lock);
  myth_spin_unlock_body(&fm->lock);
  return NULL;
}

static inline void myth_fd_map_clear(myth_fd_map_t fm) {//Remove all elements from fd_map
  int i;
  myth_spin_lock_body(&fm->lock);
  for (i=0;i<MYTH_FD_MAP_SIZE;i++){
    myth_fd_map_entry_t fe;
    fe=&fm->entry[i];
    myth_spin_lock_body(&fe->lock);
    fe->size=0;
    myth_spin_unlock_body(&fe->lock);
  }
  //fm->size=0;
  myth_spin_unlock_body(&fm->lock);
}

static inline void myth_fd_map_destroy(myth_fd_map_t fm) {
  int i;
  for (i=0;i<MYTH_FD_MAP_SIZE;i++){
    myth_fd_map_entry_t fe;
    fe=&fm->entry[i];
    myth_spin_destroy(&fe->lock);
    myth_free_with_size(fe->data,0);
    myth_free_with_size(fe->key,0);
  }
  myth_spin_destroy(&fm->lock);
  myth_free_with_size(fm->entry,0);
  myth_free_with_size(fm,0);
}

static inline void myth_io_wait_list_init(myth_io_wait_list_t wl) {
  myth_spin_init_body(&wl->lock);
  wl->count = 0;
  wl->size = 1;
  wl->io_ops = myth_malloc(sizeof(myth_io_op_t)*1);
}

static inline void myth_io_wait_list_destroy(myth_io_wait_list_t wl) {
  myth_spin_destroy(&wl->lock);
  myth_free(wl->io_ops);
}

static inline void myth_io_cs_enter(myth_running_env_t e) {
#if MYTH_USE_IO_THREAD
  int ret;
  ret = real_pthread_mutex_lock(&e->io_struct.mtx);
  if (ret!=0){
    fprintf(stderr,"errno:%d\n",ret);
    errno=ret;
    perror(NULL);
    assert(0);
  }
#else
  (void)e;
#endif
#if MYTH_USE_SIGHANDLER
  assert(e->io_struct.cs_flag==0);
  e->io_struct.cs_flag=1;
  myth_wbarrier();
#endif
}

static inline void myth_io_cs_exit(myth_running_env_t e) {
#if MYTH_USE_SIGHANDLER
  assert(e->io_struct.cs_flag==1);
  myth_wbarrier();
  e->io_struct.cs_flag=0;
#else
  (void)e;
#endif
#if MYTH_USE_IO_THREAD
  real_pthread_mutex_unlock(&e->io_struct.mtx);
#endif
}

static inline int myth_io_is_in_cs(myth_running_env_t e) {
  int ret=0;
#if MYTH_USE_SIGHANDLER
  ret=ret || e->io_struct.cs_flag;
#else
  (void)e;
#endif
#if MYTH_USE_IO_THREAD
#endif
  return ret;
}

static inline void myth_io_wait_list_push(myth_running_env_t e,
					  myth_io_wait_list_t wl, myth_io_op_t op) {
  (void)e;
  myth_spin_lock_body(&wl->lock);
  if (wl->count==wl->size){
    wl->size*=2;
    wl->io_ops=real_realloc(wl->io_ops,sizeof(myth_io_op_t)*wl->size);
  }
  wl->io_ops[wl->count]=op;
  wl->count++;
  myth_spin_unlock_body(&wl->lock);
}

static inline myth_io_op_t myth_io_wait_list_pop(myth_running_env_t e, myth_io_wait_list_t wl) {
  myth_io_op_t ret;
  (void)e;
#if QUICK_CHECK_IO_WAIT_LIST
  if (!wl->count) return NULL;
#endif
  myth_spin_lock_body(&wl->lock);
  if (wl->count){
    ret=wl->io_ops[wl->count-1];
    //if (ret)printf("popped:%p->%p\n",wl,ret);
    wl->count--;
  }
  else ret=NULL;
  myth_spin_unlock_body(&wl->lock);
  //if (ret)printf("unlocked:%p<-%p\n",wl,ret);
  return ret;
}

static inline void myth_io_fd_list_init(myth_running_env_t env,myth_io_fd_list_t cl) {
  cl->data=myth_flmalloc(env->rank,sizeof(myth_io_struct_perfd_t));
  cl->size=0;
  cl->bufsize=1;
  myth_spin_init_body(&cl->lock);
}

static inline void myth_io_fd_list_push(myth_running_env_t env,myth_io_fd_list_t cl,myth_io_struct_perfd_t fd_data) {
  myth_spin_lock_body(&cl->lock);
  cl->size++;
  if (cl->bufsize<cl->size){
    cl->data=myth_flrealloc(env->rank,cl->bufsize*sizeof(myth_io_struct_perfd_t),cl->data,sizeof(myth_io_struct_perfd_t)*cl->size);
    cl->bufsize=cl->size;
  }
  cl->data[cl->size-1]=fd_data;
  myth_spin_unlock_body(&cl->lock);
}

static inline myth_io_struct_perfd_t myth_io_fd_list_pop(myth_io_fd_list_t cl) {
  myth_io_struct_perfd_t ret;
#if QUICK_CHECK_IO_FD_LIST
  if (cl->size<=0) return NULL;
#endif
  myth_spin_lock_body(&cl->lock);
  if (cl->size>0){
    ret=cl->data[cl->size-1];
    cl->size--;
  }
  else ret=NULL;
  myth_spin_unlock_body(&cl->lock);
  return ret;
}

static inline myth_io_struct_perfd_t myth_io_fd_list_trypop(myth_io_fd_list_t cl) {
  myth_io_struct_perfd_t ret;
  if (myth_spin_trylock_body(&cl->lock)==0)return NULL;
  if (cl->size>0){
    ret=cl->data[cl->size-1];
    cl->size--;
  }
  else ret=NULL;
  myth_spin_unlock_body(&cl->lock);
  return ret;
}

static inline void myth_io_fd_list_destroy(myth_running_env_t env,myth_io_fd_list_t cl) {
  myth_flfree(env->rank,cl->bufsize,cl->data);
  cl->size=0;
  cl->bufsize=0;
  myth_spin_destroy(&cl->lock);
}

/* end of myth_io_proto.h */

// Workaround for FX10 glibc bug
#ifndef EPOLLRDHUP
#define EPOLLRDHUP 0x2000
#endif

#define MYTH_IO_SIGNAL_NO SIGRTMIN

#define MYTH_IO_EPOLL_FLAG (EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLET)

static inline void myth_io_init(void) {
  g_fd_map=myth_fd_map_init();
}

#if MYTH_USE_SIGIO
static void myth_io_sighandler(int signum,siginfo_t *sinfo,void* ctx);
#endif

#if MYTH_USE_IO_THREAD
static inline myth_thread_t myth_io_polling_thread(struct myth_running_env *env);
static void *myth_io_thread_func(void* args) {
  myth_running_env_t env;
  myth_io_struct_perenv_t io;
  env=(myth_running_env_t)args;
  io=&env->io_struct;
  //change scheduling policy
  struct sched_param param;
  param.sched_priority=1;
  if (pthread_setschedparam(real_pthread_self(), SCHED_RR, &param)!=0){
    perror(NULL);
  }
  while (!io->exit_flag) {
    myth_io_polling_thread(env);
  }
  return NULL;
}
#endif

static void myth_io_worker_init(myth_running_env_t env,myth_io_struct_perenv_t io) {
  //Each worker thread has an epoll instance
  io->epfd=epoll_create(MYTH_EPOLL_SIZE);
  io->epnum=0;
  io->fd_map=g_fd_map;
  io->fd_count=0;
  io->cs_flag=0;
  io->sig_count=0;
#if MYTH_USE_SIGIO
  struct sigaction newact;
  memset(&newact,0,sizeof(newact));
  newact.sa_sigaction=myth_io_sighandler;
  newact.sa_flags=SA_SIGINFO;
  sigaction(MYTH_IO_SIGNAL_NO,&newact,NULL);
#endif
#if MYTH_USE_IO_THREAD
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

static void myth_io_worker_fini(myth_running_env_t env,myth_io_struct_perenv_t io) {
#if MYTH_USE_IO_THREAD
  io->exit_flag=1;
  real_pthread_join(io->thread,NULL);
#endif
  real_close(io->epfd);
  myth_io_fd_list_destroy(env,&io->close_list);
}

static inline void myth_io_fini(void) {
  myth_fd_map_destroy(g_fd_map);
}

MYTH_CTX_CALLBACK void myth_wait_for_read_1(void *arg1,void *arg2,void *arg3) {
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

static inline void myth_wait_for_read(int fd,myth_running_env_t env,myth_io_op_t op) {
  //Add current thread to blocked list
  myth_fd_map_t fd_map;
  myth_io_struct_perfd_t fd_data;
  myth_io_wait_list_t wl;
  fd_map=env->io_struct.fd_map;
  //Lookup blocked list from fd_map
  fd_data=myth_fd_map_lookup(fd_map,fd);
  if (!fd_data){
    while (1){
      myth_yield_body();
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

MYTH_CTX_CALLBACK void myth_wait_for_write_1(void *arg1,void *arg2,void *arg3) {
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

static inline void myth_wait_for_write(int fd,myth_running_env_t env,myth_io_op_t op) {
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
      myth_yield_body();
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

static inline void myth_io_register_fd(int fd) {
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
#if MYTH_ONE_IO_WORKER
  //Register all the file descriptors to worker thread #0
  w_env=&g_envs[0];
#elif MYTH_RANDOM_IO_WORKER
  //The worker thread is chosen randomly
  {
    int worker_id;
    worker_id=myth_random(0, g_attr.n_workers);
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
#if MYTH_USE_SIGIO
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

static inline int myth_socket_body (int domain, int type, int protocol) {
  int sock;
  //TODO:Handle SOCK_NONBLOCK option(after 2.6.27)
  sock=real_socket(domain,type,protocol);
  if (sock==-1)return -1;
  //Register a new file descriptor
  myth_io_register_fd(sock);
  return sock;
}

static inline int myth_connect_body (int fd, const struct sockaddr *addr, socklen_t len) {
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
				    socklen_t * addr_len) {
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

static inline int myth_bind_body(int fd, const struct sockaddr *addr, socklen_t len) {
  //Have to do nothing extra
  return real_bind(fd,addr,len);
}

static inline int myth_listen_body (int fd, int n) {
  //Have to do nothing extra
  return real_listen(fd,n);
}

static inline int myth_select_body(int nfds, fd_set *readfds, fd_set *writefds,
				   fd_set *exceptfds, struct timeval *timeout) {
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
#if SELECT_ALWAYS_RETURN_IMMEDIATELY
    if (ret!=0)myth_yield_body();
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
    myth_yield_body();
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

static inline ssize_t myth_sendto_body(int sockfd, const void *buf, 
				       size_t len, int flags,
				       const struct sockaddr *dest_addr, 
				       socklen_t addrlen) {
  MAY_BE_UNUSED uint64_t t0,t1,t2,t3;
  ssize_t ret;
  t0=0;t1=0;t2=0;t3=0;
  myth_running_env_t env;
#if MYTH_IO_PROF_DETAIL
  t0=myth_get_rdtsc();
#endif
  env=myth_get_current_env();
  myth_io_cs_enter(env);
  //Perform non-blocking send
  ret=real_sendto(sockfd,buf,len,flags,dest_addr,addrlen);
#if MYTH_IO_PROF_DETAIL
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
#if MYTH_IO_PROF_DETAIL
    t2=myth_get_rdtsc();
#endif
    op.type=MYTH_IO_SENDTO;
    op.u.st.fd=sockfd;
    op.u.st.buf=buf;
    op.u.st.n=len;
    op.u.st.flags=flags;
    op.u.st.addr=dest_addr;
    op.u.st.addr_len=addrlen;
#if MYTH_IO_PROF_DETAIL
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
#if MYTH_IO_PROF_DETAIL
    env->prof_data.io_succ_send_cycles+=t1-t0;
    env->prof_data.io_succ_send_cnt++;
#endif
  }
  //Done
  return ret;
}

static inline ssize_t myth_recvfrom_body(int sockfd, void *buf, size_t len, 
					 int flags,
					 struct sockaddr *src_addr, 
					 socklen_t *addrlen) {
  MAY_BE_UNUSED uint64_t t0,t1,t2,t3;
  ssize_t ret;
  t0=0;t1=0;t2=0;t3=0;
#if MYTH_IO_PROF_DETAIL
  t0=myth_get_rdtsc();
#endif
  myth_running_env_t env;
  env=myth_get_current_env();
  myth_io_cs_enter(env);
  //Perform non-blocking recv
  ret=real_recvfrom(sockfd,buf,len,flags,src_addr,addrlen);
#if MYTH_IO_PROF_DETAIL
  t1=myth_get_rdtsc();
#endif
  if (ret==-1){
    myth_io_op op;
    if (errno!=EAGAIN && errno!=EWOULDBLOCK){
      //error
      myth_io_cs_exit(env);
      return -1;
    }
#if MYTH_IO_PROF_DETAIL
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
#if MYTH_IO_PROF_DETAIL
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
#if MYTH_IO_PROF_DETAIL
    env->prof_data.io_succ_recv_cycles+=t1-t0;
    env->prof_data.io_succ_recv_cnt++;
#endif
  }
  //Done
  return ret;
}

static inline ssize_t myth_send_body (int fd, const void *buf, size_t n, int flags) {
  MAY_BE_UNUSED uint64_t t0,t1,t2,t3;
  ssize_t ret;
  t0=0;t1=0;t2=0;t3=0;
  myth_running_env_t env;
#if MYTH_IO_PROF_DETAIL
  t0=myth_get_rdtsc();
#endif
  env=myth_get_current_env();
  myth_io_cs_enter(env);
  //Perform non-blocking send
  ret=real_send(fd,buf,n,flags);
#if MYTH_IO_PROF_DETAIL
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
#if MYTH_IO_PROF_DETAIL
    t2=myth_get_rdtsc();
#endif
    op.type=MYTH_IO_SEND;
    op.u.s.fd=fd;
    op.u.s.buf=buf;
    op.u.s.n=n;
    op.u.s.flags=flags;
#if MYTH_IO_PROF_DETAIL
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
#if MYTH_IO_PROF_DETAIL
    env->prof_data.io_succ_send_cycles+=t1-t0;
    env->prof_data.io_succ_send_cnt++;
#endif
  }
  //Done
  return ret;
}

static inline ssize_t myth_recv_body (int fd, void *buf, size_t n, int flags) {
  MAY_BE_UNUSED uint64_t t0,t1,t2,t3;
  ssize_t ret;
  t0=0;t1=0;t2=0;t3=0;
#if MYTH_IO_PROF_DETAIL
  t0=myth_get_rdtsc();
#endif
  myth_running_env_t env;
  env=myth_get_current_env();
  myth_io_cs_enter(env);
  //Perform non-blocking recv
  ret=real_recv(fd,buf,n,flags);
#if MYTH_IO_PROF_DETAIL
  t1=myth_get_rdtsc();
#endif
  if (ret==-1){
    myth_io_op op;
    if (errno!=EAGAIN && errno!=EWOULDBLOCK){
      //error
      myth_io_cs_exit(env);
      return -1;
    }
#if MYTH_IO_PROF_DETAIL
    t2=myth_get_rdtsc();
#endif
    //Wait for I/O ready
    op.type=MYTH_IO_RECV;
    op.u.r.fd=fd;
    op.u.r.buf=buf;
    op.u.r.n=n;
    op.u.r.flags=flags;
#if MYTH_IO_PROF_DETAIL
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
#if MYTH_IO_PROF_DETAIL
    env->prof_data.io_succ_recv_cycles+=t1-t0;
    env->prof_data.io_succ_recv_cnt++;
#endif
  }
  //Done
  return ret;
}

static inline int myth_close_body (int fd) {
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

static inline int myth_fcntl_body (int fd, int cmd,va_list vl) {
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


// Workaround for FX10 glibc bug
#ifndef EPOLLRDHUP
#define EPOLLRDHUP 0x2000
#endif

//Perform I/O operation. Return 0 if the operation failed because of blocking, or 1
static inline int myth_io_execute(myth_io_op_t op) {
  MAY_BE_UNUSED uint64_t t0,t1;
  MAY_BE_UNUSED myth_running_env_t env;
  int ret;
  t0=0;t1=0;
  env=NULL;
  switch (op->type) {
  case MYTH_IO_CONNECT:{
    //In current version, connect is not multiplexed. Therefore this part is unreachable
    myth_unreachable();
    socklen_t ret_len;
    int errcode;
    ret_len=sizeof(int);
    getsockopt(op->u.c.fd,SOL_SOCKET,SO_ERROR,&errcode,&ret_len);
    op->errcode=errcode;
    op->ret=(errcode==0)?0:-1;
    //assert(myth_fd_map_lookup(op->th->env->io_struct.fd_map,op->u.c.fd));
    return 1;
    break;
  }
  case MYTH_IO_SEND:
#if MYTH_IO_PROF_DETAIL
    t0=myth_get_rdtsc();
#endif
    ret=real_send(op->u.s.fd,op->u.s.buf,op->u.s.n,op->u.s.flags);
    //assert(myth_fd_map_lookup(op->th->env->io_struct.fd_map,op->u.s.fd));
#if MYTH_IO_PROF_DETAIL
    t1=myth_get_rdtsc();
    env=myth_get_current_env();
    if (ret==-1 && (errno==EAGAIN || errno==EWOULDBLOCK)){
      env->prof_data.io_block_send_cnt++;
      env->prof_data.io_block_send_cycles+=t1-t0;
    }
    else{
      env->prof_data.io_succ_send_cnt++;
      env->prof_data.io_succ_send_cycles+=t1-t0;
    }
#endif
    break;
  case MYTH_IO_RECV:
#if MYTH_IO_PROF_DETAIL
    t0=myth_get_rdtsc();
#endif
    ret=real_recv(op->u.r.fd,op->u.r.buf,op->u.r.n,op->u.r.flags);
    //assert(myth_fd_map_lookup(op->th->env->io_struct.fd_map,op->u.r.fd));
#if MYTH_IO_PROF_DETAIL
    t1=myth_get_rdtsc();
    env=myth_get_current_env();
    if (ret==-1 && (errno==EAGAIN || errno==EWOULDBLOCK)){
      env->prof_data.io_block_recv_cnt++;
      env->prof_data.io_block_recv_cycles+=t1-t0;
    }
    else if (ret!=-1){
      env->prof_data.io_succ_recv_cnt++;
      env->prof_data.io_succ_recv_cycles+=t1-t0;
    }
#endif
    break;
  case MYTH_IO_SENDTO:
    ret=real_sendto(op->u.st.fd,op->u.st.buf,op->u.st.n,op->u.st.flags,op->u.st.addr,op->u.st.addr_len);
    break;
  case MYTH_IO_RECVFROM:
    ret=real_recvfrom(op->u.rf.fd,op->u.rf.buf,op->u.rf.n,op->u.rf.flags,op->u.rf.addr,op->u.rf.addr_len);
    break;
  case MYTH_IO_SELECT:
    //Not implemented yet
    assert(0);
    break;
  case MYTH_IO_ACCEPT:
    ret=real_accept(op->u.a.fd,op->u.a.addr,op->u.a.len);
    //assert(myth_fd_map_lookup(op->th->env->io_struct.fd_map,op->u.a.fd));
    break;
  default:
    assert(0);
    break;
  }
  if (ret==-1){
    if (errno==EAGAIN || errno==EWOULDBLOCK){
      return 0;
    }
  }
  op->ret=ret;
  op->errcode=errno;
#if 0
  if (ret == -1) {
    switch (op->type) {
    case MYTH_IO_SEND:
      fprintf(stderr, "send:fd=%d,buf=%p,size=%d,flags=%d:", op->u.s.fd,
	      op->u.s.buf, (int) op->u.s.n, (int) op->u.s.flags);
      break;
    case MYTH_IO_RECV:
      fprintf(stderr, "recv:fd=%d,buf=%p,size=%d,flags=%d:", op->u.r.fd,
	      op->u.r.buf, (int) op->u.r.n, (int) op->u.r.flags);
      break;
    case MYTH_IO_ACCEPT:
      fprintf(stderr, "accept:");
      break;
    default:
      assert(0);
      break;
    }
    fprintf(stderr, "errcode=%d\n", op->errcode);
  }
#endif
  return 1;
}

//Checks I/O readiness and return one of the thread that become ready.
//Others are stored into the runqueue.
static inline myth_thread_t myth_io_polling(struct myth_running_env *env) {
  MAY_BE_UNUSED uint64_t t0,t1,t2,t3;
  int i;
  int ready;
  MAY_BE_UNUSED int hit;
  struct epoll_event events[MYTH_EPOLL_SIZE];
  myth_io_struct_perfd_t fd_data;
  myth_io_op_t op;
  myth_thread_t first_runnable;
  first_runnable=NULL;
  t0=0;t1=0;t2=0;t3=0;
  hit=0;
  myth_io_cs_enter(env);
#if MYTH_IO_PROF_DETAIL
  t0=myth_get_rdtsc();
#endif
#if MYTH_ONE_IO_WORKER
  if (env->rank!=0)return NULL;
#endif
  //Pop a blocked read I/O operation from the list
  while ((op=myth_io_wait_list_pop(env,&env->io_struct.rd_reserve_list))!=NULL){
    hit=1;
    //Try again
    if (myth_io_execute(op)){
      //Success
      myth_thread_t th;
      th=op->th;
      th->env=env;
      //Push the thread into the runqueue
      if (!first_runnable){first_runnable=th;}
      else{myth_queue_push(&env->runnable_q,th);}
    }
    else{
      //Blocked. Return to I/O operation list again
      myth_io_wait_list_push(env,op->wl_ptr,op);
      break;
    }
  }
  //Pop a blocked read I/O operation from the list
  while ((op=myth_io_wait_list_pop(env,&env->io_struct.wr_reserve_list))!=NULL){
    //Try again
    if (myth_io_execute(op)){
      //Success
      myth_thread_t th;
      th=op->th;
      th->env=env;
      //Push the thread into the runqueue
      if (!first_runnable){first_runnable=th;}
      else{myth_queue_push(&env->runnable_q,th);}
    }
    else{
      //Blocked. Return to I/O operation list again
      myth_io_wait_list_push(env,op->wl_ptr,op);
      break;
    }
  }
  myth_io_struct_perfd_t *freed_fds;
  int freed_fds_size;
  //Close connection and release data
  int freed_fds_buf_size=1;
  freed_fds=myth_flmalloc(env->rank,freed_fds_buf_size);freed_fds_size=0;
  while ((fd_data=myth_io_fd_list_pop(&env->io_struct.close_list))!=NULL){
    hit=1;
    //Set all the suspended threads ready
    myth_thread_t th;
    while ((op=myth_io_wait_list_pop(env,&fd_data->rd_list))!=NULL){
      th=op->th;
      assert(op->type==MYTH_IO_ACCEPT || op->type==MYTH_IO_RECV);
      int b;
      b=myth_io_execute(op);
      assert(b);
      th->env=env;
      if (!first_runnable){first_runnable=th;}
      else{myth_queue_push(&env->runnable_q,th);}
    }
    myth_io_wait_list_destroy(&fd_data->rd_list);
    while ((op=myth_io_wait_list_pop(env,&fd_data->wr_list))!=NULL){
      th=op->th;
      assert(op->type==MYTH_IO_CONNECT || op->type==MYTH_IO_SEND);
      int b;
      b=myth_io_execute(op);
      assert(b);
      th->env=env;
      if (!first_runnable){first_runnable=th;}
      else{myth_queue_push(&env->runnable_q,th);}
    }
    myth_io_wait_list_destroy(&fd_data->wr_list);
    fd_data->closed=1;
    //Since fds can be used in epoll_wait, they are not released here
    freed_fds=myth_flrealloc(env->rank,freed_fds_buf_size,freed_fds,sizeof(myth_io_struct_perfd_t)*(freed_fds_size+1));
    freed_fds_buf_size=sizeof(myth_io_struct_perfd_t)*(freed_fds_size+1);
    freed_fds[freed_fds_size]=fd_data;
    freed_fds_size++;
  }
#if MYTH_IO_PROF_DETAIL
  t2=myth_get_rdtsc();
#endif
  //Check I/O readiness
  ready=epoll_wait(env->io_struct.epfd,events,MYTH_EPOLL_SIZE,0);
#if MYTH_IO_PROF_DETAIL
  t3=myth_get_rdtsc();
#endif
  if (ready==-1){
    if (errno!=EINTR){
      perror(NULL);
      fprintf(stderr,"Errcode = %d\n",errno);
      assert(0);
    }
  }
  if (ready>0)hit=1;
  for (i=0;i<ready;i++){
    int ef;
    ef=events[i].events;
    fd_data=events[i].data.ptr;
    if (fd_data->closed)continue;//Ignore closed fds
    if (ef & EPOLLRDHUP){
      //Put all the threads waiting for I/O ready
      myth_thread_t th;
      while ((op=myth_io_wait_list_pop(env,&fd_data->rd_list))!=NULL){
	th=op->th;
	MAY_BE_UNUSED int b;
	myth_assert(op->type==MYTH_IO_ACCEPT || op->type==MYTH_IO_RECV);
	b=myth_io_execute(op);
	myth_assert(b);
	th->env=env;
	if (!first_runnable){first_runnable=th;}
	else{myth_queue_push(&env->runnable_q,th);}
      }
      while ((op=myth_io_wait_list_pop(env,&fd_data->wr_list))!=NULL){
	th=op->th;
	MAY_BE_UNUSED int b;
	myth_assert(op->type==MYTH_IO_CONNECT || op->type==MYTH_IO_SEND);
	b=myth_io_execute(op);
	myth_assert(b);
	th->env=env;
	if (!first_runnable){first_runnable=th;}
	else{myth_queue_push(&env->runnable_q,th);}
      }
    }
    else{
      if (ef & EPOLLIN){
	myth_thread_t th;
	while ((op=myth_io_wait_list_pop(env,&fd_data->rd_list))!=NULL){
	  th=op->th;
	  myth_assert(op->type==MYTH_IO_ACCEPT || op->type==MYTH_IO_RECV);
	  //Try again and put into the runqueue if succeeded
	  if (myth_io_execute(op)){
	    th->env=env;
	    if (!first_runnable){first_runnable=th;}
	    else{myth_queue_push(&env->runnable_q,th);}
	  }
	  else{
	    myth_io_wait_list_push(env,&fd_data->rd_list,op);
	    break;
	  }
	}
      }
      if (ef & EPOLLOUT){
	myth_thread_t th;
	while ((op=myth_io_wait_list_pop(env,&fd_data->wr_list))!=NULL){
	  th=op->th;
	  myth_assert(op->type==MYTH_IO_CONNECT || op->type==MYTH_IO_SEND);
	  //Try again and put into the runqueue if succeeded
	  if (myth_io_execute(op)){
	    th->env=env;
	    if (!first_runnable){first_runnable=th;}
	    else{myth_queue_push(&env->runnable_q,th);}
	  }
	  else{
	    myth_io_wait_list_push(env,&fd_data->wr_list,op);
	    break;
	  }
	}
      }
    }
  }
  for (i=0;i<freed_fds_size;i++){
    myth_flfree(env->rank,sizeof(myth_io_struct_perfd),freed_fds[i]);
  }
  myth_flfree(env->rank,freed_fds_buf_size,freed_fds);
  myth_io_cs_exit(env);
#if MYTH_IO_PROF_DETAIL
  t1=myth_get_rdtsc();
  if (g_sched_prof){
    if (hit){
      env->prof_data.io_chk_hit++;
      env->prof_data.io_chk_hit_cycles+=t1-t0;
      env->prof_data.io_epoll_hit++;
      env->prof_data.io_epoll_hit_cycles+=t3-t2;
    }
    else{
      env->prof_data.io_chk_miss_cycles+=t1-t0;
      env->prof_data.io_chk_miss++;
      env->prof_data.io_epoll_miss++;
      env->prof_data.io_epoll_miss_cycles+=t3-t2;
    }
  }
#endif
  return first_runnable;
}

static inline myth_thread_t myth_io_polling_sig(struct myth_running_env *env) {
  MAY_BE_UNUSED uint64_t t0,t1,t2,t3;
  int i;
  int ready;
  MAY_BE_UNUSED int hit;
  struct epoll_event events[MYTH_EPOLL_SIZE];
  myth_io_struct_perfd_t fd_data;
  myth_io_op_t op;
  myth_thread_t first_runnable;
  first_runnable=NULL;
  t0=0;t1=0;t2=0;t3=0;
  hit=0;
  if (myth_io_is_in_cs(env))return NULL;
  myth_io_cs_enter(env);
#if MYTH_IO_PROF_DETAIL
  t0=myth_get_rdtsc();
#endif
#if MYTH_ONE_IO_WORKER
  if (env->rank!=0)return NULL;
#endif
#if MYTH_IO_PROF_DETAIL
  t2=myth_get_rdtsc();
#endif
  ready=epoll_wait(env->io_struct.epfd,events,MYTH_EPOLL_SIZE,0);
#if MYTH_IO_PROF_DETAIL
  t3=myth_get_rdtsc();
#endif
  if (ready==-1){
    if (errno!=EINTR){
      perror(NULL);
      fprintf(stderr,"Errcode = %d\n",errno);
      assert(0);
    }
  }
  if (ready>0)hit=1;
  for (i=0;i<ready;i++){
    int ef;
    ef=events[i].events;
    fd_data=events[i].data.ptr;
    if (fd_data->closed)continue;
    if (ef & EPOLLRDHUP){
      myth_thread_t th;
      while ((op=myth_io_wait_list_pop(env,&fd_data->rd_list))!=NULL){
	th=op->th;
	MAY_BE_UNUSED int b;
	myth_assert(op->type==MYTH_IO_ACCEPT || op->type==MYTH_IO_RECV);
	b=myth_io_execute(op);
	myth_assert(b);
	th->env=env;
	if (!first_runnable){first_runnable=th;}
	else{myth_queue_push(&env->runnable_q,th);}
      }
      while ((op=myth_io_wait_list_pop(env,&fd_data->wr_list))!=NULL){
	th=op->th;
	MAY_BE_UNUSED int b;
	myth_assert(op->type==MYTH_IO_CONNECT || op->type==MYTH_IO_SEND);
	b=myth_io_execute(op);
	myth_assert(b);
	th->env=env;
	if (!first_runnable){first_runnable=th;}
	else{myth_queue_push(&env->runnable_q,th);}
      }
    }
    else{
      if (ef & EPOLLIN){
	myth_thread_t th;
	while ((op=myth_io_wait_list_pop(env,&fd_data->rd_list))!=NULL){
	  th=op->th;
	  myth_assert(op->type==MYTH_IO_ACCEPT || op->type==MYTH_IO_RECV);
	  if (myth_io_execute(op)){
	    th->env=env;
	    if (!first_runnable){first_runnable=th;}
	    else{myth_queue_push(&env->runnable_q,th);}
	  }
	  else{
	    myth_io_wait_list_push(env,&fd_data->rd_list,op);
	    break;
	  }
	}
      }
      if (ef & EPOLLOUT){
	myth_thread_t th;
	while ((op=myth_io_wait_list_pop(env,&fd_data->wr_list))!=NULL){
	  th=op->th;
	  myth_assert(op->type==MYTH_IO_CONNECT || op->type==MYTH_IO_SEND);
	  if (myth_io_execute(op)){
	    th->env=env;
	    if (!first_runnable){first_runnable=th;}
	    else{myth_queue_push(&env->runnable_q,th);}
	  }
	  else{
	    myth_io_wait_list_push(env,&fd_data->wr_list,op);
	    break;
	  }
	}
      }
    }
  }
#if MYTH_IO_PROF_DETAIL
  t1=myth_get_rdtsc();
  if (g_sched_prof){
    if (hit){
      env->prof_data.io_chk_hit++;
      env->prof_data.io_chk_hit_cycles+=t1-t0;
      env->prof_data.io_epoll_hit++;
      env->prof_data.io_epoll_hit_cycles+=t3-t2;
    }
    else{
      env->prof_data.io_chk_miss_cycles+=t1-t0;
      env->prof_data.io_chk_miss++;
      env->prof_data.io_epoll_miss++;
      env->prof_data.io_epoll_miss_cycles+=t3-t2;
    }
  }
#endif
  myth_io_cs_exit(env);
  return first_runnable;
}

#if MYTH_USE_SIGIO
static void myth_io_sighandler(int signum,siginfo_t *sinfo,void* ctx) {
  //	int ret;
  //	ret=write(1,"SIGIO\n",6);
  //	printf("fd=%d\n",sinfo->si_fd);
  //	return;
  int errno_bk;
  myth_running_env_t env;
  myth_thread_t th;
  myth_fd_map_t fd_map;
  myth_io_struct_perfd_t fd_data;
  myth_io_op_t op;
  errno_bk=errno;
  env=myth_get_current_env();
  env->io_struct.sig_count++;
  if (myth_queue_is_operating(&env->runnable_q)){errno=errno_bk;return;}
  if (myth_io_is_in_cs(env)){errno=errno_bk;return;}
  fd_map=env->io_struct.fd_map;
  assert(fd_map);
  //Lookup blocked list from fd_map
  fd_data=myth_fd_map_trylookup(fd_map,sinfo->si_fd);
  if (!fd_data){errno=errno_bk;return;}
  myth_io_cs_enter(env);
  switch (sinfo->si_code)
    {
    case POLL_IN:
      //lookup data from fd
      while ((op=myth_io_wait_list_pop(env,&fd_data->rd_list))!=NULL){
	th=op->th;
	myth_assert(op->type==MYTH_IO_ACCEPT || op->type==MYTH_IO_RECV);
	if (myth_io_execute(op)){
	  th->env=env;
	  {myth_queue_push(&env->runnable_q,th);}
	}
	else{
	  myth_io_wait_list_push(env,fd_data->rd_reserve_list_ptr,op);
	  break;
	}
      }
      break;
    case POLL_OUT:
      while ((op=myth_io_wait_list_pop(env,&fd_data->wr_list))!=NULL){
	th=op->th;
	myth_assert(op->type==MYTH_IO_CONNECT || op->type==MYTH_IO_SEND);
	if (myth_io_execute(op)){
	  th->env=env;
	  {myth_queue_push(&env->runnable_q,th);}
	}
	else{
	  myth_io_wait_list_push(env,fd_data->wr_reserve_list_ptr,op);
	  break;
	}
      }
      break;
    case POLL_HUP:
      break;
    case POLL_MSG:
    case POLL_ERR:
    case POLL_PRI:
      break;
    }
  myth_io_cs_exit(env);
  errno=errno_bk;
}
#endif

#if MYTH_USE_IO_THREAD
static inline myth_thread_t myth_io_polling_thread(struct myth_running_env *env) {
  uint64_t t0,t1,t2,t3;
  int i;
  int ready;
  int hit;
  struct epoll_event events[MYTH_EPOLL_SIZE];
  myth_io_struct_perfd_t fd_data;
  myth_io_op_t op;
  myth_thread_t first_runnable;
  first_runnable=NULL;
  t0=0;t1=0;t2=0;t3=0;
  hit=0;
  myth_io_cs_enter(env);
#if MYTH_IO_PROF_DETAIL
  t0=myth_get_rdtsc();
#endif
#if MYTH_ONE_IO_WORKER
  if (env->rank!=0)return NULL;
#endif
  while ((op=myth_io_wait_list_pop(env,&env->io_struct.rd_reserve_list))!=NULL){
    hit=1;
    if (myth_io_execute(op)){
      myth_thread_t th;
      th=op->th;
      th->env=env;
      if (!first_runnable){first_runnable=th;}
      else{myth_queue_push(&env->runnable_q,th);}
    }
    else{
      myth_io_wait_list_push(env,op->wl_ptr,op);
      break;
    }
  }
  while ((op=myth_io_wait_list_pop(env,&env->io_struct.wr_reserve_list))!=NULL){
    if (myth_io_execute(op)){
      myth_thread_t th;
      th=op->th;
      th->env=env;
      if (!first_runnable){first_runnable=th;}
      else{myth_queue_push(&env->runnable_q,th);}
    }
    else{
      myth_io_wait_list_push(env,op->wl_ptr,op);
      break;
    }
  }
#if MYTH_IO_PROF_DETAIL
  t2=myth_get_rdtsc();
#endif
  //wait by epoll
  myth_io_cs_exit(env);
  ready = epoll_wait(env->io_struct.epfd, events, MYTH_EPOLL_SIZE, MYTH_IO_THREAD_PERIOD);
  myth_io_cs_enter(env);
#if MYTH_IO_PROF_DETAIL
  t3=myth_get_rdtsc();
#endif
  if (ready==-1){
    if (errno!=EINTR){
      perror(NULL);
      fprintf(stderr,"Errcode = %d\n",errno);
      assert(0);
    }
  }
  if (ready>0)hit=1;
  for (i=0;i<ready;i++){
    int ef;
    ef=events[i].events;
    fd_data=events[i].data.ptr;
    if (fd_data->closed)continue;
    if (ef & EPOLLRDHUP){
      myth_thread_t th;
      while ((op=myth_io_wait_list_pop(env,&fd_data->rd_list))!=NULL){
	th=op->th;
	int b;
	myth_assert(op->type==MYTH_IO_ACCEPT || op->type==MYTH_IO_RECV);
	b=myth_io_execute(op);
	myth_assert(b);
	th->env=env;
	if (!first_runnable){first_runnable=th;}
	else{myth_queue_push(&env->runnable_q,th);}
      }
      while ((op=myth_io_wait_list_pop(env,&fd_data->wr_list))!=NULL){
	th=op->th;
	int b;
	myth_assert(op->type==MYTH_IO_CONNECT || op->type==MYTH_IO_SEND);
	b=myth_io_execute(op);
	myth_assert(b);
	th->env=env;
	if (!first_runnable){first_runnable=th;}
	else{myth_queue_push(&env->runnable_q,th);}
      }
    }
    else{
      if (ef & EPOLLIN){
	myth_thread_t th;
	while ((op=myth_io_wait_list_pop(env,&fd_data->rd_list))!=NULL){
	  th=op->th;
	  myth_assert(op->type==MYTH_IO_ACCEPT || op->type==MYTH_IO_RECV);
	  if (myth_io_execute(op)){
	    th->env=env;
	    if (!first_runnable){first_runnable=th;}
	    else{myth_queue_push(&env->runnable_q,th);}
	  }
	  else{
	    myth_io_wait_list_push(env,&fd_data->rd_list,op);
	    break;
	  }
	}
      }
      if (ef & EPOLLOUT){
	myth_thread_t th;
	while ((op=myth_io_wait_list_pop(env,&fd_data->wr_list))!=NULL){
	  th=op->th;
	  myth_assert(op->type==MYTH_IO_CONNECT || op->type==MYTH_IO_SEND);
	  if (myth_io_execute(op)){
	    th->env=env;
	    if (!first_runnable){first_runnable=th;}
	    else{myth_queue_push(&env->runnable_q,th);}
	  }
	  else{
	    myth_io_wait_list_push(env,&fd_data->wr_list,op);
	    break;
	  }
	}
      }
    }
  }
#if MYTH_IO_PROF_DETAIL
  t1=myth_get_rdtsc();
  if (g_sched_prof){
    if (hit){
      env->prof_data.io_chk_hit++;
      env->prof_data.io_chk_hit_cycles+=t1-t0;
      env->prof_data.io_epoll_hit++;
      env->prof_data.io_epoll_hit_cycles+=t3-t2;
    }
    else{
      env->prof_data.io_chk_miss_cycles+=t1-t0;
      env->prof_data.io_chk_miss++;
      env->prof_data.io_epoll_miss++;
      env->prof_data.io_epoll_miss_cycles+=t3-t2;
    }
  }
#endif
  myth_queue_push(&env->runnable_q,first_runnable);
  myth_io_cs_exit(env);
  return NULL;
}
#endif


#endif //MYTH_WRAP_SOCKIO

#endif /* MYTH_IO_FUNC_H_ */
