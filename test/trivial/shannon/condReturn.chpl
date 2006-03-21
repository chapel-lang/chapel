class C {
  var m = 2.3;
}

function buildC() {
  return C();
}

function foo(x: int ...?numargs) {
  writeln("In foo()");
  if (numargs > 1) {
    return buildC();
  } else {
    halt("not enough args");
  }
}

writeln(foo(2,2));
writeln(foo(2));

