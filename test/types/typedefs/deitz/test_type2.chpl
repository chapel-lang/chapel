class C {
  type t = int;
  var x: t;
}

var c = new unmanaged C();

writeln(c);

delete c;
