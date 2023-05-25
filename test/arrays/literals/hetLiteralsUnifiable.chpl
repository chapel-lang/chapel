var A = [true, 2, false, 4, 5];
var B = [1, 2.0, 3, 4.0, 5.0];
var C = [1, 2, true, 4, false];
var D = [1.0, 2, 3.0, 4, 5.0];
var E = [1, 2.3, 4.5i, 6.7 + 8.9i];
var F = [new owned Son(), new owned Daughter(), new owned Parent()];

writeln(A, ": ", A.type:string);
writeln(B, ": ", B.type:string);
writeln(C, ": ", C.type:string);
writeln(D, ": ", D.type:string);
writeln(E, ": ", E.type:string);
writeln(F, ": ", F.type:string);

class Parent { }
class Son: Parent { var x: int; }
class Daughter: Parent { var z: complex; }
