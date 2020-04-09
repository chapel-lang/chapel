class C {
  var a,b,c: int;
  proc foo(ref i: int) {
    i = a*b*c;
  }
}

var c = new unmanaged C(1,2,3);
var i: int;

c.foo(i);

writeln(i);

delete c;
