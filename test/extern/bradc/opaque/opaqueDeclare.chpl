_extern def getPtr(): opaque;
_extern def printPtr(ptr: opaque);

var x: opaque = getPtr();
printPtr(x);
