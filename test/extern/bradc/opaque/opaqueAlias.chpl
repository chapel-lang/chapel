type cptr = opaque;

_extern proc getPtr(): cptr;
_extern proc printPtr(ptr: cptr);

var x: cptr = getPtr();
printPtr(x);
