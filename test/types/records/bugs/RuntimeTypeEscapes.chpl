
record r {
  type T;
  var x: T;
}

proc test() {
  type D = domain(1);
  type X = r(T=D);
  var obj: X;
  writeln(obj);
}
test();

