class C {
  type t = int;
  var x: t;
}

var c1 = new borrowed C(int);
var c2 = new borrowed C(real);

proc foo(c: borrowed C(?)) {
  writeln(c);
}

foo(c1);
foo(c2);

proc bar(x: ?) {
  writeln(x);
}

bar(c1);
bar(c2);
bar(3.14);
