use myrecord;

proc f(arg) {
  return arg;
}

var A:[1..3] R;
writeln(f(A));

// Does invoking f(A) cause the elements of A to be copied?
