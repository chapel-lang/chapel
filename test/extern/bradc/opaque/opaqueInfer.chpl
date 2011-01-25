_extern proc getPtr(): opaque;
_extern proc printPtr(ptr);

var x = getPtr();
printPtr(x);
