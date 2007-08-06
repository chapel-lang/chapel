#ifndef SOCKETS_H_
#define SOCKETS_H_
#include "armci.h"
#ifndef WIN32
#define USE_SOCKET_VECTOR_API
#endif
#if defined(USE_SOCKET_VECTOR_API)
#  include <sys/uio.h>  
#endif
extern int tcp_sendrcv_bufsize;        
extern int armci_PollSocket(int sock);
extern int armci_WaitSock(int *socklist, int num, int *ready);
extern int armci_ReadFromSocket(int sock, void* buffer, int lenbuf);
extern int armci_WriteToSocket (int sock, void* buffer, int lenbuf);

#if defined(USE_SOCKET_VECTOR_API)
extern int armci_RecvStridedFromSocket(int sock,void* buffer,int *str_arr,int *cnt,int str_level,struct iovec *iov); 
extern int armci_SendStridedToSocket(int sock,void* buffer,int *str_arr,int *cnt,int str_level,struct iovec *iov);
extern int armci_RecvVectorFromSocket(int sock,armci_giov_t darr[], int len,struct iovec *iov);
extern int armci_SendVectorToSocket(int sock,armci_giov_t darr[], int len,struct iovec *iov);
extern int armci_ReadVFromSocket(int sock,struct iovec *iov, int iovlength, int totalsize);
extern int armci_WriteVToSocket (int sock,struct iovec *iov, int iovlength, int totalsize);
#endif
extern void armci_ListenSockAll(int* socklist, int num);
extern void armci_AcceptSockAll(int* socklist, int num);
extern int armci_CreateSocketAndConnect(char *hostname, int port);
extern void armci_ShutdownAll(int socklist[], int num);
extern void armci_CreateSocketAndBind(int *sock, int *port);
#define PACKET_SIZE tcp_sendrcv_bufsize 
#define TIMEOUT_ACCEPT 60
#define GET_SEND_BUFFER _armci_buf_get
#define FREE_SEND_BUFFER _armci_buf_release

#ifndef UIO_MAXIOV
#define MAX_IOVEC 8
#else
#define MAX_IOVEC (UIO_MAXIOV>100?100:UIO_MAXIOV)
#endif
#endif
