var sum = 0;
var A: [1..10] int;

for (i, a) in zip(1..10, A) {
  a = i;
}

writeln(+ reduce A);
