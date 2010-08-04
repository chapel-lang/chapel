//   $Source: /var/local/cvs/gasnet/other/amudp/sockutil.h,v $
//     $Date: 2006/04/16 09:28:42 $
// $Revision: 1.5 $
// Description: Simple sock utils
// Copyright 1999, Dan Bonachea

#ifndef SOCKUTIL_H
#define SOCKUTIL_H

#include <portable_inttypes.h>

#include "socket.h"
#include "sockaddr.h"
#include "exc.h"

//-------------------------------------------------------------------------------------
bool socklibinit();
bool socklibend();

bool isValidIP(const char* buf);
bool isLittleEndian();
bool isBigEndian();

SOCKET listen_socket(unsigned short port, bool allowshared=false); // on any external interface
SOCKET listen_socket(struct sockaddr* saddr, bool allowshared=false);
  // create a socket to listen to a specific port (throw exn on err)

SOCKET accept_socket(SOCKET listener, struct sockaddr* calleraddr=0);
  // accept a new connection from a listener, blocking until one is available
  // optionally return caller's address

SOCKET connect_socket(struct sockaddr* saddr);
SOCKET connect_socket(char* addr);
  // create a socket and connect it to a remote host/port (throw exn on err)

void recvAll(SOCKET s, void* buffer, int numbytes);
  // blocks until it can receive numbytes on s into buffer
  // (throws xSocket on close)

int recvLine(SOCKET s, char* buf, int bufsiz);
  // block until next line (terminated by CRLF) is received
  // returns size with CRLF removed
  // may throw exception if bufsiz is too small

void sendAll(SOCKET s, const void* buffer, int numbytes);
void sendAll(SOCKET s, const char* buffer, int numbytes=-1);
  // blocks until it can send numbytes on s from buffer
  // (throws xSocket on close)
void sendEOL(SOCKET s); // send CR LF

uint32_t recv32(SOCKET s);
void send32(SOCKET s, uint32_t value);
  // recv/send a 32-bit value with endian conversions
extern bool endianconvert; // whether or not conversions should happen on recv/send

uint16_t byteSwap16(uint16_t val);
uint32_t byteSwap32(uint32_t val);
uint64_t byteSwap64(uint64_t val);

uint16_t ntoh16(uint16_t v);
uint32_t ntoh32(uint32_t v);
uint64_t ntoh64(uint64_t v);
void ntoh16a(void *pv);
void ntoh32a(void *pv);
void ntoh64a(void *pv);
uint16_t hton16(uint16_t v);
uint32_t hton32(uint32_t v);
uint64_t hton64(uint64_t v);
void hton16a(void *pv);
void hton32a(void *pv);
void hton64a(void *pv);

char recvch(SOCKET s); // get one character

void waitForClose(SOCKET s); // blocks until close - throw exn if data received

bool inputWaiting(SOCKET s); // returns true if input or close conn is waiting

bool waitForActivity(SOCKET s, struct timeval* tv=NULL); // block until data arrives or close posts

int numBytesWaiting(SOCKET s); // returns number of bytes waiting to be received

// checks if a connection has been closed
bool isClosed(SOCKET s);

// checks if socket s has OOB data waiting
bool hasOOBdata(SOCKET s);

void close_socket(SOCKET s);
void closeGracefully(SOCKET s); // close a socket gracefully, blocking until everything is sent
 // note: does NOT throw exn, even though it closed it

// ------ error determination ------
char const *errorCodeString(int socket_error_code);
  // map an error code into a human-readable string, null-terminated but
  // with no newline, or NULL if the code is not understood; the returned
  // pointer must be treated as a pointer to a static area that will be
  // overwritten by the next call, even that is not necessarily true on
  // some systems

int getSocketErrorCode();
  // return the error code for use with errorCodeString; only defined after
  // a socket function returns an error code (usually either SOCKET_ERROR
  // or INVALID_SOCKET)

SockAddr getsockname(SOCKET s);
SockAddr getpeername(SOCKET s);
unsigned long getLocalAddress(SOCKET s);
int getLocalPort(SOCKET s);
void getSockPeer(SOCKET s, sockaddr_in &addr);
unsigned long getRemoteAddress(SOCKET s);
int getRemotePort(SOCKET s);
void getSockName(SOCKET s, sockaddr_in &addr);

char const *getMyHostName();
  // return the local hostname

SockAddr DNSLookup(const char *hostnameOrIPStr);
  // get the sockaddr for a hostname
  // don't call on a local hostname, because there may be several interfaces 
  // and we may get the wrong one


#ifdef __cplusplus
  extern "C" {
#endif

  extern int myselect(int  n,  fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
              struct timeval *timeout);
  #define select myselect

  extern int myrecvfrom(SOCKET s, char * buf, int len, int flags,                  
                        struct sockaddr *from, int *fromlen);

#ifdef __cplusplus
    }
#endif

#endif
