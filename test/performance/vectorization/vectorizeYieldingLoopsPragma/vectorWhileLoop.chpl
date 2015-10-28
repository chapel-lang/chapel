// check that yielding while loops in explicitly vectorized iters are marked as
// order independent

var A: [1..10] int;

for i in myIter() {
  A[i] = i;
}
writeln(A);

for i in myIterWithHelperLoop() {
  A[i] = 2*i;
}
writeln(A);


pragma "vectorize yielding loops"
iter myIter() {
  var i = 1;
  while i <= 10 {
    yield i;
    i += 1;
  }
}

pragma "vectorize yielding loops"
iter myIterWithHelperLoop() {
  var i, j = 1;

  while i <= 10 {
    i += 1;
  }

  while j <= 10 {
    yield j;
    j += 1;
  }
}
