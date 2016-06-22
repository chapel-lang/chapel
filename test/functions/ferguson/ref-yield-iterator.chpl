var g = 0;

iter a() ref {
  writeln("in a() ref");
  yield g;
}
iter a() {
  writeln("in a()");
  yield g;
}

for x in a() {
  x = 1;
}

for x in a() {
  writeln(x);
}
