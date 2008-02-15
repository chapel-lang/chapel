/* $Id: sockets.c,v 1.23 2004/04/15 05:11:33 vinod Exp $ */
/**************************************************************************
 Some parts of this code were derived from the TCGMSG file sockets.c
 Jarek Nieplocha, last update 10/28/99
 02/28/00: modified armci_WaitSock to allow some elements of socklist to 
           be <0 (and ignored). Needed for the threaded version of server.
 *************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef WIN32
#  include <winsock.h>
#  define bcopy(s1,s2,n) memcpy(s2,s1,n)
#  define sleep(x) Sleep(1000*(x))
#  define CLOSE closesocket
#else
#  include <sys/wait.h>
#  include <sys/time.h>
#  include <sys/types.h>
#  include <sys/socket.h>
/*#  include <sys/uio.h> */ /*moved to sockets.h*/ 
#  include <netinet/in.h>
#  include <netinet/tcp.h>
#  include <netdb.h>
#  include <unistd.h>
#  define CLOSE close
#endif

#include "sockets.h"

#ifdef AIX
#  include <standards.h>
#  include <sys/select.h>
#  ifdef _AIXVERSION_430
     typedef socklen_t soclen_t;
#  else
     typedef size_t soclen_t;
#  endif
#elif defined(XLCLINUX)
typedef socklen_t soclen_t;
#else
typedef int soclen_t;
#endif


#ifdef CRAY
#include <memory.h>
#endif

/* portability of socklen_t definition is iffy - we need to avoid it !!
#if defined(LINUX) && ( defined(_SOCKETBITS_H) || defined(__BITS_SOCKET_H))
#elif defined(AIX)
  typedef size_t socklen_t;
#else
  typedef int socklen_t;
#endif
*/

#ifndef MAX_STRIDE_LEVEL
#define MAX_STRIDE_LEVEL 8
#endif

extern int armci_me, armci_nproc,armci_nclus;
extern void armci_die(char* str,int);
extern void armci_die2(char* str,int,int); 
int tcp_sendrcv_bufsize=131072;
#define DEBUG_ 0
#define DEBUG1 0
#define CONNECT_TRIALS 4 
#define MAX_INTR_NO_DATA 8

int armci_PollSocket(int sock)
/*
  Poll the socket for available input.

  Return 1 if data is available, 0 otherwise.
*/
{
  fd_set ready;
  struct timeval timelimit;
  int nready;

  if (sock < 0)
    return 0;

again:
  FD_ZERO(&ready);
  FD_SET(sock, &ready);
  timelimit.tv_sec = 0;
  timelimit.tv_usec = 0;

  nready = select(sock+1, &ready, (fd_set *) NULL, (fd_set *) NULL, &timelimit);
  if (nready < 0) {
    if (errno == EINTR)
      goto again;
    else
      armci_die("armci_PollSocket: error from select",   sock);
  }

  return nready;
}


/*\ sleep in select until data appears on one of sockets
 *  return number of sockets ready and indicate which ones are in ready array 
 *  allows <0 values in socklist array (ignores them)
\*/
int armci_WaitSock(int *socklist, int num, int *ready)
{

  int sock,maxsock=0;
  fd_set dset;
  int nready;

  if(num<0) armci_die("armci_WaitSock: num <0",num);

again:

  FD_ZERO(&dset);
  maxsock=0;
  for(sock=0; sock<num; sock++){
     if(socklist[sock] > maxsock)maxsock = socklist[sock];
     
     if(socklist[sock] >0){ /* ignore fd=-1 on the list */
        FD_SET(socklist[sock], &dset);
     }
  }


  nready = select(maxsock+1, &dset, (fd_set*)NULL, (fd_set*)NULL, NULL); 

    if (nready < 0) {
    if (errno == EINTR){
      fprintf(stderr,"%d:interrupted in select\n",armci_me);
      goto again;
    } else
      armci_die("armci_WaitSocket: error from select",   sock);
  }

  
  for(sock=0; sock<num; sock++){
     ready[sock]=0;
     if(socklist[sock] < 0) continue; 
     if(FD_ISSET(socklist[sock],&dset)) ready[sock]=1;
  }

  return nready;
}
    




