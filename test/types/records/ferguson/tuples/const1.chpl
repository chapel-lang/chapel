use myrecord;

proc f(x ...?k) {
  for param i in 0..k-1 do
    writeln(x(i).x);
}
proc f2( x ...?k, out error:int) {
  f((...x));
}
proc f2( x ...?k) {
  f((...x));
}
proc f3(x, y) {
  f2(x, y);
}
proc f4(tup: 2*R) {
  f3(tup(0), tup(1));
}




proc g(const x ...?k) {
  for param i in 0..k-1 do
    writeln(x(i).x);
}
proc g2(const x ...?k, out error:int) {
  g((...x));
}
proc g2(const x ...?k) {
  g((...x));
}
proc g3(const x, const y) {
  g2(x, y);
}
proc g4(const tup: 2*R) {
  g3(tup(0), tup(1));
}

proc test() {

  var a:R;
  a.setup(x=10);
  a.verify();
  var b:R;
  b.setup(x=20);
  b.verify();

  writeln("Calling f()");
  f(a);
  f(a,b);

  writeln("Calling f2()");
  f2(a);
  f2(a,b);

  writeln("Calling f3()");
  f3(a,b);

  writeln("Calling f4()");
  f4((a,b));

  writeln("Calling g()");
  g(a);
  g(a,b);

  writeln("Calling g2()");
  g2(a);
  g2(a,b);

  writeln("Calling g3()");
  g3(a,b);

  writeln("Calling g4()");
  g4((a,b));
}

test();
verify();
