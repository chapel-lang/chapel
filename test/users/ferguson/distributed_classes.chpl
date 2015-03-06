use CyclicDist;

class MyClass {
  var x: int;
}

const D = {1..10} dmapped Cyclic(startIdx=1);
var   A:[D] MyClass;

for i in 1..10 {
  on A[i] do A[i] = new MyClass(i);
}

for i in 1..10 {
  writeln(i, " is stored on locale ", A[i].locale);
}