void armci_TcpNoDelay( int sock)
/*
  Turn off waiting for more input to improve buffering 
  by TCP layer ... improves performance for small messages by
  a factor of 30 or more. Slightly degrades performance for
  large messages.
*/
{
  int status, level, value=1;
#ifdef AIX
  struct protoent *proto = getprotobyname("tcp");
#else
  struct protoent *proto = getprotobyname("TCP");
#endif
  void *optval = &value;

  if (proto == (struct protoent *) NULL)
    armci_die("armci_TcpNoDelay: getprotobyname on TCP failed!",  -1);

  level = proto->p_proto;

  status = setsockopt(sock, level, TCP_NODELAY, optval, sizeof(int));

  if (status != 0)
    armci_die("armci_TcpNoDelay: setsockopt failed",  status);
}



void armci_ShutdownAll(int socklist[], int num)
/* 
   close all sockets discarding any pending data in either direction.
*/
{
   int i;

   for (i=0; i<num; i++)
      if (socklist[i] >= 0) {
         (void) shutdown(socklist[i], 2);
         (void) CLOSE(socklist[i]);
         socklist[i]=-1;
      }
}


#if defined(USE_SOCKET_VECTOR_API) 

int _armci_tcp_writev(int sock, struct iovec *iovptr,int writeiovlength,int currentwritesize,struct iovec *iov){
    int n=0;
    while(n!=currentwritesize){
        int rc;
        rc=writev(sock,iovptr,writeiovlength);
        if(rc<0)perror("writev failed");
        if(DEBUG1&&0)if(rc<currentwritesize){printf("\n%d:_armci_tcp_writev write %d bytes of %d bytes writeiovlen=%d",armci_me,rc,currentwritesize,writeiovlength);fflush(stdout);} 
        n+=rc;
        if(n<currentwritesize){
            int completediovs=0;
            int templength=0;
            while(templength!=rc){
                if(iovptr->iov_len+templength>rc){
                    iovptr->iov_base=(char *)((*iovptr).iov_base)+(rc-templength);
                    iovptr->iov_len-=(rc-templength);
                    templength+=(rc-templength);
                }
                else {
                    templength+=iovptr->iov_len;
                    iovptr+=1;
                    completediovs++;
                }
            }
            writeiovlength-=completediovs;
            if(writeiovlength<=0)writeiovlength=1;
        } 
    }
    return(n);
}

int _armci_tcp_readv(int sock, struct iovec *iovptr,int readiovlength,int currentreadsize,struct iovec *iov){
    int n=0;
    while(n!=currentreadsize){
        int rc;
        rc=readv(sock,iovptr,readiovlength);
        if(rc<0)perror("readv failed");
        if(DEBUG1&&0)if(rc<currentreadsize){printf("\n%d:_armci_tcp_readv Read %d bytes of %d bytes readiovlen=%d",armci_me,rc,currentreadsize,readiovlength);fflush(stdout);}
        n+=rc;
        if(n<currentreadsize){
            int completediovs=0;
            int templength=0;
            while(templength!=rc){
                if(iovptr->iov_len+templength>rc){
                    iovptr->iov_base=(char *)((*iovptr).iov_base)+(rc-templength);
                    iovptr->iov_len-=(rc-templength);
                    templength+=(rc-templength);
                }
                else {
                    templength+=iovptr->iov_len;
                    iovptr+=1;
                    completediovs++;
                }
            }
            readiovlength-=completediovs;
            if(readiovlength<=0)readiovlength=1;
        }
    }
    return(n);
}

