// By analogy to the following:
//
//   var A: [D] real;
//   var B = A;
//   D = ...;
//   ==> B.domain resizes, too
//
// the domain of an array from a promoted- or forall-expression
// should resize when the source domain resizes.

var D = {1..3};
var DC = {1..3};

var B: [D] int;
var C: [DC] int;

var A1 = B + C;
var A2 = [idx in D] idx;

writeln("created As => ", A1.domain, A2.domain, B.domain, C.domain);
writeln(" A2 = ", A2);

D = {1..5};
writeln("updated D =>  ", A1.domain, A2.domain, B.domain, C.domain);
writeln(" A2 = ", A2);

DC = {1..7};
writeln("updated DC => ", A1.domain, A2.domain, B.domain, C.domain);
