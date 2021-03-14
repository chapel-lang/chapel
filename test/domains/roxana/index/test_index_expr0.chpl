config var n : int = 4;

var D : domain(2) = {1..n, 1..n};

var A : [D] real;

var iD : index(D);
//var i2 : index(2);

//forall (i,j) in D {
//  A(i,j) = 1;
//}

//writeln("A:\n", A);

[i in D] A(i) = 2;

writeln("A:\n", A);
