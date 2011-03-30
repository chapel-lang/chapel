_extern proc getPtr(): opaque;
_extern proc printPtr(ptr: opaque);

var x: opaque = getPtr();
printPtr(x);
