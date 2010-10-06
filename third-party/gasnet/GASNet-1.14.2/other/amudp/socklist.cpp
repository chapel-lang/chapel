//   $Source: /var/local/cvs/gasnet/other/amudp/socklist.cpp,v $
//     $Date: 2006/04/20 23:45:12 $
// $Revision: 1.5 $
// Description: 
// Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>

#include <assert.h>
#include "socklist.h"

#ifdef SOCKLIST_MT
  semaphore SocketList::lock("socketlist"); 
  #define LOCK   do { if (TS) lock.wait(); } while (0)
  #define UNLOCK do { if (TS) lock.signal(); } while (0)
#else
  #define LOCK
  #define UNLOCK
#endif

SocketList::SocketList(unsigned long maxsize, bool ThreadSafe) {
  size = maxsize;
  table = new SOCKET[maxsize];
  count = 0;
  maxfd = 0;
  FD_ZERO(&prvSet);
  TS = ThreadSafe;
}

SocketList::~SocketList() {
  delete table;
}

void SocketList::clear() {
  LOCK;
  count = 0;
  maxfd = 0;
  FD_ZERO(&prvSet);
  UNLOCK;
  return;
}

bool SocketList::insert(SOCKET s) {
  LOCK;
  if (count >= size) {
    UNLOCK;
    return false;
  }
  if (prvlookup(s)) {
    UNLOCK;
    return false;
  }
  table[count] = s;
  count++;
  if (s > maxfd) maxfd = s;
  FD_SET(s, &prvSet);
  UNLOCK;
  return true;
}

bool SocketList::remove(SOCKET s) {
  LOCK;
  for (unsigned long i=0; i < count; i++) {
    if (table[i] == s) {
      table[i] = table[--count];
      FD_CLR(s, &prvSet);
      if (maxfd == s) {
        maxfd = 0;
        for (unsigned long j=0; j < count; j++) {
          if (table[j] > maxfd) maxfd = table[j];
        }
      }
      UNLOCK;
      return true;
    }
  }
  UNLOCK;
  return false;
}

SOCKET SocketList::operator[](unsigned long  index) { // note: data gets rearranged on insert/remove
  assert(index < count && !TS);
  return table[index];
}

bool SocketList::prvlookup(SOCKET s) {
  for (unsigned long i=0; i < count; i++) if (table[i] == s) return true;
  return false;
}

bool SocketList::lookup(SOCKET s) {
  LOCK;
  bool retval = prvlookup(s);
  UNLOCK;
  return retval;
}

fd_set* SocketList::makeFD_SET(fd_set* set) {
  LOCK;
  memcpy(set, &prvSet, sizeof(fd_set));
  UNLOCK;
  return set;  
}

int SocketList::getIntersection(fd_set* set, SOCKET* buffer, int bufsiz) {
  getIntersection(set, buffer, &bufsiz);
  return bufsiz;
}

SOCKET* SocketList::getIntersection(fd_set* set, SOCKET* buffer, int* sz) {
  LOCK;
  // writes a list of sockets that appear in this list and set into buffer
  unsigned long cur = 0;
  for (unsigned long i=0; i < count && cur < (unsigned long)*sz; i++) {
    if (FD_ISSET(table[i], set)) {
      buffer[cur++] = table[i];
    }
  }
  *sz = cur;
  UNLOCK;
  return buffer;
}
   
SocketList::SocketList(SocketList& other) { // copy ctr
  #ifdef SOCKLIST_MT
    int locked = (TS || other.TS);
    if (locked) lock.wait();
  #endif
  count = other.count;
  maxfd = other.maxfd;
  size = other.size;
  table = new SOCKET[size];
  for (unsigned long i=0; i < count; i++) {
    table[i] = other.table[i];
  }
  memcpy(&prvSet,&other.prvSet,sizeof(fd_set));
  #ifdef SOCKLIST_MT
    if (locked) lock.signal();
  #endif
}

