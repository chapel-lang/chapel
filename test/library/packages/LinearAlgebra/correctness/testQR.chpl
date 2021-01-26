use LinearAlgebra;

proc isSame(A: [?Adom], B) {
    var epsilon = 1.0e-5;
    var Delta = abs(A-B);
    var delta = + reduce([i in Adom] Delta[i]);
    return delta < epsilon;
}

proc printQR(Q, R){
    writeln("Q :");
    writeln(Q);
    writeln("R :");
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

    var (Q,R) = qr(A);
    var Areconstructed = dot(Q,R);
    if(isSame(Areconstructed, A)) then
        printQR(Q,R);
    else
        writeln("QR Factorization has failed");

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

    var (Q,R) = qr(A);
    var Areconstructed = dot(Q,R);
    if(isSame(Areconstructed, A)) then
        printQR(Q,R);
    else
        writeln("QR Factorization has failed");
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

    var (Q,R) = qr(A);
    var Areconstructed = dot(Q,R);
    if(isSame(Areconstructed, A)) then
        printQR(Q,R);
    else
        writeln("QR Factorization has failed");
}
{
    var dom = {1..3,1..3};
    var A: [dom] real;

    var (Q,R) = qr(A);
    var Areconstructed = dot(Q,R);
    if(isSame(Areconstructed, A)) then
        printQR(Q,R);
    else
        writeln("QR Factorization has failed");
}