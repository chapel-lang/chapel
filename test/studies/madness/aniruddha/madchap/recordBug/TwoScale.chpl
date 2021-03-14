/** TwoScale Module:
  
    Tabulation of the twoscale coefficients, and a routine to compute the
    scaling functions (Legendre polyn scaled and shifted to the unit interval).
*/

//use Math;
private use IO;

config const hg_inputfile = "hg_coeffs.dat";

// FIXME: Module-Private Stuff
var hg_MaxK        : int;
var hg_coeff_space : domain(3) = {0..-1, 0..-1, 0..-1};
var hg_coeffs      : [hg_coeff_space] real;
var phi_norm_space : domain(1) = {0..-1};
var phi_norms      : [phi_norm_space] real;
var phi_initialized = false;


/** Read two scale coeffs from the input file.
 */
proc hg_readCoeffs(inputfile) {
    var max_k: int;
    var coeffData = open(inputfile, iomode.r).reader();

    coeffData.read(max_k);
    hg_MaxK = max_k;

    hg_coeff_space = {0..max_k, 0..2*max_k-1, 0..2*max_k-1};

    // hg_coeffs[0,..,..] is not used
    for i in 1..max_k {
        for (j, k) in {0..i*2-1, 0..i*2-1} {
            var x: real;
            coeffData.read(x);
            hg_coeffs[i, j, k] = x;
        }
    }

    coeffData.close();
}


/** Return maximum k.
 */
proc hg_getMaxK(): int {
    return hg_MaxK;
}


/** Return the twoscale coefficients for the multiwavelets of order k.
 */
proc hg_getCoeffs(k: int) {
    var kthCoeffs: [0..2*k-1, 0..2*k-1] real;
    // FIXME: Ideally I want to slice a 2d matrix out of the 3d matrix.
    //        Current copy code is a workaround.

    if k < 0 || k > hg_MaxK then
        halt("TwoScaleCoeffs.getCoeffs(): k=", k, " is out of bounds!");

    for (i, j) in kthCoeffs.domain do 
        kthCoeffs(i, j) = hg_coeffs(k, i, j);

    return kthCoeffs;
}


/** Evaluate the Legendre polynomials up to the given order at x
    defined on [-1,1]
    
     * x       point at which to evaluate polynomials
     * order   max order of polynomials to evaluate
     * return  [] real first k polynomials evaluated at point x
 */
proc pn(x: real, order: int) {
    var p: [0..order] real;

    p[0] = 1.0;
    if order == 0 then return p;
    p[1] = x;

    for n in 1..order-1 do
        p[n+1] = n*(x*p[n] - p[n-1])/(n+1) + x*p[n];
    return p;
}


/** Evaluate the shifted normalized Legendre polynomials up to the
    given order at x defined on [0,1].

    These are also our scaling functions, phi_i(x) , i=0..k-1

    In addition to forming an orthonormal basis on [0,1] we have
    phi_j(1/2-x) = (-1)^j phi_j(1/2+x)

    (the wavelets are similar with phase (-1)^(j+k)).

     * x       point at which to evaluate polynomials
     * k       evaluate first k polynomials
     * return  double[] first k polynomials evaluated at point x
 */ 
proc phi(x: real, k: int) {
    var p: [0..k-1] real = 0.0;
    var order = k-1;

    if (!phi_initialized) {
        phi_norm_space = {0..#100};
        for n in phi_norm_space do
            phi_norms[n] = sqrt(2.0*n+1);
    }
    
    p = pn(2.0*x-1.0, order);
    for n in 1..k-1 do
        p[n] = p[n]*phi_norms[n];
    return p;
}


// --- Begin Initialization --- //

hg_readCoeffs(hg_inputfile);

// --- End   Initialization --- //


proc main () {
    proc mxmt(a: [] real, b: [] real) where a.rank == 2 && b.rank == 2 {
        var c: [a.domain] real;

        for (i, j) in a.domain do
            for k in a.domain.dim(0) do
                c[i, j] += a[k, i] * b[k, j];

        return c;
    }

    proc err(a: [] real): real where a.rank == 2 {
        var sum = 0.0;

        for i in a.domain.dim(0) {
            for j in 0..i-1 do
                sum += a[i, j]*a[i, j] + a[j, i]*a[j, i];
            sum += (a[i, i]-1.0)*(a[i, i]-1.0);
        }

        return sqrt(sum);
    }

    for k in 1..hg_getMaxK() {            
        var hg = hg_getCoeffs(k);
        writeln("k= ", k, " err(mxmt(hg,hg)= ", err(mxmt(hg,hg)));
        //writeln("k= ", k, " pn(0.5,k)=", pn(0.5,k));
        //writeln("k= ", k, " phi(0.5,k)=", phi(0.5,k));
        //writeln("k= ", k, "\n", hg);
    }

    // test exception
    var hg = hg_getCoeffs(hg_getMaxK()+1);
}
