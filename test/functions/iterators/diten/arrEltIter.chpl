
iter f() {
  var Arr: [0..10] int;
  for (c, d) in zip (Arr, 0..10) {
    c = d; // nil dereference in assigning to "c"
  }
  for elem in Arr do
    yield elem;
}

proc main {
  for i in f() do
    writeln(i);
}
