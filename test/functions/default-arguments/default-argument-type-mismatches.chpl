// At one point, this program would cause foo()
// to run a promoted foo, but per issue #7940
// it now should be a compilation error.
proc foo(x: int = 1..3) {
  writeln(x);
}

foo();

proc int_string(x: int = "hello") {
}

int_string();

proc getinttype() type {
  return int;
}

proc nottype_type(x: int = getinttype()) {
}

nottype_type();
