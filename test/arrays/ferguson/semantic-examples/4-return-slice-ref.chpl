use myrecord;

proc f() ref {
  var A:[1..4] R;
  return A[2..3];
}

writeln(f());


