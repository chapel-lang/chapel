use BlockDist;

var A = newBlockArr(0..10, int);
var B = newBlockArr(0..10, int);
var someOther = newBlockArr(0..10, int);


forall (a,b) in zip(A,B) {
  b = someOther[a];
}

writeln(B);

