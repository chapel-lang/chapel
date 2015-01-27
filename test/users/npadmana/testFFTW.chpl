use FFTW;

var err : real;
var oneD : domain(1) = 0.. #5;
var A,B : [oneD] fftw_complex;
var plan1 = plan_dft_1d(5,A[0],B[0],1,FFTW_ESTIMATE);
// Planning destroys the arrays
A = [(1,0),(2,0),(3,0),(4,0),(5,0)];
var goodB = [(15.0,0.0),(-2.5,-3.44095480),(-2.5,-0.812299240),(-2.5,0.812299240),(-2.5,3.44095480)];
execute(plan1);
err = max reduce abs(B-goodB);
writeln(err < 1.e-6);
destroy_plan(plan1);
