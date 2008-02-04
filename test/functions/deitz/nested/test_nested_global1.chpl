class C {
  var x : int;
}

var c = new C(x = 2);

def foo(type t) {
  var x : t;
  def bar(c : C) {
    writeln(c);
  }
  writeln(x);
}

foo(int);
foo(real);
bar(c);
