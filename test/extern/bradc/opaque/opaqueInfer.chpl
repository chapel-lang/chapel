extern proc getPtr(): opaque;
extern proc printPtr(ptr);

var x = getPtr();
printPtr(x);
