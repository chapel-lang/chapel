/* 
 * myth_io.h
 */
#pragma once
#ifndef MYTH_IO_H_
#define MYTH_IO_H_

#include <sys/socket.h>
#include <stdint.h>
#include <stdarg.h>

#include "myth/myth.h"
#include "myth_config.h"
#include "myth_spinlock_func.h"

typedef enum myth_io_type {
  MYTH_IO_CONNECT,
  MYTH_IO_ACCEPT,
  MYTH_IO_RECV,
  MYTH_IO_SEND,
  MYTH_IO_RECVFROM,
  MYTH_IO_SENDTO,
  MYTH_IO_SELECT,
} myth_io_type;

typedef struct myth_fd_map_entry {
  int bufsize;
  int size;
  //void **data;
  struct myth_io_struct_perfd **data;
  int *key;
  myth_spinlock_t lock;
} myth_fd_map_entry,*myth_fd_map_entry_t;

typedef struct myth_fd_map {
  myth_spinlock_t lock;
  myth_fd_map_entry *entry;
  //int size;
} myth_fd_map,*myth_fd_map_t;

typedef struct myth_io_fd_list {//File descriptor list
  myth_spinlock_t lock;
  int size;
  int bufsize;
  struct myth_io_struct_perfd **data;
} myth_io_fd_list, *myth_io_fd_list_t;

//I/O operation descriptor containing type and arguments
typedef struct myth_io_op {
  struct myth_thread *th;//The owner thread
  struct myth_io_wait_list *wl_ptr;
  myth_io_type type;
  union{
    struct{
      int fd;
      struct sockaddr *addr;
      socklen_t *len;
    } a;//accept
    struct{
      int fd;
      //__SOCKADDR_ARG addr;
      //socklen_t *addr_len;
    } c;
    struct{
      int fd;
      __const void *buf;
      size_t n;
      int flags;
    } s;//send
    struct{
      int fd;
      void *buf;
      size_t n;
      int flags;
    } r;//recv
    struct{
      int fd;
      __const void *buf;
      size_t n;
      int flags;
      const struct sockaddr *addr;
      socklen_t addr_len;
    } st;//sendto
    struct{
      int fd;
      void *buf;
      size_t n;
      int flags;
      struct sockaddr *addr;
      socklen_t *addr_len;
    } rf;//recvfrom
    struct{
      int nfds;
      fd_set *readfds;
      fd_set *writefds;
      fd_set *exceptfds;
      struct timeval *timeout;
    } sl;//select
  } u;
  int ret;
  int errcode;
} myth_io_op, *myth_io_op_t;

//Pending I/O operation list
typedef struct myth_io_wait_list {
  myth_spinlock_t lock;
  myth_io_op_t *io_ops;
  int size;//the size of io_ops
  int count;//valid elements
} myth_io_wait_list, *myth_io_wait_list_t;

//Per-worker-thread structure
typedef struct myth_io_struct_perenv {
  int fd_count;
  int epfd;
  int epnum;
  myth_fd_map_t fd_map;
  myth_io_fd_list close_list;
  myth_io_wait_list rd_reserve_list;
  myth_io_wait_list wr_reserve_list;
  //myth_io_fd_list notify_list;
  volatile int8_t cs_flag;
  int sig_count;
#if MYTH_USE_IO_THREAD
  pthread_t thread;
  uint8_t exit_flag;
  pthread_mutex_t mtx;
#endif
} myth_io_struct_perenv, *myth_io_struct_perenv_t;

//typedef struct myth_running_env * myth_running_env_t;

//Fd-specific data structure
typedef struct myth_io_struct_perfd {
  int fd;
  myth_io_wait_list rd_list;
  myth_io_wait_list wr_list;
  myth_io_wait_list *rd_reserve_list_ptr;
  myth_io_wait_list *wr_reserve_list_ptr;
  int tmp_for_poll;
  myth_running_env_t env;
  int closed;
} myth_io_struct_perfd, *myth_io_struct_perfd_t;

extern myth_fd_map_t g_fd_map;


#if MYTH_WRAP_SOCKIO

static void myth_io_init(void);
static void myth_io_fini(void);
static void myth_io_worker_init(myth_running_env_t env,
				myth_io_struct_perenv_t io);
static void myth_io_worker_fini(myth_running_env_t env,
				myth_io_struct_perenv_t io);

static inline void myth_wait_for_read(int fd, myth_running_env_t env,
				      myth_io_op_t op);
static inline void myth_wait_for_write(int fd, myth_running_env_t env,
				       myth_io_op_t op);

static inline int myth_socket_body (int domain, int type, int protocol);
static inline int myth_connect_body (int fd, const struct sockaddr* addr,
				     socklen_t len);
static inline int myth_accept_body (int fd, struct sockaddr *addr,
		   socklen_t * addr_len);
static inline int myth_bind_body(int fd, const struct sockaddr* addr, socklen_t len);
static inline int myth_listen_body (int fd, int n);
static inline ssize_t myth_send_body (int fd, const void *buf, size_t n, int flags);
static inline ssize_t myth_recv_body (int fd, void *buf, size_t n, int flags);
static inline int myth_close_body (int fd);
static inline int myth_fcntl_body (int fd, int cmd,va_list vl);
static inline myth_thread_t myth_io_polling(struct myth_running_env *env);
static inline int myth_io_execute(myth_io_op_t op);

#endif	/* MYTH_WRAP_SOCKIO */

#endif /* MYTH_IO_H_ */
