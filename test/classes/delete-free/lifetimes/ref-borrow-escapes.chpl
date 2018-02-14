pragma "safe"
module refborrowescapes {
use OwnedObject;

class MyClass {
  var x:int;
}

proc refIdentity(const ref x) const ref {
  return x;
}

proc badReturnRefIdentityArg(arg) const ref {
  return refIdentity(arg);
}

proc test0() {
  var c = new Owned(new MyClass(1));
  var borrow = c.borrow();
  const ref r = badReturnRefIdentityArg(borrow);
  writeln(r);
}

test0();


record R {
  pragma "owned"
  var c:MyClass;
}

proc badReturnRefIdentityArg2(arg) const ref {
  return refIdentity(arg);
}

proc test1() {
  pragma "unsafe"
  var rr = new R(new MyClass(1));

  var infLifetime = rr.c;
  const ref r = badReturnRefIdentityArg2(infLifetime);
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
