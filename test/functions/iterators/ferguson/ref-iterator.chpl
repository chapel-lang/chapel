var g:int;

iter a() ref {
  writeln("yield ref");
  yield g;
}

iter a() {
  writeln("yield value");
  yield g;
}

for i in a() {
  i = 1;
}

writeln(g);

var sum = 0;
for i in a() {
  sum += i;
}

writeln(sum);

