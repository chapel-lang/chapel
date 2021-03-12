use LinearAlgebra;
use Random;
use TestUtils;

var A = Matrix(
    [1.0, 3.0, 2.0],
    [5.0, 2.0, 0.0]
);
var B = pinv(A);

var npB = Matrix(
    [-0.09122807,  0.20701754],
    [ 0.22807018, -0.01754386],
    [ 0.20350877, -0.07719298]
);

assertAlmostEqual(B, npB, "Yes");

var Acalc = dot(A, dot(B, A));

assertAlmostEqual(A, Acalc, "Yes");

var A2 = Matrix(
    [18.0, 22.0, 54.0],  
    [42.0, 22.0, 70.0],
    [86.0, 62.0, 54.0],
    [86.0, 174.0, 134.0],
    [42.0, 62.0, 134.0],
    [53.0, 88.0, 93.0]
);

var npB2 = Matrix(
    [-0.00088878,  0.00574856,  0.01593542, -0.00438144, -0.00403594,
        -0.00093537],
    [-0.00229485, -0.00650944, -0.00441327,  0.00906565, -0.00409677,
        0.00163516],
    [ 0.00337342,  0.00348255, -0.00419068, -0.00267184,  0.00805934,
        0.00084332]
);

var B2 = pinv(A2);
assertAlmostEqual(B2, npB2, "Yes");

var A2calc = dot(A2, dot(B2, A2));

assertAlmostEqual(A2, A2calc, "Yes");

config const n = 20;
config const m = 15;

var A1: [0..#m, 0..#n] real;

forall i in 0..#m {
    fillRandom(A1[i, ..]);
}

// writeln(A1);

var B1 = pinv(A1);

var A1calc = dot(A1, dot(B1, A1));

assertAlmostEqual(A1, A1calc, "Yes");