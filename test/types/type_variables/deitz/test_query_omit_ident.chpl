class C {
  type t = int;
  var x: t;
}

var c1 = new C(int);
var c2 = new C(real);

proc foo(c: C(?)) {
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

delete c1;
delete c2;
