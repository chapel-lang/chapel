class C {
  type t = int;
  var x: t;
}

var c1 = new C(int);
var c2 = new C(real);

def foo(c: C(?)) {
  writeln(c);
}

foo(c1);
foo(c2);

def bar(x: ?) {
  writeln(x);
}

bar(c1);
bar(c2);
bar(3.14);
