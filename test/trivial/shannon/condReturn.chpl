class C {
  var m = 2.3;
}

proc buildC() {
  return new owned C();
}

proc foo(x: int ...?numargs) {
  writeln("In foo()");
  if (numargs > 1) {
    return buildC();
  } else {
    halt("not enough args");
    return none;
  }
}

writeln(foo(2,2));
writeln(foo(2));
