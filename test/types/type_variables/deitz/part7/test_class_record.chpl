class C {
  type t;
  var x : t;
}

record R {
  var y : int;
}

var c = C(R);

writeln(c);
