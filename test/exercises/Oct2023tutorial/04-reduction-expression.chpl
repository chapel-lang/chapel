// examples of reduction expressions from slide deck 04

config const n = 10,
             m = 20;

const Arr = [42, 5, 7, 3];

const total = + reduce Arr,
      factN = * reduce {1..n},
      biggest = max reduce (forall i in myIter() do foo(i));

writeln("total = ", total);
writeln("factN = ", factN);
writeln("biggest = ", biggest);

iter myIter() {
  for j in 1..m do yield j; 
  //forall j in 1..m do yield j;        // FIXME: why does this get a compile-time error?
}

proc foo(v) {
  return v+v;
}
