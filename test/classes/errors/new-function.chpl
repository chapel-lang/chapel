class C { }

proc makeUnmanaged() {
  return new unmanaged C();
}

var zz = new owned makeUnmanaged();
var xx = new makeUnmanaged();
