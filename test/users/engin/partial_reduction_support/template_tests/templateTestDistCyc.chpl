use CyclicDist;

config const N = 4;

const space = {0..#N, 0..#N, 0..#N};
const ParentDom = space dmapped Cyclic(space.low);
var arr: [ParentDom] int;

for (i,j,k) in ParentDom {
  arr[i,j,k] = i*N**2 + j*N**1 + k*N**0;
}

writeln(arr._value.dsiPartialReduce_templateopt(1));
writeln();
writeln(arr._value.dsiPartialReduce_templateopt(2));
writeln();
writeln(arr._value.dsiPartialReduce_templateopt(3));
