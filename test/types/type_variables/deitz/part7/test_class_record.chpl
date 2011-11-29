class C {
  type t;
  var x : t;
}

record R {
  var y : int;
}

var c = new C(R);

writeln(c);

delete c;
