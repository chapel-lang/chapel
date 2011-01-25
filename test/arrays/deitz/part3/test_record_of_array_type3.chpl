record R {
  var A: [1..3] int;
}

record RR {
  var r: R;
}

proc f(type t) {
  var v: t;
  writeln(v);
}

f(RR);