int armci_ReadVFromSocket(int sock,struct iovec *iov, int iovlength, int totalsize)
{
    struct iovec *iovptr;
    int i=0,num_xmit=1,lastiovoriglen=0,lastiovnewlen=0,lastiovindex=-1,n=0;
    int readiovlength,currentreadsize=totalsize,totalreadsofar=0,byteslefttoread=0;
    char *lastiovorigbase=NULL;
    iovptr=iov; 
    if(totalsize>PACKET_SIZE){
        while(totalreadsofar!=totalsize){
            currentreadsize=0;
            if(lastiovindex>=0)
                iovptr=iov+lastiovindex;
            if(lastiovoriglen!=0){
	        iov[lastiovindex].iov_base = (lastiovorigbase+lastiovnewlen); 
	        iov[lastiovindex].iov_len=lastiovoriglen-lastiovnewlen;
                lastiovoriglen=0;   
            } 
            iovlength=0;  
            
            if(totalsize-totalreadsofar<PACKET_SIZE)byteslefttoread=totalsize-totalreadsofar;
            else byteslefttoread=PACKET_SIZE;
	    while(currentreadsize<byteslefttoread){
                if(iov[i].iov_len+currentreadsize>byteslefttoread){
                    lastiovoriglen=iov[i].iov_len;lastiovorigbase=(char *)iov[i].iov_base;
                    lastiovindex=i; 
		    iov[i].iov_len=byteslefttoread-currentreadsize;
		    currentreadsize+=iov[i].iov_len; lastiovnewlen=iov[i].iov_len;
                }
                else {  
		    currentreadsize+=iov[i].iov_len;
                    i++;
                    lastiovindex=i;
                    iovlength++;
                }
            }
            if(lastiovoriglen>0)iovlength+=1;
            totalreadsofar+=currentreadsize;
            num_xmit++;
            readiovlength=iovlength;
            n=_armci_tcp_readv(sock,iovptr,readiovlength,currentreadsize,iov);  
        if(DEBUG1){printf("\nFinished reading n=%d bytes iov of length %d \n",n,iovlength);fflush(stdout);}
        }
    }
    else {
	iovptr=iov;
        readiovlength=iovlength;
        n=0;
        n+=_armci_tcp_readv(sock,iovptr,readiovlength,currentreadsize,iov); 
        if(DEBUG1){printf("\nFits in one packet Finished reading n=%d bytes iov of length %d \n",n,iovlength);fflush(stdout);}
     }  
    return(n);
}


int armci_WriteVToSocket(int sock,struct iovec *iov, int iovlength, int totalsize){     

    int lastiovoriglen=0,lastiovnewlen=0,lastiovindex=-1,totalwritesofar=0,byteslefttowrite=0;
    struct iovec *iovptr; 
    int i=0,num_xmit=0,n=0;
    int currentwritesize=totalsize,writeiovlength;
     char *lastiovorigbase=NULL;
    iovptr=iov; 
    if(totalsize>PACKET_SIZE){
 	while(totalwritesofar!=totalsize){      
            currentwritesize=0; 
            if(lastiovindex>=0)
	        iovptr=iov+lastiovindex;
            if(lastiovoriglen!=0){
                iov[lastiovindex].iov_base = (lastiovorigbase+lastiovnewlen); 
                iov[lastiovindex].iov_len= lastiovoriglen-lastiovnewlen;
                lastiovoriglen=0;
            }
            iovlength=0;
          
            if(totalsize-totalwritesofar<PACKET_SIZE)byteslefttowrite=totalsize-totalwritesofar;
            else byteslefttowrite=PACKET_SIZE;
            while(currentwritesize<byteslefttowrite){
		if(iov[i].iov_len+currentwritesize>byteslefttowrite){
                    lastiovoriglen=iov[i].iov_len;lastiovorigbase=(char *)iov[i].iov_base;
                    lastiovindex=i;
                    iov[i].iov_len=byteslefttowrite-currentwritesize;
		    currentwritesize+=iov[i].iov_len;lastiovnewlen=iov[i].iov_len;
                }
                else {
                    currentwritesize+=iov[i].iov_len;
                    i++;
                    lastiovindex=i;
                    iovlength++;
                }
            }
            totalwritesofar+=currentwritesize;
            num_xmit++;
            if(lastiovoriglen>0)iovlength+=1;
            writeiovlength=iovlength;     
	    n=_armci_tcp_writev(sock,iovptr,writeiovlength,currentwritesize,iov); 
            if(DEBUG1){printf("\nFinished writing n=%d iov of length %d \n",n,iovlength);fflush(stdout);}
            if(n!=currentwritesize)armci_die2("\n problems with writing using writev\n",n,currentwritesize);
        }
  
    }
    else {
	iovptr=iov;
        writeiovlength=iovlength;  
        n=0;
        n= _armci_tcp_writev(sock,iovptr,writeiovlength,currentwritesize,iov); 
        if(n<0)perror("write failed");
        if(DEBUG1){printf("\nFits in one packet Finished writing n=%d iov of length %d \n",n,iovlength);fflush(stdout);}
        if(n!=currentwritesize)armci_die2("\n problems with writing using writev\n",n,currentwritesize);
    } 
    return(n);
}

#endif /*for use_socket_vec_api*/  

