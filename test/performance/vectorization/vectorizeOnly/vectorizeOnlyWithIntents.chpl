// check that the vectorizeOnly works with forall intents since forall intents are
// currently implemented by using varargs

var x = 0;
forall i in vectorizeOnly(1..10) with (ref x) {
  x = i;
  writeln((i, x));
}
writeln(x);

forall i in vectorizeOnly(1..10, 11..20) with (ref x) {
  x = i(1);
  writeln((i, x));
}
writeln(x);

forall i in zip(vectorizeOnly(11..20, 21..30)) with (ref x) {
  x = i(1);
  writeln((i, x));
}
writeln(x);
