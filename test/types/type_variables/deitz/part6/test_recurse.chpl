class C {
  type t;
  var x : t;
  var n : C(t);
}

var c = new unmanaged C(int);

writeln(c);

delete c;
