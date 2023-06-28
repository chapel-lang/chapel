class C { var x: int; }
var A : [1..1] borrowed C = [new C(12345678), ];
assert(A[1].x == 12345678);
