var A: [1..5] int;
A(2) = 1;

class C {
  var a = A(2);
}

var c = new unmanaged C();
A(2) = 2;
var d = new unmanaged C();

writeln(c);
writeln(d);

delete c;
delete d;
