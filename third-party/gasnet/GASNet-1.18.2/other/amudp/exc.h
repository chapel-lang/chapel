//   $Source: /var/local/cvs/gasnet/other/amudp/exc.h,v $
//     $Date: 2009/03/30 02:40:45 $
// $Revision: 1.4 $
// Description: Exception handlers for fserver
// Copyright 1999, Dan Bonachea & Scott McPeak

#ifndef _EXC_H
#define _EXC_H

#include "socket.h"

void breaker(void);

// by using this macro, the debugger gets a shot before the stack is unwound
#ifdef THROW
#undef THROW
#endif
#define THROW(obj) \
  { breaker(); throw (obj); }


// this function returns true if we're in the process of unwinding the
// stack, and therefore destructors may want to avoid throwing new exceptions;
// for now, may return false positives, but won't return false negatives
bool unwinding();

// inside a catch expression, the unwinding() function needs a tweak; pass
// the caught expression, and this returns whether there any *additional*
// exceptions currently in flight
class xBase;
bool unwinding_other(xBase const &x);

// using unwinding() in destructors to avoid abort()
#define CAUTIOUS_RELAY           \
  catch (xBase &x) {             \
    if (!unwinding_other(x)) {   \
      throw;   /* re-throw */    \
    }                            \
  }

#define MAX_EXC_MSG 1024
//------------------------------------------------------------------------------------
// intent is to derive all exception objects from this
class xBase {
  char msg[MAX_EXC_MSG];
    // the human-readable description of the exception

public:
  static int creationCount;
    // current # of xBases running about; used to support unrolling()

  xBase(char const *m);    // create exception object with message 'm'
  xBase(xBase const &obj);   // copy ctor
  virtual ~xBase();

  char const *why() const
    { return msg; }

};
//------------------------------------------------------------------------------------
// exception thrown by SOCKET funcs
class xSocket : public xBase {
public:
  SOCKET socket;       // socket on which failure occurred
  // all xSocket exceptions imply that the socket has been closed

  // error condition ctor
  xSocket(SOCKET s, char const *msg);

  xSocket(xSocket const &obj);
  ~xSocket();
};

void xsocket(SOCKET s, char const *msg); // creates and throws an xsocket

//------------------------------------------------------------------------------------
// exception thrown top indicate a form field problem
class xForm : public xBase {
public:
  // error condition ctor
  xForm(char const *m);    // create exception object with message 'm'
  xForm(xForm const &obj);   // copy ctor
  ~xForm();
};


#endif // _EXC_H

