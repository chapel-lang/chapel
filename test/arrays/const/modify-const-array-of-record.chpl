const D = {1..4};

record R {
  var val: int;
  proc ref addOne() do val += 1;
}

// compiler correctly errors here
const r = new R();
r.addOne();

// compiler fails to error here
const A: [D] R;
A[1].addOne();
A.addOne();
var Slice = {2..3};
A[Slice].addOne();
// handled by tests in test/promotion/overArrayOfIndices, but is also a const violation
// var B = [1,3,1];
// A[B].addOne();

