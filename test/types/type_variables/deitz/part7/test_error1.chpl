class C {
  type t;
  var x : t;
}

var c = new C(int);

writeln(c);

def foo(i : int) {
  writeln(i);
}

foo(c);
