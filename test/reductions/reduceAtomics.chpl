var A: [1..10] atomic int;

forall i in 1..10 do
  A[i].write(i);

var AofA: [1..3] [1..10] atomic int;

forall i in 1..3 do
  forall j in 1..10 do
    AofA[i][j].write(i*10 + j);

var B = + reduce A;
var C = + reduce AofA;

writeln(B);
writeln(C);


// It's annoying that I have to define these in order to run a +
// reduction over an array of atomic ints, but that seems to be
// the case at present...
/*
proc +(x: atomic int, y: atomic int) {
  return x.read() + y.read();
}

proc +=(ref x: int, y: atomic int) {
  x += y.read();
}

proc +=(X: [?D] int, Y: [D] atomic int) {
  forall i in D do 
    X[i] += Y[i].read();
}
*/
