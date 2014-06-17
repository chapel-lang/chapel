type cptr = opaque;

extern proc getPtr(): opaque;
extern proc printPtr(ptr: opaque);

{
  var x = getPtr();
  var y = x;
  printPtr(y);
}

{
  var x: opaque = getPtr();
  var y: opaque = x;
  printPtr(y);
}

{
  var x: cptr = getPtr();
  var y: cptr = x;
  printPtr(y);
}
