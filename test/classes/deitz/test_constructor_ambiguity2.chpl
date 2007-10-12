class C {
  type t;
  var x: t;
}

def foo(type t) { writeln("in foo(type t)"); }

def foo(c) { writeln("in foo(c)"); }

foo(C(int));
