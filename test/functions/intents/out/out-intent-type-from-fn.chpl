
// Does the type of an out argument
// come from the call site or from
// the function body?
proc foo(out a) {
  a = 1;
  writeln("foo(out a) ; a.type is ", a.type:string);
}

proc bar(in a:real(32), out b) {
  b = a;
  writeln("bar(in a, out b) ; b.type is ", b.type:string);
}

proc baz(in a, out b) {
  b = a;
  writeln("baz(in a, out b) ; b.type is ", b.type:string);
}


proc main() {
  var dd: real;
  writeln(dd);

  var x: real = 0.0;
  foo(x);

  var r32: real(32) = 1.0;
  bar(r32, x);

  baz(r32, x);
}
