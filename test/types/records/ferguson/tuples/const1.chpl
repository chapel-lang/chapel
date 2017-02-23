use myrecord;

proc f(x ...) {
  for param i in 1..x.size do
    writeln(x(i).x);
}
proc f2( x ...) {
  f((...x));
}


proc g(const x ...) {
  for param i in 1..x.size do
    writeln(x(i).x);
}
proc g2(const x ...) {
  g((...x));
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

  writeln("Calling g()");
  g(a);
  g(a,b);

  writeln("Calling g2()");
  g2(a);
  g2(a,b);

}

test();
verify();
