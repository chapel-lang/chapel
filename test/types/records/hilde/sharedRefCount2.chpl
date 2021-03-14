// sharedRefCount1.chpl
//
// To show that failing to bump the reference count results in premature
// deletion of the implementation.
//
use refCountedType;

proc escape() { 
  var hndl: Handle = new Handle(2.0);
  return hndl;
}

proc testit(hndl: Handle)
{
  // The contents of the handle should not be freed at this point.
  if hndl.freed == false then
    writeln("Passed.");
  else
    writeln("Failed.");
}

var hndl:Handle = new Handle(1.0);

hndl = escape();

testit(hndl);
