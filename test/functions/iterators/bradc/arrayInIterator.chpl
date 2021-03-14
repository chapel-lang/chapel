
proc main() {
  for x in testme() {
    writeln("x is: ", x);
  }
}


iter testme() {
  for i in 1..10 do
    yield i: real;

  var x: [1..10] real;

  for i in 1..10 do
    x[i] = i;

  for i in 1..10 do
    yield x[i];
}