int armci_ReadFromSocket(int sock, void* buffer, int lenbuf)
/*
   Read from the socket until we get all we want.
*/
{

   int nread, status, nintr=0;
   char *buf = (char*)buffer;
   status = lenbuf;
   while (lenbuf > 0) {
again:
     
     nread = recv(sock, buf, lenbuf, 0);
     /* on linux 0 can be returned if socket is closed  by sender */ 
     if(nread < 0 || ((nread ==  0) && errno ) ){
       if (errno == EINTR){

         if(DEBUG_){
           fprintf(stderr,"%d:interrupted in recv\n",armci_me);
	 }

         /* retry a few times if nread==0 */
         if(nread==0) nintr++; 
         else nintr=0;
         if(nintr>MAX_INTR_NO_DATA) return -1; /* the socket must be closed */

         goto again;

       }else {
         if(DEBUG_){
           (void) fprintf(stderr,"sock=%d, pid=%d, nread=%d, len=%d\n",
                               sock, armci_me, nread, lenbuf);
           if(errno)perror("armci_ReadFromSocket: recv failed");
         }
         status = -1;
         break;
       }
     }
     buf += nread;
     lenbuf -= nread;
   }
    
   return status;
}


int armci_WriteToSocket (int sock, void* buffer, int lenbuf)
/*
  Write to the socket in packets of PACKET_SIZE bytes
*/
{
  int status = lenbuf;
  int nsent, len;
  char *buf = (char*)buffer;
 
  if(DEBUG_){
    printf("%d armci_WriteToSocket sock=%d lenbuf=%d\n",armci_me,sock,lenbuf);
    fflush(stdout);
  }

  while (lenbuf > 0) {
again:

    len = (lenbuf > PACKET_SIZE) ? PACKET_SIZE : lenbuf;
    nsent = send(sock, buf, len, 0);
   
    if (nsent < 0) { /* This is bad news */
      if(errno ==EINTR){
         if(DEBUG_){
           fprintf(stderr,"%d:interrupted in socket send\n",armci_me);
         }
         goto again;
      }else{

         if(DEBUG_){
           (void) fprintf(stderr,"sock=%d, pid=%d, nsent=%d, len=%d\n",
                        sock, armci_me, nsent, lenbuf);
           (void) fflush(stderr);
         }
        status = -1; break;
      }
    }

    buf += nsent;
    lenbuf -= nsent;
  }

  return status;
}



