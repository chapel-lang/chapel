class C {
  type t;
  var x : t;
}

var c = C(int);

writeln(c);

def foo(i : int) {
  writeln(i);
}

foo(c);
