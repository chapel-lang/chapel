// check that 'foreach' works with forall intents since forall intents are
// currently implemented by using varargs

var x = 0;
foreach i in 1..10 with (ref x) {
  x = i;
  writeln((i, x));
}
writeln(x);

foreach i in zip(1..10, 11..20) with (ref x) {
  x = i(1);
  writeln((i, x));
}
writeln(x);

foreach i in zip(11..20, 21..30) with (ref x) {
  x = i(1);
  writeln((i, x));
}
writeln(x);

