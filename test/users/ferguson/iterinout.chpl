

iter test(inout x:int) {
  for i in 1..10 {
    yield i;
  }
  x = 100;
}

var x:int;

for i in test(x) {
  writeln(i);
}
writeln(x);
