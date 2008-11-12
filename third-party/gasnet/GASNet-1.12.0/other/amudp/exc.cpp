//   $Source: /var/local/cvs/gasnet/other/amudp/exc.cpp,v $
//     $Date: 2006/04/10 04:20:12 $
// $Revision: 1.3 $
// Description: Exception handlers for fserver
// Copyright 1999, Dan Bonachea & Scott McPeak

#include "exc.h"          // this module
#include "sockutil.h"

#include <stdio.h>
#include <string.h>       // strlen, strcpy

//------------------------------------------------------------------------------------
void ackackack(int*) {}

void breaker() {
  static int i=0;
  int a=1;               // all this junk is just to make sure
                         // that this function has a complete
  ackackack(&a);         // stack frame, so the debugger can unwind
  i++;                   // the stack
}

// ------------------------- xBase -----------------
int xBase::creationCount = 0;

xBase::xBase(char const *m) {
  strncpy(msg, m, MAX_EXC_MSG);
  msg[MAX_EXC_MSG-1] = '\0';

  // done at very end when we know this object will
  // successfully be created
  creationCount++;
}


xBase::xBase(xBase const &obj){
  strncpy(msg, obj.msg, MAX_EXC_MSG);
  msg[MAX_EXC_MSG-1] = '\0';
  creationCount++;
}


xBase::~xBase() {
  creationCount--;
}


// this is obviously not perfect, since exception objects can be
// created and not thrown; I heard the C++ standard is going to,
// or already does, include (by this name?) a function that does this
// correctly; until then, this will serve as a close approximation
// (this kind of test is, IMO, not a good way to handle the underlying
// problem, but it does reasonably handle 70-90% of the cases that
// arise in practice, so I will endorse it for now)
bool unwinding() {
  return xBase::creationCount != 0;
}


// tweaked version
bool unwinding_other(xBase const &) {
  // we know the passed xBase exists.. any others?
  return xBase::creationCount > 1;
} 


//-------------------------------------------------------------------------------------
// ------------------- xSocket ----------------------------
xSocket::xSocket(SOCKET s, char const *msg)
  : xBase(msg),
    socket(s)
{}

xSocket::xSocket(xSocket const &obj)
  : xBase(obj),
    socket(obj.socket)
{}

xSocket::~xSocket()
{}

void xsocket(SOCKET s, char const *msg) { // creates and throws an xsocket
  char tmp[MAX_EXC_MSG];
  sprintf(tmp, "%s (%i:%s)", msg, getSocketErrorCode(), errorCodeString(getSocketErrorCode()));
  THROW(xSocket(s, tmp));
}

//-------------------------------------------------------------------------------------
// ------------------- xForm ----------------------------
xForm::xForm(char const *m) 
  : xBase(m) 
{}    // create exception object with message 'm'

xForm::xForm(xForm const &obj)
  : xBase(obj)
{}

xForm::~xForm()
{}
