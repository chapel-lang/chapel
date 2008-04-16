type cptr = opaque;

_extern def getPtr(): cptr;
_extern def printPtr(ptr: cptr);

var x: cptr = getPtr();
printPtr(x);
