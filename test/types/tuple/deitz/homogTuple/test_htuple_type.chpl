var x: 3*int;
writeln(x);
class C {
  type t;
  var x: t;
}
var c = new unmanaged C(2*int);
writeln(c);
delete c;
