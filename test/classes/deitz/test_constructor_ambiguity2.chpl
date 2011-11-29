class C {
  type t;
  var x: t;
}

proc foo(type t) { writeln("in foo(type t)"); }

proc foo(c) { writeln("in foo(c)"); }

foo(C(int));
