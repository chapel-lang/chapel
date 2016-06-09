var A:[1..3] int;

proc f(arg) {
  return arg; // copy created as part of returning
}

writeln(f(A));
