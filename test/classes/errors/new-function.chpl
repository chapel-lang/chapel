class C { }

proc makeUnmanaged() {
  return new unmanaged C();
}

var zz = new owned makeUnmanaged();
var xx = new makeUnmanaged();
proc foo(a: int) {
  return a+1;
}
var a = new foo(1);
