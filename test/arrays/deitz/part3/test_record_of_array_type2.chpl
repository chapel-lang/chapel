record R {
  var A: [1..3] int;
}

proc f(type t) {
  var v: t;
  writeln(v);
}

f(R);
