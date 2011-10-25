iter test(inout x:int) {
  writeln("starting test(", x, ")");
  x = 200;
  writeln("after assignment x=", x);
  for i in 1..10 {
    yield i;
  }
  x = 100;
}

var x:int = 5;

for i in test(x) {
  writeln((i, x));
}

writeln(x);
