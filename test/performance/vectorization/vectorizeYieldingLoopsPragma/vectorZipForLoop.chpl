// check that yielding for loops in explicitly vectorized iters are marked as
// order independent when invoked from a zippered loop

var A: [1..10] int;

for (i, j) in zip(myIter(), myIter()) {
  A[i] = j;
}
writeln(A);

for (i, j) in zip(myIterWithHelperLoop(), myIterWithHelperLoop()) {
  A[i] = 2*j;
}
writeln(A);


pragma "vectorize yielding loops"
iter myIter() {
  for i in 1..10 do yield i;
}

pragma "vectorize yielding loops"
iter myIterWithHelperLoop() {
  for i in 1..10 do;
  for i in 1..10 do yield i;
}
