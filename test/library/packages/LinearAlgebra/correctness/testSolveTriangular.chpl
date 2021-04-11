use LinearAlgebra, Random;

const eps = 1e-7;

proc substituteAndCheckEqual(A, x, b){
    var b_hat = dot(A, x);
    var totalDiff = + reduce(abs(b-b_hat));
    return totalDiff < eps;
}

{
    var A: [1..3, 1..3] real;
    var b: [1..3] real;
    
    A[1,..] = [1.0, 0.0, 0.0];
    A[2,..] = [1.0, 2.0, 0.0];
    A[3,..] = [1.0, 3.0, 5.0];

    b = [1.0, 2.0, 3.0];

    var x = solve_tril(A, b, unit_diag=false);
    if(!substituteAndCheckEqual(A, x, b)) then
        writeln("Found incorrect solution");
    
    var y = solve_triu(A.T, b);
    if(!substituteAndCheckEqual(A.T, y, b)) then
        writeln("Found incorrect solution");
}

{
    var A: [1..5, 1..5] real;
    var b: [1..5] real;
    
    fillRandom(A, seed=10);
    fillRandom(b, seed=10);

    var (LU, ipiv) = lu(A);
    var L = tril(LU);

    var x = solve_tril(L, b, unit_diag=false);

    if(!substituteAndCheckEqual(L, x, b)) then
        writeln("Found incorrect solution");
    
    var U = triu(LU);
    var y = solve_triu(U, b);

    if(!substituteAndCheckEqual(U, y, b)) then
        writeln("Found incorrect solution");
}

{
    var A: [1..4, 1..4] real;
    var b: [1..4] real;

    // Lower Circulant Matrix
    A[1,..] = [1.0, 0.0, 0.0, 0.0];
    A[2,..] = [-0.5, 1.0, 0.0, 0.0];
    A[3,..] = [0.0, -0.5, 1.0, 0.0];
    A[4,..] = [0.0, 0.0, -0.5, 1.0];

    b = [1.0, -0.5, -0.5, 1.0];

    var x = solve_tril(A, b, unit_diag=true);

    if(!substituteAndCheckEqual(A, x, b)) then
        writeln("Found incorrect solution");
}
