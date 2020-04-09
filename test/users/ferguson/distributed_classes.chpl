use CyclicDist;

class MyClass {
  var x: int;
}

const D = {1..10} dmapped Cyclic(startIdx=1);
var   A:[D] unmanaged MyClass?;

for i in 1..10 {
  on A[i] do A[i] = new unmanaged MyClass(i);
}

for i in 1..10 {
  writeln(i, " is stored on locale ", A[i].locale);
}

for a in A {
  delete a;
}
