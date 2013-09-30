#include <assert.h>

#include "myth_desc.h"
#include "myth_worker.h"
#include "myth_io.h"

#include "myth_io_proto.h"

#include "myth_worker_func.h"
#include "myth_io_func.h"

myth_fd_map_t g_fd_map;

int g_recvfrom_cnt,g_sendto_cnt;
int g_n_recvfrom_cnt,g_n_sendto_cnt;

#ifdef MYTH_WRAP_SOCKIO

int socket (int domain, int type, int protocol)
{
	return myth_socket_body(domain,type,protocol);
}
int connect (int fd, const struct sockaddr *addr, socklen_t len)
{
	return myth_connect_body(fd,addr,len);
}
int accept (int fd, struct sockaddr* addr,
		   socklen_t * addr_len)
{
	return myth_accept_body(fd,addr,addr_len);
}
int listen (int fd, int n)
{
	return myth_listen_body(fd,n);
}
int bind(int fd, const struct sockaddr* addr, socklen_t len)
{
	return myth_bind_body(fd,addr,len);
}
int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout)
{
	return myth_select_body(nfds,readfds,writefds,exceptfds,timeout);
}
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
	return myth_sendto_body(sockfd,buf,len,flags,dest_addr,addrlen);
}
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
	return myth_recvfrom_body(sockfd,buf,len,flags,src_addr,addrlen);
}
ssize_t send (int fd, const void *buf, size_t n, int flags)
{
	return myth_send_body(fd,buf,n,flags);
}
ssize_t recv (int fd, void *buf, size_t n, int flags)
{
	return myth_recv_body(fd,buf,n,flags);
}
int close (int fd)
{
	return myth_close_body(fd);
}
int fcntl (int fd, int cmd, ...)
{
	int ret;
	va_list vl;
	va_start(vl,cmd);
	ret=myth_fcntl_body(fd,cmd,vl);
	va_end(vl);
	return ret;
}

#endif
