record R {
  var c: int;
  proc initialize () {c = 2;}
}

proc init (x: [] ?t) {
  for i in x.domain do {
    var y: t;
    x[i] = y;
  }
}

var a: [1..2] R;
init(a);
a[1].c = 1;
writeln(a);
writeln(a[1]);
writeln(a[1].c);
