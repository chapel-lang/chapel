use FFTW;

var err : real;

/* Read in the data and set up the domains appropriately */
param ndim : int = 1;
var dims : ndim*int(32); 
var D : domain(ndim);
// Define ranges here
var rD,cD,reD,imD : domain(ndim,int,true); 
var A,B,goodA,goodB : [D] fftw_complex;
{
	var f = open("arr1d.dat",iomode.r).reader(kind=iokind.little);
	for ii in 1..ndim {
		f.read(dims(ii));
	}
	D = 0.. #dims(1);
	for (r,c) in goodA {
		f.read(r);
		c = 0;
	}
	for (r,c) in goodB {
		f.read(r); f.read(c);
	}
	f.close();
}
// Set up domains based on dimension size
if (ndim==1) {
	// Domains for real FFT
	rD = 0.. #(2*(dims(1)/2 + 1)); // Padding to do in place transforms
	cD = 0.. #(dims(1)/2 + 1);
	// Define domains to extract the real and imaginary parts
	reD = rD[0..(2*(dims(1)/2)+1) by 2]; // Padding to do in place transforms
	imD = rD[1..(2*(dims(1)/2) + 1) by 2]; // Padding to do in place transforms
}

// FFTW does not normalize inverse transform, set up norm
var norm = * reduce dims;


// FFT testing here
var fwd = plan_dft_1d(dims, A[0],B[0],FFTW_FORWARD,FFTW_ESTIMATE);
var rev = plan_dft_1d(dims, B[0],A[0],FFTW_BACKWARD,FFTW_ESTIMATE);
// Test forward and reverse transform
A = goodA;
execute(fwd);
err = max reduce abs(B-goodB);
writeln(err);
execute(rev);
A /= norm; 
err = max reduce abs(A-goodA);
writeln(err);
destroy_plan(fwd);
destroy_plan(rev);

// Test in-place transforms
fwd = plan_dft_1d(dims,A[0],A[0],FFTW_FORWARD,FFTW_ESTIMATE);
rev = plan_dft_1d(dims,A[0],A[0],FFTW_BACKWARD,FFTW_ESTIMATE);
A = goodA;
// Test forward and reverse transform
A = goodA;
execute(fwd);
err = max reduce abs(A-goodB);
writeln(err);
execute(rev);
A /= * reduce dims; // FFTW does an unnormalized transform
err = max reduce abs(A-goodA);
writeln(err);
destroy_plan(fwd);
destroy_plan(rev);

// Testing r2c and c2r
var rA : [rD] real(64);
var cB : [cD] fftw_complex;
fwd = plan_dft_r2c_1d(dims,rA[0],cB[0],FFTW_ESTIMATE);
rev = plan_dft_c2r_1d(dims,cB[0],rA[0],FFTW_ESTIMATE);
rA[D] = re(goodA);
execute(fwd);
err = max reduce abs(cB - goodB[cD]);
writeln(err);
execute(rev);
rA /= norm;
err = max reduce abs(rA[D] - re(goodA));
writeln(err);
destroy_plan(fwd);
destroy_plan(rev);
// In place transform
fwd = plan_dft_r2c_1d(dims,rA[0],rA[0],FFTW_ESTIMATE);
rev = plan_dft_c2r_1d(dims,rA[0],rA[0],FFTW_ESTIMATE);
rA[D] = re(goodA);
execute(fwd);
err = (max reduce abs(rA[reD] - re(goodB[cD]))) + (max reduce abs(rA[imD] - im(goodB[cD])));
writeln(err);
execute(rev);
rA /= norm;
err = max reduce abs(rA[D] - re(goodA));
writeln(err);
destroy_plan(fwd);
destroy_plan(rev);





