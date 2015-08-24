// check that yielding for loops in explicitly vectorized iters are marked as
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
  for i in 1..10 do yield i;
}

pragma "vectorize yielding loops"
iter myIterWithHelperLoop() {
  for i in 1..10 do;
  for i in 1..10 do yield i;
}
