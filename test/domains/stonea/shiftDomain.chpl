var D = {1..5, 1..5};
var D2 = D.translate(2, 3);
//
// +/- on domains is now illegal
// Below is the test as originally written.
//
// var D2 = (2, 3) + {1..5, 1..5};
// Should be the same as:
// 
// var D2 = [D.dim(0).low+2..D.dim(1).high+2,
//           D.dim(0).low+3..D.dim(1).high+3];
writeln(D2);