void armci_CreateSocketAndBind(int *sock, int *port)
/*
  Create a socket, bind it to a wildcard internet name and return
  the info so that its port number may be advertised
*/
{
  soclen_t  length;
  struct sockaddr_in server;
  int size = PACKET_SIZE;
  int on = 1;

  length = sizeof (struct sockaddr_in);

  /* Create socket */

  if ( (*sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    armci_die("armci_CreateSocketAndBind: socket creation failed",  *sock);

  if(setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof on) == -1)
	armci_die("armci_CreateSocketAndBind: error from setsockopt",  -1);

  /* Increase size of socket buffers to improve long message
     performance and increase size of message that goes asynchronously */

  if(setsockopt(*sock, SOL_SOCKET, SO_RCVBUF, (char *) &size, sizeof(size)))
    armci_die("armci_CreateSocketAndBind: error setting SO_RCVBUF", size);
  if(setsockopt(*sock, SOL_SOCKET, SO_SNDBUF, (char *) &size, sizeof(size)))
    armci_die("armci_CreateSocketAndBind: error setting SO_SNDBUF", size);

  armci_TcpNoDelay(*sock);

  /* Name socket with wildcards */

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = 0;
  if (bind(*sock, (struct sockaddr *) &server, length) < 0)
    armci_die("armci_CreateSocketAndBind: bind failed", 0);

  /* Find out port number etc. */

  if (getsockname(*sock, (struct sockaddr *) &server, &length) < 0)
    armci_die("armci_CreateSocketAndBind: getsockname failed",  0);

  *port = ntohs(server.sin_port);

}


/*\ listen for socket connections
\*/
void armci_ListenSockAll(int* socklist, int num)
{
int i;

  if(num<0)armci_die("armci_ListenSockAll invalid number of sockets",num);

  for(i=0; i< num; i++){
     againlist:
       if (listen(socklist[i], num) < 0) {
         if (errno == EINTR)
           goto againlist;
         else
           armci_die("armci_ListenSockAll: listen failed",  0);
       }
  }

  if (DEBUG_) {
    (void) printf("process %d out of listen on %d sockets\n",armci_me,num);
    (void) fflush(stdout);
  }
}

void armci_tcp_get_sock_buf_size(int msgsock){
 int buffer_orig=32768,r;
 r = -1;
 while ( r < 0 && (tcp_sendrcv_bufsize > buffer_orig) ) {
 
  r= setsockopt(msgsock, SOL_SOCKET, SO_SNDBUF, (char *) &tcp_sendrcv_bufsize, sizeof(tcp_sendrcv_bufsize));
  r= setsockopt(msgsock, SOL_SOCKET, SO_RCVBUF, (char *) &tcp_sendrcv_bufsize, sizeof(tcp_sendrcv_bufsize));
  if( r < 0 ) tcp_sendrcv_bufsize =(tcp_sendrcv_bufsize/2);
  }
}

/*\  accept connections on the specified sockets
\*/
void armci_AcceptSockAll(int* socklist, int num)
{
  fd_set ready, fdzero;
  struct timeval timelimit;
  int maxsock, msgsock, nready, num_accept=0;
  int i;

  if(num<0)armci_die("armci_AcceptSockAll invalid number of sockets",num);

  /* Use select to wait for someone to try and establish a connection
     so that we can add a short timeout to avoid hangs */

  FD_ZERO(&fdzero);

againsel:
  FD_ZERO(&ready);

  /* we negate socket number on the list to mark already connected */
  maxsock=0;
  for(i=0; i<num; i++){
     if(socklist[i] > maxsock)maxsock = socklist[i]; /* find largest value*/
     if(socklist[i]>0) FD_SET(socklist[i], &ready);
/*     printf("%d: accepting socket%d=%d of %d\n",armci_me,i,socklist[i],num);*/
  }

  timelimit.tv_sec = TIMEOUT_ACCEPT;
  timelimit.tv_usec = 0;
  nready = select(maxsock+1, &ready, (fd_set *) NULL, (fd_set *) NULL,
                  &timelimit);

  /* error screening */
  if ( (nready <= 0) && (errno == EINTR) )
    goto againsel;
  else if (nready < 0)
    armci_die("armci_AcceptSockAll: error from select",nready);
  else if (nready == 0)
    armci_die("armci_AcceptSockAll:timeout waiting for connection",nready);

/*  if (bcmp(&ready,&fdzero,sizeof(fdzero)))*/
/*    armci_die("armci_AcceptSockAll: out of select but not ready!",nready);*/

  /* accept connection from newly contacted clients */
  for(i=0; i< num; i++){ 
    int sock = socklist[i];
    if(sock<0) continue; /* accepted already */
    if(!FD_ISSET(sock, &ready)) continue; /* not contacted yet */

    againacc:

      msgsock = accept(sock, (struct sockaddr *) NULL, (soclen_t *) NULL);
      if(msgsock==0){
        int msgsock2;
        msgsock2 = dup(msgsock);
        /*(void) CLOSE(msgsock);*/
        msgsock = msgsock2;
      }

      if (msgsock == -1) {
        if (errno == EINTR)
          goto againacc;
        else
          armci_die("armci_AcceptSockAll: accept failed",  msgsock);
      }

    if(DEBUG_){
       (void) printf("process %d out of accept socket=%d\n",armci_me,msgsock);
       (void) fflush(stdout);
    }

    /* Increase size of socket buffers to improve long message
       performance and increase size of message that goes asynchronously */
armci_tcp_get_sock_buf_size(msgsock);     

    armci_TcpNoDelay(msgsock);

    (void) CLOSE(sock); /* will not be needing this again */

    socklist[i] = -msgsock; /* negate connected socket on the list */

    num_accept++;
  }

  if(num_accept < num)
     goto againsel;

  for(i=0; i< num; i++) 
     if(socklist[i]>=0)
        armci_die("armci_AcceptSockAll: not connected",socklist[i]);
     else
        socklist[i] = - socklist[i];

}


int armci_ListenAndAccept(int sock)
/*
  Listen and accept a connection on the specified socket
  which was created with CreateSocketAndBind
*/
{
  fd_set ready;
  struct timeval timelimit;
  int msgsock, nready;
  int size = PACKET_SIZE;
  
againlist:
  if (listen(sock, 1) < 0) {
    if (errno == EINTR)
      goto againlist;
    else
      armci_die("armci_ListenAndAccept: listen failed",  0);
  }

  if (DEBUG_) {
    (void) printf("process %d out of listen on socket %d\n",armci_me,sock);
    (void) fflush(stdout);
  }

  /* Use select to wait for someone to try and establish a connection
     so that we can add a short timeout to avoid hangs */

againsel:
  FD_ZERO(&ready);
  FD_SET(sock, &ready);

  timelimit.tv_sec = TIMEOUT_ACCEPT;
  timelimit.tv_usec = 0;
  nready = select(sock+1, &ready, (fd_set *) NULL, (fd_set *) NULL,
		  &timelimit);
  if ( (nready <= 0) && (errno == EINTR) )
    goto againsel;
  else if (nready < 0)
    armci_die("armci_ListenAndAccept: error from select",  nready);
  else if (nready == 0)
    armci_die("armci_ListenAndAccept: timeout waiting for connection", nready);

  if (!FD_ISSET(sock, &ready))
    armci_die("armci_ListenAndAccept: out of select but not ready!",  nready);

againacc:
  msgsock = accept(sock, (struct sockaddr *) NULL, (soclen_t *) NULL);
  if (msgsock == -1) {
    if (errno == EINTR)
      goto againacc;
    else
      armci_die("armci_ListenAndAccept: accept failed",  msgsock);
  }

  if (DEBUG_) {
    (void) printf("process %d out of accept on socket %d\n", armci_me,msgsock);
    (void) fflush(stdout);
  }

  /* Increase size of socket buffers to improve long message
     performance and increase size of message that goes asynchronously */

  if(setsockopt(msgsock, SOL_SOCKET, SO_RCVBUF, (char *) &size, sizeof size))
    armci_die("armci_ListenAndAccept: error setting SO_RCVBUF",  size);
  if(setsockopt(msgsock, SOL_SOCKET, SO_SNDBUF, (char *) &size, sizeof size))
    armci_die("armci_ListenAndAccept: error setting SO_SNDBUF",  size);

  armci_TcpNoDelay(sock);

  (void) CLOSE(sock);     /* will not be needing this again */
  return msgsock;
}


int armci_CreateSocketAndConnect(char *hostname, int port)
/*
  Return the file descriptor of the socket which connects me to the
  remote process on hostname at port 

  hostname = hostname of the remote process
  port     =  port number of remote socket
*/
{
  int sock, status;
  struct sockaddr_in server;
  struct hostent *hp;
  int on = 1;
  int trial;
#if !defined(SGI) && !defined(WIN32)
  struct hostent *gethostbyname();
#endif

  /* Create socket */

  if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    (void) fprintf(stderr,"trying to connect to host=%s, port=%d\n",
                   hostname, port);
    armci_die("armci_CreateSocketAndConnect: socket failed",  sock);
  }

  /* the following can be disabled */
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 
		 (char *) &on, sizeof on) == -1)
	armci_die("armci_CreateSocketAndConnect: error setting REUSEADDR",  -1);

  /* Connect socket */
  server.sin_family = AF_INET;
  hp = gethostbyname(hostname);
  if (hp == 0) {
    (void) fprintf(stderr,"trying to connect to host=%s, port=%d\n",
                   hostname, port);
    armci_die("armci_CreateSocketAndConnect: gethostbyname failed", 0);
  }

  bcopy((char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
  server.sin_port = htons((unsigned short) port);

  trial = 0;
againcon:
  if ((status = 
     connect(sock, (struct sockaddr *) &server, sizeof server)) < 0) {
    if (errno == EINTR)
      goto againcon;
    else if(trial>CONNECT_TRIALS){
      
           (void) fprintf(stderr,"%d:trying connect to host=%s, port=%d t=%d %d\n",
                   armci_me,hostname, port,trial,errno);
           perror("trying to connect:");
           armci_die("armci_CreateSocketAndConnect: connect failed", status);
       }else {

         trial++;
         sleep(1);
         goto againcon;
       }
  }
  
  /* Increase size of socket buffers to improve long message
     performance and increase size of message that goes asynchronously */

  armci_tcp_get_sock_buf_size(sock); 
  armci_TcpNoDelay(sock);

  return sock;
}
