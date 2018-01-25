pragma "safe"
module l9 {
use OwnedObject;

class MyClass {
  var x:int;
}
record R {
  pragma "owned"
  var c:MyClass;
}


proc refIdentity(const ref x) const ref {
  return x;
}

proc badReturnRefIdentityArg(arg) const ref {
  return refIdentity(arg);
}

proc test1() {
  pragma "unsafe"
  var rr = new R(new MyClass(1));

  var infLifetime = rr.c;
  const ref r = badReturnRefIdentityArg(infLifetime);
  writeln(r);
}

proc bad(const ref r) const ref {
  var b = r.borrow();
  return refIdentity(b);
}

proc test2() {
  var r = new Owned(new MyClass(1));
  const ref re = bad(r);
  writeln(re);
}

test1();
test2();

}
