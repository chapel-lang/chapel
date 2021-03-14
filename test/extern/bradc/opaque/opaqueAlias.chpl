type cptr = opaque;

extern proc getPtr(): cptr;
extern proc printPtr(ptr: cptr);

var x: cptr = getPtr();
printPtr(x);
