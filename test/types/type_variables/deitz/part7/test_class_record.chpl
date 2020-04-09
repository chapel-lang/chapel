class C {
  type t;
  var x : t;
}

record R {
  var y : int;
}

var c = new unmanaged C(R);

writeln(c);

delete c;
