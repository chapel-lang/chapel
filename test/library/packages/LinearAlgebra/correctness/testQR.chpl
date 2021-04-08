use LinearAlgebra;
use Math;

proc isSame(A: [?Adom], B) {
    var equal = true;
    for(valueFound, expectedValue) in zip(A, B) do
        equal = equal && Math.isclose(valueFound, expectedValue);
    return equal;
}

proc printQR(Q, R){
    writeln("Expected Q :");
    writeln(Q);
    writeln("Expected R :");
    writeln(R);
}

{
    var dom = {1..3,1..3};
    var A: [dom] real;
    A[1,1] = 1.0;
    A[1,2] = 2.0;
    A[1,3] = 3.0;
    A[2,1] = 4.0;
    A[2,2] = 5.0;
    A[2,3] = 6.0;
    A[3,1] = 7.0;
    A[3,2] = 8.0;
    A[3,3] = 9.0;

    var Q_values = [
        -0.123091, 0.904534, 0.408248,
        -0.492366, 0.301511, -0.816497,
        -0.86164, -0.301511, 0.408248
    ];

    var R_values = [
        -8.12404, -9.60114, -11.0782,
        0.0, 0.904534, 1.80907,
        0.0, 0.0, -8.88178e-16
    ];

    var Q_expected = reshape(Q_values, {1..3, 1..3});
    var R_expected = reshape(R_values, {1..3, 1..3});

    var (Q,R) = qr(A);

    if(!isSame(Q, Q_expected) || !isSame(R, R_expected)){
        writeln("Found wrong values for Q,R");
        printQR(Q_expected, R_expected);
    }

}
{
    var dom = {1..3,1..3};
    var A: [dom] real;
    A[1,1] = 1.0;
    A[1,2] = 1.0;
    A[2,1] = 1.0;
    A[2,3] = 1.0;
    A[3,2] = 1.0;
    A[3,3] = 1.0;

    var Q_values = [
        -0.707107, 0.408248, -0.57735,
        -0.707107, -0.408248, 0.57735,
        0.0, 0.816497, 0.57735
    ];

    var R_values = [
        -1.41421, -0.707107, -0.707107,
        0.0, 1.22474, 0.408248,
        0.0, 0.0, 1.1547
    ];

    var Q_expected = reshape(Q_values, {1..3, 1..3});
    var R_expected = reshape(R_values, {1..3, 1..3});

    var (Q,R) = qr(A);

    if(!isSame(Q, Q_expected) || !isSame(R, R_expected)){
        writeln("Found wrong values for Q,R");
        printQR(Q_expected, R_expected);
    }
}
{
    var dom = {1..4,1..4};
    var A: [dom] real;
    A = 1.0;
    A[1,2] = -1.0;
    A[1,4] = -1.0;
    A[2,3] = -1.0;
    A[2,4] = -1.0;
    A[4,3] = -1.0;
    A /= 2;

    var Q_values = [
        -0.5, 0.866025, 1.34049e-17, -3.59184e-18,
        -0.5, -0.288675, 0.408248, -0.707107,
        -0.5, -0.288675, -0.816497, -5.55112e-17,
        -0.5, -0.288675, 0.408248, 0.707107
    ];

    var R_values = [
        -1.0, -0.5, 0.0, 0.0,
        0.0, -0.866025, 0.57735, -0.57735,
        0.0, 0.0, -0.816497, -0.408248,
        0.0, 0.0, 0.0, 0.707107
    ];

    var Q_expected = reshape(Q_values, {1..4, 1..4});
    var R_expected = reshape(R_values, {1..4, 1..4});

    var (Q,R) = qr(A);

    if(!isSame(Q, Q_expected) || !isSame(R, R_expected)){
        writeln("Found wrong values for Q,R");
        printQR(Q_expected, R_expected);
    }
}
{
    var dom = {1..3,1..3};
    var A: [dom] real;

    var Q_values = [
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    ];

    var R_values = [
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0
    ];

    var Q_expected = reshape(Q_values, {1..3, 1..3});
    var R_expected = reshape(R_values, {1..3, 1..3});

    var (Q,R) = qr(A);

    if(!isSame(Q, Q_expected) || !isSame(R, R_expected)){
        writeln("Found wrong values for Q,R");
        printQR(Q_expected, R_expected);
    }
}
