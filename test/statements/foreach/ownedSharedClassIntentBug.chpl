class C {
  proc this(i) { return i; }
}

var c = new C();  // shared and owned is broken with foreach


var A: [1..10] int;
foreach i in 1..10 { // everything works with forall
  A[i] = c[i];
}
writeln(A);
