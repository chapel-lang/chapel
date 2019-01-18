foo();

proc foo() {
  bar(10, 20);
}

inline proc bar(a : int, b : int) {
  baz(a, b);
}

proc baz(const ref x : int, const ref y : int) {
  writeln('baz ', x, ' ', y);
}


/*
foo0();
foo1();

proc foo0() {
  var m = 10;
  var n = 20;

  bar(m, n);
}

proc foo1() {
  bar(10, 20);
}

inline proc bar(a : int, b : int) {
  baz(a, b);
}

proc baz(const ref x : int, const ref y : int) {
  writeln('baz ', x, ' ', y);
}
*/
