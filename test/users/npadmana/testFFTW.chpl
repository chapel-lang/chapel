use FFTW;

var oneD : domain(1) = 0.. #5;
var A,B : [oneD] fftw_complex;
var plan1 = plan_dft_1d(5,A[0],B[0],1,FFTW_ESTIMATE);
// Planning destroys the arrays
A = [(1,0),(2,0),(3,0),(4,0),(5,0)];
writeln(A);
execute(plan1);
writeln(B); // Note that A may be erased
destroy_plan(plan1);
