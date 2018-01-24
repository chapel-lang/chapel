pragma "safe"
module l8 {
use OwnedObject;

class MyClass {
  var x:int;
}

proc refIdentity(const ref x) const ref {
  return x;
}

proc returnRefIdentityArg(arg) const ref {
  return refIdentity(arg);
}

proc test() {
  var c = new Owned(new MyClass(1));
  var borrow = c.borrow();
  const ref r = returnRefIdentityArg(borrow); 
  writeln(r);
}

test();
}
