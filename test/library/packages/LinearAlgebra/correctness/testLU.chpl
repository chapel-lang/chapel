use LinearAlgebra;
use Random;
proc getLU(A: [?Adom]) {
    const (n,m) = Adom.shape;
    var (luMatrix,pivots) = lu(A);
    var U = triu(luMatrix);
    var LRect = luMatrix-U+eye(Adom);
    var L = LRect[1..n,1..n];
    return (L,U,pivots);
}

proc verifyLU(A, L, U, pivots) {
    var Arecon = dot(L,U);
    Arecon = permute(pivots,Arecon);
    var isSame = & reduce(Arecon==A);
    return isSame;
}

proc printLU(L, U) {
    writeln("L is :");
    writeln(L);
    writeln("U is :");
    writeln(U);
}

{
    var dom = {1..3,1..3};
    var A:[dom] real;
    A[1,1] = 1.0;
    A[1,2] = 2.0;
    A[1,3] = 3.0;
    A[2,1] = 4.0;
    A[2,2] = 5.0;
    A[2,3] = 6.0;
    A[3,1] = 8.0;
    A[3,2] = 10.0;
    A[3,3] = 12.0;
    var (L,U,pivots) = getLU(A);
    if(!verifyLU(A,L,U,pivots)) then
        writeln("LU decomposition of the matrix has failed");
    else
        printLU(L,U);
}
{
    var dom = {1..3,1..4};
    var A:[dom] real;
    A[1,1] = 1.0;
    A[1,2] = 2.0;
    A[1,3] = -3.0;
    A[1,4] = 1.0;
    A[2,1] = 2.0;
    A[2,2] = 4.0;
    A[2,4] = 7.0;
    A[3,1] = -1.0;
    A[3,2] = 3.0;
    A[3,3] = 2.0;
    var (L,U,pivots) = getLU(A);
    if(!verifyLU(A,L,U,pivots)) then
        writeln("LU decomposition of the matrix has failed");
    else
        printLU(L,U);
}
{
    var dom = {1..5,1..5};
    var A:[dom] real;
    A[1,1] = 3.0;
    A[1,4] = 2.0;
    A[2,2] = 5.0;
    A[2,3] = 1.0;
    A[3,2] = 1.0;
    A[3,3] = 2.0;
    A[3,4] = 7.0;
    A[3,5] = 9.0;
    A[4,1] = 2.0;
    A[4,3] = 7.0;
    A[4,4] = 1.0;
    A[5,3] = 9.0;
    A[5,5] = 4.0;
    var (L,U,pivots) = getLU(A);
    if(!verifyLU(A,L,U,pivots)) then
        writeln("LU decomposition of the matrix has failed");
    else
        printLU(L,U);
}
{
    var dom = {1..3,1..3};
    var A:[dom] real;
    A[1,1] = 1.0;
    A[2,3] = 1.0;
    A[3,2] = 1.0;
    var (L,U,pivots) = getLU(A);
    if(!verifyLU(A,L,U,pivots)) then
        writeln("LU decomposition of the matrix has failed");
    else
        printLU(L,U);
}
{
    var dom = {1..3,1..3};
    var A:[dom] real;
    fillRandom(A,seed=10);
    var (L,U,pivots) = getLU(A);
    if(!verifyLU(A,L,U,pivots)) then
        writeln("LU decomposition of the matrix has failed");
    else
        printLU(L,U);
}
{
    var dom = {1..3,1..3};
    var A:[dom] real;
    var (L,U,pivots) = getLU(A);
    if(!verifyLU(A,L,U,pivots)) then
        writeln("LU decomposition of the matrix has failed");
    else
        printLU(L,U);
}
{
    var dom = {1..1,1..10};
    var A: [dom] real;
    fillRandom(A, seed = 10);
     var (L,U,pivots) = getLU(A);
    if(!verifyLU(A,L,U,pivots)) then
        writeln("LU decomposition of the matrix has failed");
    else
        printLU(L,U);
}
