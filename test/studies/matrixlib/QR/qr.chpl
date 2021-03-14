/* This file contains a solver function named QR() along
 * with its supporting functions and iterators.  It also contains a
 * main function that tests the QR function.
 *
 * The test matrix is read in from prolate5.dat by default, but a
 * different input file can be specified on the command line using
 * --inputfile=<filename>.  
 *
 */
// Enables file operations
use IO;

// configuration variables:
//   inputfile: a string representing the file containing matrix data

config const inputfile = "prolate5.dat";

// QR: Computes QR.  The arguments are as follows:
//   A: the m x n input array to be factored.  
//
proc QR(A: [?D]) where (D.rank == 2) {

  const m = D.dim(0).size,
        n = D.dim(1).size,
        minDim = min(m,n),
        zero: A.eltType,
        one = 1.0: A.eltType,
        safmin = 2.00416836e-292,
        rsafmin = one/safmin;

  var tau: [1..minDim] A.eltType;
  var alpha, Xnorm, beta: A.eltType;
  var subnormscale = 1.0: A.eltType;

  for i in 1..minDim-1 {
    ref X = A(i+1..m,i..i);
    alpha = A(i,i);
    Xnorm = norm(X);
    if (Xnorm != zero) {
      beta = norm(alpha,Xnorm);
      if (beta < safmin) {
        do {
          X *= rsafmin;
          beta *= rsafmin;
          alpha *= rsafmin;
          subnormscale *= safmin;
        } while (beta < safmin);
        Xnorm = norm(X);
        beta = norm(alpha,Xnorm);
        if (alpha > zero) then beta = -beta;
        A(i,i) = beta*subnormscale;
        tau(i) = (beta-alpha)/beta;
        X /= (alpha-beta);
      }
      else {
        if (alpha > zero) then beta = -beta;
        A(i,i) = beta;
        tau(i) = (beta-alpha)/beta;
        X /= (alpha-beta);
      }   
    }
    if (tau(i) != zero) {
      alpha = A(i,i);
      A(i,i) = one;
      ref v = A[i..m, i..i];
      ref Ai = A[i..m, i+1..n];
      var Di = {i+1..n};
      var w: [Di] real;
      for j in Di {
        w(j) = + reduce (Ai[..,j..j]*v);
      }
      for j in Di {
        for k in i..m {
           Ai(k,j) -= tau(i)*v(k,i)*w(j);
        }
      }
      A(i,i) = alpha;
    }
  }
}

proc norm(x...?n) {
  var sum, scale, absXi:x(1).type;
  const one = 1.0:x(1).type;
  const zero = 0.0:x(1).type;

  sum = one;
  for i in 0..#n {
    if (x(i) != zero) {
      absXi = abs(x(i));
      if (scale < absXi) {
        sum = one + sum*(scale/absXi)**2;
        scale = absXi;
      }
      else
        sum += (absXi/scale)**2;
    }
  }
  return (scale*sqrt(sum));
}

proc norm(X: [?D]) {
  var sum, scale, absXi:X.eltType;
  const one = 1.0:X.eltType;
  const zero = 0.0:X.eltType;

  sum = one;
  for i in D {
    if (X(i) != zero) {
      absXi = abs(X(i));
      if (scale < absXi) {
        sum = one + sum*(scale/absXi)**2;
        scale = absXi;
      }
      else
        sum += (absXi/scale)**2;
    }
  }
  return (scale*sqrt(sum));
}

// The main function here provides a test harness for calling the
// QR routine

proc main() {

  // Open the input file, read in the array size, ensure it's square
  var infile = open(inputfile, iomode.r).reader();
  const m = infile.read(int),
        n = infile.read(int);

  // declare a range A1D, 2D domain AD, and array (matrix) A
  const AD = {1..m, 1..n};
  var A: [AD] real;

  // Initialize A by reading values in from infile.
  for ij in AD do
    infile.read(A(ij));

  // close the input file
  infile.close();

  // print out the initial matrix
  writeln("Unfactored Matrix:\n", A, "\n");

  // perform the blocking
  QR(A);

  // print out the results
  writeln("Factored Matrix:\n", A, "\n");
}
