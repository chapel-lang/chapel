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
  if hndl.value == 2.0 then
    writeln("Passed.");
  else
    writeln("Failed.");
}

var hndl:Handle = new Handle(1.0);

hndl = escape();

// Try to corrupt the memory freed in escape.
var i:Impl = new Impl();
i.value = 4.9;

testit(hndl);

delete i;

