//   $Source: /var/local/cvs/gasnet/other/amudp/socklist.h,v $
//     $Date: 2009/03/30 02:40:45 $
// $Revision: 1.5 $
// Description: 
// Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>

#ifndef _SOCKLIST_H
#define _SOCKLIST_H

#include "socket.h"
#include <stdlib.h>

#ifdef SOCKLIST_MT
  #include "semaphor.h"
#endif

class SocketList {
  private:
    SOCKET *table;
    unsigned long count;
    unsigned long size;
    unsigned long maxfd;
    fd_set prvSet;

    #ifdef SOCKLIST_MT
      static semaphore lock; 
    #endif
    int TS; // threadsafe?
    bool prvlookup(SOCKET s); // used for locking

  public:
    SocketList(unsigned long maxsize, bool ThreadSafe=false);
    SocketList(SocketList& other); // copy ctr
    ~SocketList();

    void clear();
    unsigned long getCount(void) { return count; }
    unsigned long getMaxFd(void) { return maxfd; }

    #ifdef SOCKLIST_MT
      void setThreadSafe(int ThreadSafe) { TS = ThreadSafe; }
    #endif

    bool insert(SOCKET s); // return TRUE on success, false otherwise (full or reinsert)
    bool remove(SOCKET s); // return TRUE on success, false otherwise (not present)
    bool lookup(SOCKET s); // return TRUE on success, false otherwise (not present)

    SOCKET operator[](unsigned long index); // note: data gets rearranged on insert/remove
      // this operator is disallowed for threadsafe lists

    fd_set* makeFD_SET(fd_set* set); // make an fd_set using contents (very efficient)
    SOCKET *getIntersection(fd_set* set, SOCKET* buffer, int* size); 
      // writes a list of sockets that appear in this list and set into buffer
      // size returns with num of values written, up to size
    int getIntersection(fd_set* set, SOCKET* buffer, int bufsiz);
      // same as above except return the number matched

};

#endif
