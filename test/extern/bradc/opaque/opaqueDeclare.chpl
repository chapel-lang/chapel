extern proc getPtr(): opaque;
extern proc printPtr(ptr: opaque);

var x: opaque = getPtr();
printPtr(x);
