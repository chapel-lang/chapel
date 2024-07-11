enum E1 { red=10, blue=20, green=30 };
enum E2 { red=11, blue=22, green=33 };

proc foo(param one: bool) {
  type t = if one then E1 else E2;

  writeln(t.red : int);
}


foo(true);
foo(false);
