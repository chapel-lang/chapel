use FFTW;

var err : real;

/* 1D tests go below */
var D = 0.. #100;
var A,B,goodA,goodB : [D] fftw_complex;
{
	var f = open("arr1d.dat",iomode.r).reader(kind=iokind.little);
	var n : int(32);
	f.read(n); 
	assert(n==100);
	for (r,c) in goodA {
		f.read(r);
		c = 0;
	}
	for (r,c) in goodB {
		f.read(r); f.read(c);
	}
	f.close();
}
var fwd = plan_dft_1d(100, A[0],B[0],1,FFTW_ESTIMATE);
var rev = plan_dft_1d(100, B[0],A[0],-1,FFTW_ESTIMATE);
// Test forward and reverse transform
A = goodA;
execute(fwd);
err = max reduce abs(B-goodB);
writeln(err);
execute(rev);
A /= 100; // FFTW does an unnormalized transform
err = max reduce abs(A-goodA);
writeln(err);
destroy_plan(fwd);
destroy_plan(rev);

// Test in-place transforms
fwd = plan_dft_1d(100,A[0],A[0],1,FFTW_ESTIMATE);
rev = plan_dft_1d(100,A[0],A[0],-1,FFTW_ESTIMATE);
A = goodA;
// Test forward and reverse transform
A = goodA;
execute(fwd);
err = max reduce abs(A-goodB);
writeln(err);
execute(rev);
A /= 100; // FFTW does an unnormalized transform
err = max reduce abs(A-goodA);
writeln(err);
destroy_plan(fwd);
destroy_plan(rev);





