// Test explicitly returning an iterator
iter myIter() {
  for i in 1..10 do
    yield i;
}

proc returnMyIter() {
  return myIter();
}

var x = returnMyIter();
writeln(x);


// Also test promoted variant.
proc addEm(A, B) {
  return A + B;
}

var A:[1..10] int;
var B:[1..10] int;
var C = addEm(A,B);
writeln(C);
