/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*

The Jama Chapel module is a port of the JAMA matrix library

http://math.nist.gov/javanumerics/jama/

JAMA is a basic linear algebra package for Java. This is a port of Jama to Chapel.
Jama provides user-level classes for constructing and manipulating real, dense
matrices. Jama is meant to provide sufficient functionality for routine problems,
packaged in a way that is natural and understandable to non-experts. Jama is
intended to serve as the standard matrix class for Chapel. Jama is a straightforward
public-domain reference implementation has been developed by the MathWorks and
NIST as a strawman for such a class. MathWorks and NIST released this version in
order to obtain public comment. There was no guarantee that future versions of
JAMA will be compatible with this one.

A sibling matrix package, Jampack, has also been developed at NIST and the
University of Maryland. The two packages arose from the need to evaluate
alternate designs for the implementation of matrices in Java. JAMA is based
on a single matrix class within a strictly object-oriented framework. Jampack
uses a more open approach that lends itself to extension by the user. As it
turns out, for the casual user the packages differ principally in the syntax
of the matrix operations. We hope you will take the time to look at Jampack
along with JAMA. There is much to be learned from both packages.

Capabilities.

JAMA is comprised of six Java classes: Matrix, CholeskyDecomposition,
LUDecomposition, QRDecomposition, SingularValueDecomposition and
EigenvalueDecomposition.

Reference Implementation.

The implementation of JAMA downloadable from this site is meant to be a
reference implementation only. As such, it is pedagogical in nature. The
algorithms employed are similar to those of the classic Wilkinson and Reinsch
Handbook, i.e. the same algorithms used in EISPACK, LINPACK and MATLAB.
Matrices are stored internally as native arrays (i.e., double[][]). The
coding style is straightforward and readable. While the reference
implementation itself should provide reasonable execution speed for small
to moderate size applications, we fully expect software vendors to
provide versions which are optimized for particular environments. Not Covered.

JAMA is by no means a complete linear algebra environment. For example,
there are no provisions for matrices with particular structure (e.g.,
banded, sparse) or for more specialized decompositions (e.g. Schur,
generalized eigenvalue). Complex matrices are not included. It is not our
intention to ignore these important problems. We expect that some of these
(e.g. complex) will be addressed in future versions. It is our intent that
the design of JAMA not preclude extension to some of these additional areas.

Finally, JAMA is not a general-purpose array class. Instead, it focuses on
the principle mathematical functionality required to do numerical linear algebra.
As a result, there are no methods for array operations such as reshaping or
applying elementary functions (e.g. sine, exp, log) elementwise. Such operations,
while quite useful in many applications, are best collected into a separate array
class.

The Matrix class provides the fundamental operations of numerical linear algebra.
Various constructors create Matrices from two dimensional arrays of double precision
floating point numbers. Various gets and sets provide access to submatrices and
matrix elements. The basic arithmetic operations include matrix addition and
multiplication, matrix norms and selected element-by-element array operations.
A convenient matrix print method is also included.

Five fundamental matrix decompositions, which consist of pairs or triples of
matrices, permutation vectors, and the like, produce results in five
decomposition classes. These decompositions are accessed by the Matrix class
to compute solutions of simultaneous linear equations, determinants, inverses
and other matrix functions. The five decompositions are

    Cholesky Decomposition of symmetric, positive definite matrices
    LU Decomposition (Gaussian elimination) of rectangular matrices
    QR Decomposition of rectangular matrices
    Eigenvalue Decomposition of both symmetric and nonsymmetric square matrices
    Singular Value Decomposition of rectangular matrices

The current JAMA deals only with real matrices. We expect that future versions
will also address complex matrices. This has been deferred since crucial design
decisions cannot be made until certain issues regarding the implementation of
complex in the Java language are resolved.

The design of JAMA represents a compromise between the need for pure and elegant
object-oriented design and the need to enable high performance implementations.

Authors

JAMA's initial design, as well as this reference implementation, was developed by
Joe Hicklin
Cleve Moler
Peter Webb      ... from The MathWorks

Ronald F. Boisvert
Bruce Miller
Roldan Pozo
Karin Remington      ... from NIST

Copyright Notice

This software is a cooperative product of The MathWorks and the National Institute
of Standards and Technology (NIST) which has been released to the public domain.
Neither The MathWorks nor NIST assumes any responsibility whatsoever for its use
by other parties, and makes no guarantees, expressed or implied, about its quality,
reliability, or any other characteristic.

*/
module LinearAlgebraJama {

use Random;
use Math;

const seed = 314159265;

/** sqrt(a^2 + b^2) without under/overflow. **/

proc hypot(a, b:real) {
   var r : real;

   if (abs(a) > abs(b)) {
      r = b/a;
      r = abs(a)*((1.0+r*r)**0.5); //Math.sqrt(1+r*r);
   }
   else if (b != 0.0) {
      r = a/b;
      r = abs(b)*((1.0+r*r)**0.5); //sqrt(1+r*r);
   }
   else {
      r = 0.0;
   }

   return r;
}


/*
   Cholesky Decomposition.

   For a symmetric, positive definite matrix A, the Cholesky decomposition
   is an lower triangular matrix L so that A = L*L'.

   If the matrix is not symmetric or positive definite, the constructor
   returns a partial decomposition and sets an internal flag that may
   be queried by the isSPD() method.

*/

class CholeskyDecomposition {

   /* Row and column dimension (square matrix). */
   var n:int;

   pragma "no doc"
   var lDom = {0..1,0..1};

   /* Array for internal storage of decomposition. internal array storage. */
   var L : [lDom] real;

   /* Symmetric and positive definite flag.
   */
   var isspd:bool;

   /* Cholesky algorithm for symmetric and positive definite matrix.
       Structure to access L and isspd flag.
       Arg a Square, symmetric matrix.
   */
   proc init (Arg: unmanaged Matrix) {


     // Initialize.
      var A = Arg.getArray();
      n = Arg.getRowDimension();
      lDom = {1..n, 1..n};
      //L = new double[n,n];
      isspd = (Arg.getColumnDimension() == n);

      this.complete();

      // Main loop.
      //for (int j = 0; j < n; j++) {
      for j in 1..n {
         var Lrowj = L[j,..];
         var d = 0.0;
         //for (int k = 0; k < j; k++) {
         for k in 1..j {
            var Lrowk = L[k,..];
            var s = 0.0;
            //for (int i = 0; i < k; i++) {
            for i in 1..k {
               s += Lrowk[i]*Lrowj[i];
            }
            s = (A[j,k] - s)/L[k,k];
            Lrowj[k] = s;
            d = d + s*s;
            isspd = isspd & (A[k,j] == A[j,k]);
         }
         d = A[j,j] - d;
         isspd = isspd & (d > 0.0);
         L[j,j] = max(d,0.0) ** 0.5; // Math.sqrt(Math.max(d,0.0));
         //for (int k = j+1; k < n; k++) {
         for k in j+1..n {
            L[j,k] = 0.0;
         }
      }
   }

   /* Is the matrix symmetric and positive definite?
   return     true if A is symmetric and positive definite.
   */

   proc isSPD () {
      return isspd;
   }

   /* Return triangular factor.
   return     L
   */

   proc getL () {
      return new unmanaged Matrix(L,n,n);
   }

   /* Solve A*X = B
   param  B   A Matrix with as many rows as A and any number of columns.
   return     X so that L*L'*X = B
   */

   proc solve (B: unmanaged Matrix) {
      if (B.getRowDimension() != n) {
         assert(B.getRowDimension() != n, "Matrix row dimensions must agree.");
      }
      if (!isspd) {
         assert(!isspd, "Matrix is not symmetric positive definite.");
      }

      // Copy right hand side.
      var X = B.getArrayCopy();
      var nx = B.getColumnDimension();
      const nxrng = {1..nx};
      const krng = {1..n};

           // Solve L*Y = B;
           //for (int k = 0; k < n; k++) {
           for k in krng {
             //for (int j = 0; j < nx; j++) {
             for j in nxrng {
                //for (int i = 0; i < k ; i++) {
                for i in 1..k {
                    X[k,j] -= X[i,j]*L[k,i];
                }
                X[k,j] /= L[k,k];
             }
           }

           // Solve L'*X = Y;
           //for (int k = n-1; k >= 0; k--) {
           for k in 1..n-1 by -1 {
             //for (int j = 0; j < nx; j++) {
             for j in nxrng {
                for i in k+1..n {
                    X[k,j] -= X[i,j]*L[i,k];
                }
                X[k,j] /= L[k,k];
             }
           }

      return new unmanaged Matrix(X,n,nx);
   }

}

/* Eigenvalues and eigenvectors of a real matrix.

    If A is symmetric, then A = V*D*V' where the eigenvalue matrix D is
    diagonal and the eigenvector matrix V is orthogonal.
    I.e. A = V.times(D.times(V.transpose())) and
    V.times(V.transpose()) equals the identity matrix.

    If A is not symmetric, then the eigenvalue matrix D is block diagonal
    with the real eigenvalues in 1-by-1 blocks and any complex eigenvalues,
    lambda + i*mu, in 2-by-2 blocks, [lambda, mu; -mu, lambda].  The
    columns of V represent the eigenvectors in the sense that A*V = V*D,
    i.e. A.times(V) equals V.times(D).  The matrix V may be badly
    conditioned, or even singular, so the validity of the equation
    A = V*D*inverse(V) depends upon V.cond().
*/

class EigenvalueDecomposition {

   /* Row and column dimension (square matrix).
   */
   var n:int;

   /* Symmetry flag.
   internal symmetry flag.
   */
   var issymmetric:bool;

   /* Arrays for internal storage of eigenvalues.
   internal storage of eigenvalues.
   */
   var dDom, eDom = {1..1};
   var d: [dDom] real;
   var e: [eDom] real;

   /* Array for internal storage of eigenvectors.
   internal storage of eigenvectors.
   */
   var vDom = {1..1, 1..1};
   var V : [vDom] real;

   /* Array for internal storage of nonsymmetric Hessenberg form.
   internal storage of nonsymmetric Hessenberg form.
   */
   var hDom = {1..1,1..1};
   var H : [hDom] real;

   /* Working storage for nonsymmetric algorithm.
   working storage for nonsymmetric algorithm.
   */
   var ortDom = {1..1};
   var ort : [ortDom] real;

   /* Symmetric Householder reduction to tridiagonal form. */

   proc tred2 () {

   /*  This is derived from the Algol procedures tred2 by
     Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
     Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
     Fortran subroutine in EISPACK. */

      for j in d.domain {
         d[j] = V[n-1,j];
      }

      // Householder reduction to tridiagonal form.

      for i in 2..n by -1 {
         // Scale to avoid under/overflow.

         var scale = 0.0;
         var h = 0.0;
         const irng = {1..i};

         for k in irng {
            scale = scale + abs(d[k]);
         }

         if (scale == 0.0) {
            e[i] = d[i-1];

            for j in irng {
               d[j] = V[i-1,j];
               V[i,j] = 0.0;
               V[j,i] = 0.0;
            }
         }
         else {

            // Generate Householder vector.

            //for (int k = 0; k < i; k++) {
            for k in irng {
               d[k] /= scale;
               h += d[k] * d[k];
            }
            var f = d[i-1];
            var g = h ** 0.5; //Math.sqrt(h);
            if (f > 0) {
               g = -g;
            }
            e[i] = scale * g;
            h = h - f * g;
            d[i-1] = f - g;
            //for (int j = 0; j < i; j++) {
            for j in irng {
               e[j] = 0.0;
            }

            // Apply similarity transformation to remaining columns.

            for j in irng {
               f = d[j];
               V[j,i] = f;
               g = e[j] + V[j,j] * f;

               for k in j+1..i-1 {
                  g += V[k,j] * d[k];
                  e[k] += V[k,j] * f;
               }
               e[j] = g;
            }
            f = 0.0;

            for j in irng {
               e[j] /= h;
               f += e[j] * d[j];
            }

            var hh = f / (h + h);

            for j in irng {
               e[j] -= hh * d[j];
            }

            for j in irng {
               f = d[j];
               g = e[j];

               for k in j..i-1 {
                  V[k,j] -= (f * e[k] + g * d[k]);
               }
               d[j] = V[i-1,j];
               V[i,j] = 0.0;
            }
         }
         d[i] = h;
      }

      // Accumulate transformations.

      for i in 1..n-1 {
         const irng = {1..i};
         V[n-1,i] = V[i,i];
         V[i,i] = 1.0;
         var h = d[i+1];
         if (h != 0.0) {
            for k in irng {
               d[k] = V[k,i+1] / h;
            }

            for j in irng {
               var g = 0.0;
               for k in irng {
                  g += V[k,i+1] * V[k,j];
               }

               for k in irng {
                  V[k,j] -= g * d[k];
               }
            }
         }

         for k in irng {
            V[k,i+1] = 0.0;
         }
      }

      for j in 1..n {
         d[j] = V[n-1,j];
         V[n-1,j] = 0.0;
      }

      V[n-1,n-1] = 1.0;
      e[1] = 0.0;
   }

   /* Symmetric tridiagonal QL algorithm. */

   proc tql2 () {

   //  This is derived from the Algol procedures tql2, by
   //  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
   //  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
   //  Fortran subroutine in EISPACK.

      //for (int i = 1; i < n; i++) {
      for i in 2..n {
         e[i-1] = e[i];
      }
      e[n-1] = 0.0;

      var f = 0.0;
      var tst1 = 0.0;
      var eps = 2.0 ** -52.0;

      for l in 1..n {

         // Find small subdiagonal element

         tst1 = max(tst1,abs(d[l]) + abs(e[l]));
         var m = l;
         while (m < n) {
            if (abs(e[m]) <= eps*tst1) {
               break;
            }
            m+=1;
         }

         // If m == l, d[l] is an eigenvalue,
         // otherwise, iterate.

         if (m > l) {
            var itr = 0;
            do {
               itr = itr + 1;  // (Could check iteration count here.)

               // Compute implicit shift

               var g = d[l];
               var p = (d[l+1] - g) / (2.0 * e[l]);
               var r = hypot(p,1.0);
               if (p < 0) {
                  r = -r;
               }
               d[l] = e[l] / (p + r);
               d[l+1] = e[l] * (p + r);
               var dl1 = d[l+1];
               var h = g - d[l];

               for i in l+2..n {
                  d[i] -= h;
               }
               f = f + h;

               // Implicit QL transformation.

               p = d[m];
               var c = 1.0;
               var c2 = c;
               var c3 = c;
               var el1 = e[l+1];
               var s = 0.0;
               var s2 = 0.0;

               for i in l..m-1 by -1 {
                  c3 = c2;
                  c2 = c;
                  s2 = s;
                  g = c * e[i];
                  h = c * p;
                  r = hypot(p,e[i]);
                  e[i+1] = s * r;
                  s = e[i] / r;
                  c = p / r;
                  p = c * d[i] - s * g;
                  d[i+1] = h + s * (c * g + s * d[i]);

                  // Accumulate transformation.

                  for k in 1..n {
                     h = V[k,i+1];
                     V[k,i+1] = s * V[k,i] + c * h;
                     V[k,i] = c * V[k,i] - s * h;
                  }
               }
               p = -s * s2 * c3 * el1 * e[l] / dl1;
               e[l] = s * p;
               d[l] = c * p;

               // Check for convergence.

            } while (abs(e[l]) > eps*tst1);
         }
         d[l] = d[l] + f;
         e[l] = 0.0;
      }

      // Sort eigenvalues and corresponding vectors.

      //for (int i = 0; i < n-1; i++) {
      for i in d.domain {
         var k = i;
         var p = d[i];

         for j in i+1..n {
            if (d[j] < p) {
               k = j;
               p = d[j];
            }
         }
         if (k != i) {
            d[k] = d[i];
            d[i] = p;

            for j in 1..n {
               p = V[j,i];
               V[j,i] = V[j,k];
               V[j,k] = p;
            }
         }
      }
   }

   /* Nonsymmetric reduction to Hessenberg form. */

   proc orthes () {

      //  This is derived from the Algol procedures orthes and ortran,
      //  by Martin and Wilkinson, Handbook for Auto. Comp.,
      //  Vol.ii-Linear Algebra, and the corresponding
      //  Fortran subroutines in EISPACK.

      var low = 1;
      var high = n;

      for m in low+1..high-1 {

         // Scale column.

         var scale = 0.0;

         for i in m..high {
            scale = scale + abs(H[i,m-1]);
         }
         if (scale != 0.0) {

            // Compute Householder transformation.

            var h = 0.0;

            for i in m..high {
               ort[i] = H[i,m-1]/scale;
               h += ort[i] * ort[i];
            }

            var g = h**0.5; //sqrt(h);
            if (ort[m] > 0) {
               g = -g;
            }
            h = h - ort[m] * g;
            ort[m] = ort[m] - g;

            // Apply Householder similarity transformation
            // H = (I-u*u'/h)*H*(I-u*u')/h)

            for j in m..n {
               var f = 0.0;
               for i in m..high {
                  f += ort[i]*H[i,j];
               }
               f = f/h;
               for i in m..high {
                  H[i,j] -= f*ort[i];
               }
           }

           for i in 1..high {
               var f = 0.0;
               for j in m..high by -1 {
                  f += ort[j]*H[i,j];
               }
               f = f/h;
               for j in m..high {
                  H[i,j] -= f*ort[j];
               }
            }
            ort[m] = scale*ort[m];
            H[m,m-1] = scale*g;
         }
      }

      // Accumulate transformations (Algol's ortran).

      for (i,j) in V.domain {
            V[i,j] = if (i == j) then 1.0 else 0.0;
      }

      //for (int m = high-1; m >= low+1; m--) {
      for m in low+1..high-1 by -1 {
         if (H[m,m-1] != 0.0) {
            for i in m+1..high {
               ort[i] = H[i,m-1];
            }
            const mhighrng = {m..high};
            for j in mhighrng {
               var g = 0.0;
               for i in mhighrng {
                  g += ort[i] * V[i,j];
               }
               // Double division avoids possible underflow
               g = (g / ort[m]) / H[m,m-1];
               for i in mhighrng {
                  V[i,j] += g * ort[i];
               }
            }
         }
      }
   }

   var cdivr, cdivi : real;

   /* Complex scalar division. */
   proc cdiv(xr, xi, yr, yi:real) {
      var r,d : real;
      if (abs(yr) > abs(yi)) {
         r = yi/yr;
         d = yr + r*yi;
         cdivr = (xr + r*xi)/d;
         cdivi = (xi - r*xr)/d;
      } else {
         r = yr/yi;
         d = yi + r*yr;
         cdivr = (r*xr + xi)/d;
         cdivi = (r*xi - xr)/d;
      }
   }

   /* Nonsymmetric reduction from Hessenberg to real Schur form. */

   proc hqr2 () {

      //  This is derived from the Algol procedure hqr2,
      //  by Martin and Wilkinson, Handbook for Auto. Comp.,
      //  Vol.ii-Linear Algebra, and the corresponding
      //  Fortran subroutine in EISPACK.

      // Initialize

      var nn :int = this.n;
      var n :int = nn-1; // suspicious of this subtraction
      var low = 1;
      var high = nn;
      var eps = 2.0 ** -52.0;
      var exshift = 0.0;
      var p,q,r,s,z,t,w,x,y:real;

      // Store roots isolated by balanc and compute matrix norm

      var norm = 0.0;
      for i in 1..nn {
         if (i < low | i > high) {
            d[i] = H[i,i];
            e[i] = 0.0;
         }
         for j in max(i,1)..nn {
            norm = norm + abs(H[i,j]);
         }
      }

      // Outer loop over eigenvalue index

      var itr = 0;
      while (n >= low) {

         // Look for single small sub-diagonal element

         var l = n;
         while (l > low) {
            s = abs(H[l-1,l-1]) + abs(H[l,l]);
            if (s == 0.0) {
               s = norm;
            }
            if (abs(H[l,l-1]) < eps * s) {
               break;
            }
            l-=1;
         }

         // Check for convergence
         // One root found

         if (l == n) {
            H[n,n] = H[n,n] + exshift;
            d[n] = H[n,n];
         e[n] = 0.0;
         n-=1;
            itr = 0;

         // Two roots found

         } else if (l == n-1) {
            w = H[n,n-1] * H[n-1,n];
            p = (H[n-1,n-1] - H[n,n]) / 2.0;
            q = p * p + w;
            z = abs(q) ** 0.5;
            H[n,n] = H[n,n] + exshift;
            H[n-1,n-1] = H[n-1,n-1] + exshift;
            x = H[n,n];

            // Real pair

            if (q >= 0) {
               if (p >= 0) {
                  z = p + z;
               } else {
                  z = p - z;
               }
               d[n-1] = x + z;
               d[n] = d[n-1];
               if (z != 0.0) {
                  d[n] = x - w / z;
               }
               e[n-1] = 0.0;
               e[n] = 0.0;
               x = H[n,n-1];
               s = abs(x) + abs(z);
               p = x / s;
               q = z / s;
               r = (p * p+q * q) ** 0.5;
               p = p / r;
               q = q / r;

               // Row modification

               for j in n-1..nn {
                  z = H[n-1,j];
                  H[n-1,j] = q * z + p * H[n,j];
                  H[n,j] = q * H[n,j] - p * z;
               }

               // Column modification

               for i in 1..n {
                  z = H[i,n-1];
                  H[i,n-1] = q * z + p * H[i,n];
                  H[i,n] = q * H[i,n] - p * z;
               }

               // Accumulate transformations

               for i in low..high {
                  z = V[i,n-1];
                  V[i,n-1] = q * z + p * V[i,n];
                  V[i,n] = q * V[i,n] - p * z;
               }

            // Complex pair

            } else {
               d[n-1] = x + p;
               d[n] = x + p;
               e[n-1] = z;
               e[n] = -z;
            }
            n = n - 2;
            itr = 0;

         // No convergence yet

         } else {

            // Form shift

            x = H[n,n];
            y = 0.0;
            w = 0.0;
            if (l < n) {
               y = H[n-1,n-1];
               w = H[n,n-1] * H[n-1,n];
            }

            // Wilkinson's original ad hoc shift

            if (itr == 10) {
               exshift += x;
               for i in low..n {
                  H[i,i] -= x;
               }
               s = abs(H[n,n-1]) + abs(H[n-1,n-2]);
               y = 0.75 * s;
               x = y;
               w = -0.4375 * s * s;
            }

            // MATLAB's new ad hoc shift

            if (itr == 30) {
                s = (y - x) / 2.0;
                s = s * s + w;
                if (s > 0) {
                    s = s ** 0.5;
                    if (y < x) {
                       s = -s;
                    }
                    s = x - w / ((y - x) / 2.0 + s);
                    for i in low..n {
                       H[i,i] -= s;
                    }
                    exshift += s;
                    x = 0.964; y = 0.964; w = 0.964;
                }
            }

            itr = itr + 1;   // (Could check iteration count here.)

            // Look for two consecutive small sub-diagonal elements

            var m = n-2;
            while (m >= l) {
               z = H[m,m];
               r = x - z;
               s = y - z;
               p = (r * s - w) / H[m+1,m] + H[m,m+1];
               q = H[m+1,m+1] - z - r - s;
               r = H[m+2,m+1];
               s = abs(p) + abs(q) + abs(r);
               p = p / s;
               q = q / s;
               r = r / s;
               if (m == l) {
                  break;
               }
               if (abs(H[m,m-1]) * (abs(q) + abs(r)) <
                  eps * (abs(p) * (abs(H[m-1,m-1]) + abs(z) +
                  abs(H[m+1,m+1])))) {
                     break;
               }
               m-=1;
            }

            for i in m+2..n {
               H[i,i-2] = 0.0;
               if (i > m+2) {
                  H[i,i-3] = 0.0;
               }
            }

            // Double QR step involving rows l:n and columns m:n

            for k in m..n-1 {
               var notlast = (k != n-1);
               if (k != m) {
                  p = H[k,k-1];
                  q = H[k+1,k-1];
                  r = if (notlast) then H[k+2,k-1] else 0.0;
                  x = abs(p) + abs(q) + abs(r);
                  if (x == 0.0) {
                      continue;
                  }
                  p = p / x;
                  q = q / x;
                  r = r / x;
               }

               s = (p * p + q * q + r * r) ** 0.5;
               if (p < 0) {
                  s = -s;
               }
               if (s != 0) {
                  if (k != m) {
                     H[k,k-1] = -s * x;
                  } else if (l != m) {
                     H[k,k-1] = -H[k,k-1];
                  }
                  p = p + s;
                  x = p / s;
                  y = q / s;
                  z = r / s;
                  q = q / p;
                  r = r / p;

                  // Row modification

                  for j in k..nn {
                     p = H[k,j] + q * H[k+1,j];
                     if (notlast) {
                        p = p + r * H[k+2,j];
                        H[k+2,j] = H[k+2,j] - p * z;
                     }
                     H[k,j] = H[k,j] - p * x;
                     H[k+1,j] = H[k+1,j] - p * y;
                  }

                  // Column modification

                  for i in 1..min(n,k+3) {
                     p = x * H[i,k] + y * H[i,k+1];
                     if (notlast) {
                        p = p + z * H[i,k+2];
                        H[i,k+2] = H[i,k+2] - p * r;
                     }
                     H[i,k] = H[i,k] - p;
                     H[i,k+1] = H[i,k+1] - p * q;
                  }

                  // Accumulate transformations

                  for i in low..high {
                     p = x * V[i,k] + y * V[i,k+1];
                     if (notlast) {
                        p = p + z * V[i,k+2];
                        V[i,k+2] = V[i,k+2] - p * r;
                     }
                     V[i,k] = V[i,k] - p;
                     V[i,k+1] = V[i,k+1] - p * q;
                  }
               }  // (s != 0)
            }  // k loop
         }  // check convergence
      }  // while (n >= low)

      // Backsubstitute to find vectors of upper triangular form

      if (norm == 0.0) {
         return;
      }

      for n in 1..nn-1 by -1 {
         p = d[n];
         q = e[n];

         // Real vector

         if (q == 0) {
            var l = n;
            H[n,n] = 1.0;
            for i in 1..n-1 by -1 {
               w = H[i,i] - p;
               r = 0.0;
               for j in l..n {
                  r = r + H[i,j] * H[j,n];
               }
               if (e[i] < 0.0) {
                  z = w;
                  s = r;
               } else {
                  l = i;
                  if (e[i] == 0.0) {
                     if (w != 0.0) {
                        H[i,n] = -r / w;
                     } else {
                        H[i,n] = -r / (eps * norm);
                     }

                  // Solve real equations

                  } else {
                     x = H[i,i+1];
                     y = H[i+1,i];
                     q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
                     t = (x * s - z * r) / q;
                     H[i,n] = t;
                     if (abs(x) > abs(z)) {
                        H[i+1,n] = (-r - w * t) / x;
                     } else {
                        H[i+1,n] = (-s - y * t) / z;
                     }
                  }

                  // Overflow control

                  t = abs(H[i,n]);
                  if ((eps * t) * t > 1) {
                     for j in i..n {
                        H[j,n] = H[j,n] / t;
                     }
                  }
               }
            }

         // Complex vector

         } else if (q < 0) {
            var l = n-1;

            // Last vector component imaginary so matrix is triangular

            if (abs(H[n,n-1]) > abs(H[n-1,n])) {
               H[n-1,n-1] = q / H[n,n-1];
               H[n-1,n] = -(H[n,n] - p) / H[n,n-1];
            } else {
               cdiv(0.0,-H[n-1,n],H[n-1,n-1]-p,q);
               H[n-1,n-1] = cdivr;
               H[n-1,n] = cdivi;
            }

            H[n,n-1] = 0.0;
            H[n,n] = 1.0;

            for i in 1..n-2 by -1 {
               var ra,sa,vr,vi:real;
               ra = 0.0;
               sa = 0.0;

               for j in l..n {
                  ra = ra + H[i,j] * H[j,n-1];
                  sa = sa + H[i,j] * H[j,n];
               }
               w = H[i,i] - p;

               if (e[i] < 0.0) {
                  z = w;
                  r = ra;
                  s = sa;
               } else {
                  l = i;
                  if (e[i] == 0) {
                     cdiv(-ra,-sa,w,q);
                     H[i,n-1] = cdivr;
                     H[i,n] = cdivi;
                  } else {

                     // Solve complex equations

                     x = H[i,i+1];
                     y = H[i+1,i];
                     vr = (d[i] - p) * (d[i] - p) + e[i] * e[i] - q * q;
                     vi = (d[i] - p) * 2.0 * q;
                     if ( (vr == 0.0) & (vi == 0.0) ) {
                        vr = eps * norm * (abs(w) + abs(q) +
                        abs(x) + abs(y) + abs(z));
                     }
                     cdiv(x*r-z*ra+q*sa,x*s-z*sa-q*ra,vr,vi);
                     H[i,n-1] = cdivr;
                     H[i,n] = cdivi;
                     if (abs(x) > (abs(z) + abs(q))) {
                        H[i+1,n-1] = (-ra - w * H[i,n-1] + q * H[i,n]) / x;
                        H[i+1,n] = (-sa - w * H[i,n] - q * H[i,n-1]) / x;
                     } else {
                        cdiv(-r-y*H[i,n-1],-s-y*H[i,n],z,q);
                        H[i+1,n-1] = cdivr;
                        H[i+1,n] = cdivi;
                     }
                  }

                  // Overflow control

                  t = max(abs(H[i,n-1]),abs(H[i,n]));
                  if ((eps * t) * t > 1) {
                     for j in i..n {
                        H[j,n-1] = H[j,n-1] / t;
                        H[j,n] = H[j,n] / t;
                     }
                  }
               }
            }
         }
      }

      // Vectors of isolated roots

      for i in 1..nn {
         if (i < low | i > high) {
            for j in i..nn {
               V[i,j] = H[i,j];
            }
         }
      }

      // Back transformation to get eigenvectors of original matrix

      for j in low..nn-1 by -1 {
         for i in low..high {
            z = 0.0;
            for k in low..min(j,high) {
               z = z + V[i,k] * H[k,j];
            }

            V[i,j] = z;
         }
      }
   }


   /* Check for symmetry, then construct the eigenvalue decomposition
       Structure to access D and V.
   */

   proc init (Arg: unmanaged Matrix) {
      var A = Arg.getArray();
      n = Arg.getColumnDimension();
      issymmetric = true;
      dDom = {1..n};
      eDom = {1..n};
      vDom = {1..n,1..n};
      this.complete();

      var j = 1;
      while( (j < n) & issymmetric ) {
         var i = 1;
         while ( (i < n) & issymmetric ) {
            issymmetric = (A[i,j] == A[j,i]);
            i+=1;
         }
         j+=1;
      }

      if (issymmetric) {
         for (i,j) in {1..n, 1..n} {
               V[i,j] = A[i,j];
         }

         // Tridiagonalize.
         tred2();

         // Diagonalize.
         tql2();

      } else {
         hDom = {1..n, 1..n};
         ortDom = {1..n};

         for (j,i) in {1..n,1..n} {
               H[i,j] = A[i,j];
         }

         // Reduce to Hessenberg form.
         orthes();

         // Reduce Hessenberg to real Schur form.
         hqr2();
      }
   }

   /* Return the eigenvector matrix
   */

   proc getV () : unmanaged Matrix {
      return new unmanaged Matrix(V,n,n);
   }

   /* Return the real parts of the eigenvalues
   real(diag(D))
   */

   proc getRealEigenvalues () {
      return d;
   }

   /* Return the imaginary parts of the eigenvalues
   imag(diag(D))
   */

   proc getImagEigenvalues () {
      return e;
   }

   /* Return the block diagonal eigenvalue matrix
   */

   proc getD () : unmanaged Matrix {
      var X = new unmanaged Matrix(n,n);
      var D = X.getArray();
      const nrng = {1..n};
      for i in nrng {
         for j in nrng {
            D[i,j] = 0.0;
         }
         D[i,i] = d[i];
         if (e[i] > 0) {
            D[i,i+1] = e[i];
         } else if (e[i] < 0) {
            D[i,i-1] = e[i];
         }
      }
      return X;
   }

}

/*
   LU Decomposition.

   For an m-by-n matrix A with m >= n, the LU decomposition is an m-by-n
   unit lower triangular matrix L, an n-by-n upper triangular matrix U,
   and a permutation vector piv of length m so that A(piv,:) = L*U.
   If m < n, then L is m-by-m and U is m-by-n.

   The LU decomposition with pivoting always exists, even if the matrix is
   singular, so the constructor will never fail.  The primary use of the
   LU decomposition is in the solution of square systems of simultaneous
   linear equations.  This will fail if isNonsingular() returns false.
*/

class LUDecomposition {

   /* Row and column dimensions, and pivot sign.
   column dimension.
   row dimension.
   pivot sign.
   */
   var m, n, pivsign : int;

   /* Array for internal storage of decomposition.
   */
   var luDom = {1..1, 1..1};
   var LU : [luDom] real;

   /* Internal storage of pivot vector.
   pivot vector.
   */
   var pivDom = {1..1};
   var piv : [pivDom] int;

   /* LU Decomposition
       Structure to access L, U and piv.
   A Rectangular matrix
   */

   proc init (A: unmanaged Matrix) {
   // Use a "left-looking", dot-product, Crout/Doolittle algorithm.

      m = A.getRowDimension();
      n = A.getColumnDimension();
      pivsign = 1;

      luDom = {1..m,1..n};
      LU = A.getArrayCopy();

      pivDom = {1..m};
      piv = pivDom;
      this.complete();

      var lurowiDom = {1..n};
      var LUrowi : [lurowiDom] real;

      var lucoljDom = {1..m};
      var LUcolj : [lucoljDom] real;

      // Outer loop.

      const mrng = {1..m};

      for j in 1..n {

         // Make a copy of the j-th column to localize references.
         for i in mrng {
            LUcolj[i] = LU[i,j];
         }

         // Apply previous transformations.

         for i in mrng {
            LUrowi = LU[i,..];

            // Most of the time is spent in the following dot product.

            var kmax = min(i,j);
            var s = 0.0;
            for k in 1..kmax {
               s += LUrowi[k]*LUcolj[k];
            }

            LUcolj[i] -= s;
            LUrowi[j] = LUcolj[i];
         }

         // Find pivot and exchange if necessary.

         var p = j;
         for i in j+1..m {
            if (abs(LUcolj[i]) > abs(LUcolj[p])) {
               p = i;
            }
         }
         if (p != j) {
            for k in 1..n {
               var t = LU[p,k]; LU[p,k] = LU[j,k]; LU[j,k] = t;
            }
            var k = piv[p]; piv[p] = piv[j]; piv[j] = k;
            pivsign = -pivsign;
         }

         // Compute multipliers.

         if ( (j < m) & (LU[j,j] != 0.0)) {
            for i in j+1..m {
               LU[i,j] /= LU[j,j];
            }
         }
      }
   }

   /* Is the matrix nonsingular?
       true if U, and hence A, is nonsingular.
   */
   proc isNonsingular () {
      for j in 1..n {
         if (LU[j,j] == 0) {
            return false;
         }
      }
      return true;
   }

   /* Return lower triangular factor
   */

   proc getL () {
      var X = new unmanaged Matrix(m,n);
      var L = X.getArray();
      for (i,j) in {1..m,1..n} {
            if (i > j) {
               L[i,j] = LU[i,j];
            } else if (i == j) {
               L[i,j] = 1.0;
            } else {
               L[i,j] = 0.0;
            }
      }
      return X;
   }

   /* Return upper triangular factor
   */

   proc getU () {
      var X = new unmanaged Matrix(n,n);
      var U = X.getArray();
      for (i,j) in {1..n, 1..n} {
            if (i <= j) {
               U[i,j] = LU[i,j];
            } else {
               U[i,j] = 0.0;
            }
      }
      return X;
   }

   /* Return pivot permutation vector
   */
   proc getPivot () {
      var p : [1..m] int;
      p = piv;
      return p;
   }

   /* Return pivot permutation vector as a one-dimensional double array
   */
   proc getDoublePivot () {
      var vals : [1..m] real;
      for i in 1..m {
         vals[i] = piv[i]:real;
      }
      return vals;
   }

   /* Determinant
   */

   proc det () {
      if (m != n) {
         assert(m != n, "Matrix must be square.");
      }
      var d : real = pivsign:real;
      for j in 1..n {
         d *= LU[j,j];
      }
      return d;
   }

   /* Solve A*X = B
        B,   A Matrix with as many rows as A and any number of columns.
        X so that L*U*X = B(piv,:)
   */

   proc solve (B: unmanaged Matrix) {
      if (B.getRowDimension() != m) {
         assert(B.getRowDimension() != m, "Matrix row dimensions must agree.");
      }
      if (!this.isNonsingular()) {
         assert(!this.isNonsingular(), "Matrix is singular.");
      }

      // Copy right hand side with pivoting
      var nx = B.getColumnDimension();
      var Xmat = B.getMatrix(piv,1,nx);
      var X = Xmat.A;

      // Solve L*Y = B(piv,:)
      for k in 1..n {
         for (i,j) in {k+1..n, 1..nx} {
            X[i,j] -= X[k,j]*LU[i,k];
         }
      }
      // Solve U*X = Y;
      for k in 1..n-1 by -1 {
         for j in 1..nx {
            X[k,j] /= LU[k,k];
         }
         for (i,j) in {1..k, 1..nx} {
            X[i,j] -= X[k,j]*LU[i,k];
         }
      }
      Xmat.A = X;
      return Xmat;
   }

}

/* Generate identity matrix
     m    Number of rows.
     n    Number of columns.
     returns An m-by-n matrix with ones on the diagonal and zeros elsewhere.
*/

proc identity (m:int, n:int) {
   var A = new unmanaged Matrix(m,n);
   var X = A.getArray();
   for (i,j) in {1..m, 1..n} {
      X[i,j] = if(i == j) then 1.0 else 0.0;
   }
   return A;
}

/*
   Jama = Java Matrix class.

   The Java Matrix Class provides the fundamental operations of numerical
   linear algebra.  Various constructors create Matrices from two dimensional
   arrays of double precision floating point numbers.  Various "gets" and
   "sets" provide access to submatrices and matrix elements.  Several methods
   implement basic matrix arithmetic, including matrix addition and
   multiplication, matrix norms, and element-by-element array operations.
   Methods for reading and printing matrices are also included.  All the
   operations in this version of the Matrix Class involve real matrices.
   Complex matrices may be handled in a future version.

   Five fundamental matrix decompositions, which consist of pairs or triples
   of matrices, permutation vectors, and the like, produce results in five
   decomposition classes.  These decompositions are accessed by the Matrix
   class to compute solutions of simultaneous linear equations, determinants,
   inverses and other matrix functions.  The five decompositions are:

   Cholesky Decomposition of symmetric, positive definite matrices.
   LU Decomposition of rectangular matrices.
   QR Decomposition of rectangular matrices.
   Singular Value Decomposition of rectangular matrices.
   Eigenvalue Decomposition of both symmetric and nonsymmetric square matrices.

Example of use:

Solve a linear system A x = b and compute the residual norm, ||b - A x||.


author The MathWorks, Inc. and the National Institute of Standards and Technology.
version 5 August 1998
*/

class Matrix {

   /* Row and column dimensions.
     row dimension.
        column dimension.
   */
   var m, n : int;

   /* Array for internal storage of elements.
   */
   var aDom = {1..1, 1..1};
   var A : [aDom] real;

   /* Construct an m-by-n matrix of zeros.
        m    Number of rows.
        n    Number of columns.
   */

   proc init (m:int, n: int) {
      this.m = m;
      this.n = n;
      aDom = {1..m, 1..n};
   }

   /* Construct an m-by-n constant matrix.
        m    Number of rows.
        n    Number of columns.
        s    Fill the matrix with this scalar value.
   */

   proc init (m:int, n:int, s:real) {
      this.m = m;
      this.n = n;
      aDom = {1..m, 1..n};
      A = s;
   }

   /* Construct a matrix from a 2-D array.
        A    Two-dimensional array of doubles.
   */

   proc init (A:[?aDom] real) {
      if aDom.rank == 1 {
         m = aDom.high;
         n = 1;
      }
      else {
        m = aDom.high(1);
        n = aDom.high(2);
      }

      this.aDom = {1..m, 1..n};
      this.complete();

      for i in 1..m {
         if (this.aDom.high(2) != n) {
            assert((this.aDom.high(2) != n), "All rows must have the same length.");
         }
      }

      if aDom.rank == 1 {
        this.A(1..m, 1) = A;
      }
      else {
        this.A = A;
      }
   }

   /* Construct a matrix quickly without checking arguments.
        A    Two-dimensional array of doubles.
        m    Number of rows.
        n    Number of columns.
   */

   proc init (A:[?aDom] real, m:int, n:int) where aDom.rank == 2 {
      this.m = m;
      this.n = n;
      this.aDom = {1..m, 1..n};
      this.A = A(this.aDom);
   }

   /* Construct a matrix from a one-dimensional packed array
        vals One-dimensional array of doubles, packed by columns (ala Fortran).
        m    Number of rows.
   */

   proc init (vals:[?valsDom] real, m:int) where valsDom.rank == 1 {
      this.m = m;
      n = if(m != 0) then vals.domain.high/m else 0;
      aDom = {1..m, 1..n};
      this.complete();
      if (m*n != vals.domain.high) {
         assert(m*n != vals.domain.high, "Array length must be a multiple of m.");
      }
      var revDom = {1..n, 1..m};
      for (ij, val) in zip(revDom, vals) { A(ij(2), ij(1)) = val; }
   }

   /* Construct a matrix from a copy of a 2-D array.
        A    Two-dimensional array of doubles.
   */

   proc constructWithCopy(A : [?aDom] real) where aDom.rank == 2 {
      var m = aDom.high(1);
      var n = aDom.high(2);
      var X = new unmanaged Matrix(m,n);
      var C = X.getArray();

      for i in 1..m {
         for j in 1..n {
            C[i,j] = A[i,j];
         }
      }
      return X;
   }

   /* Make a deep copy of a matrix
   */

   proc copy () {
      var X = new unmanaged Matrix(m,n);
      var C = X.getArray();
      for (i,j) in {1..m, 1..n} {
            C[i,j] = A[i,j];
      }
      return X;
   }

   /* Clone the Matrix object.
   */

   proc clone () {
      return this.copy();
   }

   /* Access the internal two-dimensional array.
        Pointer to the two-dimensional array of matrix elements.
   */

   proc getArray () {
      return A;
   }

   /* Copy the internal two-dimensional array.
        Two-dimensional array copy of matrix elements.
   */

   proc getArrayCopy () {
      var C : [1..m, 1..n] real;
      C = A;
      return C;
   }

   /* Make a one-dimensional column packed copy of the internal array.
        Matrix elements packed in a one-dimensional array by columns.
   */

   proc getColumnPackedCopy () {
      var vals : [1..m*n] real;
      for (i,j) in {1..m, 1..n} {
         vals[i+j*m] = A[i,j];
      }
      return vals;
   }

   /* Make a one-dimensional row packed copy of the internal array.
        Matrix elements packed in a one-dimensional array by rows.
   */

   proc getRowPackedCopy () {
      var vals : [1..m*n] real;
      for (i,j) in {1..m, 1..n} {
         vals[i*n+j] = A[i,j];
      }
      return vals;
   }

   /* Get row dimension.
        m, the number of rows.
   */

   proc getRowDimension () {
      return m;
   }

   /* Get column dimension.
        n, the number of columns.
   */

   proc getColumnDimension () {
      return n;
   }

   /* Get a single element.
        i    Row index.
        j    Column index.
        return A(i,j)
   */

   proc get (i, j:int) {
      return A[i,j];
   }

   /* Get a submatrix.
        i0   Initial row index
        i1   Final row index
        j0   Initial column index
        j1   Final column index
        return     A(i0:i1,j0:j1)
   */

   proc getMatrix (i0:int, i1:int, j0:int, j1:int) {
      var X = new unmanaged Matrix(i1-i0+1,j1-j0+1);
      for (i,j) in {i0..i1, j0..j1} {
         X.A[(i-i0)+1,(j-j0)+1] = A[i,j];
      }
      return X;
   }

   /* Get a submatrix.
        r    Array of row indices.
     c    Array of column indices.
     return     A(r(:),c(:))
   */

   proc getMatrix (r:[?rDom] int, c:[?cDom] int) {
      var X = new unmanaged Matrix(r.length,c.length);
      var B = X.getArray();
         for (i,j) in {1..rDom.high, 1..cDom.high} {
               B[i,j] = A[r[i],c[j]];
         }
      return X;
   }

   /* Get a submatrix.
        i0   Initial row index
        i1   Final row index
        c    Array of column indices.
        return     A(i0:i1,c(:))
   */

   proc getMatrix (i0:int, i1:int, c:[?cDom] int) {
      var X = new unmanaged Matrix(i1-i0+1,c.length);
      var B = X.getArray();
        for (i,j) in {i0..i1, 1..cDom.high} {
               B[i-i0,j] = A[i,c[j]];
         }
      return X;
   }

   /* Get a submatrix.
        r    Array of row indices.
     j0   Initial column index
     j1   Final column index
        return     A(r(:),j0:j1)
   */

   proc getMatrix (r:[?rDom] int, j0:int, j1:int) {
      var X = new unmanaged Matrix(rDom.high,j1-j0+1);
      var B = X.getArray();
         for (i,j) in {1..rDom.high, j0..j1} {
               B[i,(j-j0)+1] = A[r[i],j];
         }
      return X;
   }

   /* Set a single element.
     i    Row index.
     j    Column index.
     s    A(i,j).
   */

   proc set (i, j:int, s:real) {
      A[i,j] = s;
   }

   /* Set a submatrix.
   i0   Initial row index
   i1   Final row index
   j0   Initial column index
   j1   Final column index
   X    A(i0:i1,j0:j1)
   */

   proc setMatrix (i0, i1, j0, j1:int, X: unmanaged Matrix) {
         for (i,j) in {i0..i1, j0..j1} {
               A[i,j] = X.get(i-i0,j-j0);
         }
   }

   /* Set a submatrix.
   r    Array of row indices.
   c    Array of column indices.
   X    A(r(:),c(:))
   */

   proc setMatrix (r:[?rDom] int, c:[?cDom] int, X: unmanaged Matrix) {
      for (i,j) in {1..rDom.high, 1..cDom.high} {
               A[r[i],c[j]] = X.get(i,j);
      }
   }

   /* Set a submatrix.
   r    Array of row indices.
   j0   Initial column index
   j1   Final column index
   X    A(r(:),j0:j1)
   */

   proc setMatrix (r:[?rDom] int, j0:int, j1:int, X: unmanaged Matrix) {
         for (i,j) in {1..rDom.high, j0..j1} {
               A[r[i],j] = X.get(i,j-j0);
         }
   }

   /* Set a submatrix.
   i0   Initial row index
   i1   Final row index
   c    Array of column indices.
   X    A(i0:i1,c(:))
   */

   proc setMatrix (i0, i1:int, c:[?cDom] int, X: unmanaged Matrix) {
         for (i,j) in {i0..i1, 1..cDom.high} {
               A[i,c[j]] = X.get(i-i0,j);
         }
   }

   /* Matrix transpose.
   return    A'
   */

   proc transpose () {
      var X = new unmanaged Matrix(n,m);
      var C = X.A;
      for (i,j) in {1..m, 1..n} {
            C[j,i] = A[i,j];
      }
      X.A = C;
      return X;
   }

   /* One norm
   return    maximum column sum.
   */

   proc norm1 () :real {
      var f = 0.0;
      for j in 1..n {
         var s = (+ reduce abs(A(1..m, j)));
         f = max(f,s);
      }
      return f;
   }

   /* Two norm
   return    maximum singular value.
   */

   proc norm2 () : real{
      var toret : unmanaged SingularValueDecomposition = new unmanaged SingularValueDecomposition(this);
      return toret.norm2();
   }

   /* Infinity norm
   return    maximum row sum.
   */

   proc normInf () : real{
      var f = 0.0;
      for i in 1..m {
         var s = (+ reduce abs(A(i, 1..n)));
         f = max(f,s);
      }
      return f;
   }

   /* Frobenius norm
   return    sqrt of sum of squares of all elements.
   */

   proc normF () : real{
      var f = 0.0;
      for (i,j) in {1..m, 1..n } {
            f = hypot(f,A[i,j]);
      }
      return f;
   }

   /*  Unary minus
   return    -A
   */

   proc uminus () {
      var X = new unmanaged Matrix(m,n);
      var C = X.getArray();
      for (i,j) in {1..m, 1..n} {
            C[i,j] = -A[i,j];
      }
      return X;
   }

   /* C = A + B
   B    another matrix
   return     A + B
   */

   proc plus (B: unmanaged Matrix) {
      checkMatrixDimensions(B);
      var X = new unmanaged Matrix(m,n);
      var C = X.getArray();
      for (i,j) in {1..m, 1..n} {
            C[i,j] = A[i,j] + B.A[i,j];
      }
      return X;
   }

   /* A = A + B
   B    another matrix
   return     A + B
   */

   proc plusEquals (B: unmanaged Matrix) {
      checkMatrixDimensions(B);
      for (i,j) in {1..m, 1..n} {
            A[i,j] = A[i,j] + B.A[i,j];
      }
      return this;
   }

   /* C = A - B
   B    another matrix
   return     A - B
   */

   proc minus (B: unmanaged Matrix) {
      checkMatrixDimensions(B);
      var X = new unmanaged Matrix(m,n);
      var C = X.getArray();
      for (i,j) in {1..m, 1..n} {
            C[i,j] = A[i,j] - B.A[i,j];
      }
      return X;
   }

   /* A = A - B
   B    another matrix
   return     A - B
   */

   proc minusEquals (B: unmanaged Matrix) {
      checkMatrixDimensions(B);
      for (i,j) in {1..m, 1..n} {
            A[i,j] = A[i,j] - B.A[i,j];
      }
      return this;
   }

   /* Element-by-element multiplication, C = A.*B
   B    another matrix
   return     A.*B
   */

   proc arrayTimes (B: unmanaged Matrix) {
      checkMatrixDimensions(B);
      var X = new unmanaged Matrix(m,n);
      var C = X.getArray();
      for (i,j) in {1..m, 1..n} {
            C[i,j] = A[i,j] * B.A[i,j];
      }
      return X;
   }

   /* Element-by-element multiplication in place, A = A.*B
   B    another matrix
   return     A.*B
   */

   proc arrayTimesEquals (B: unmanaged Matrix) {
      checkMatrixDimensions(B);
      for (i,j) in {1..m, 1..n} {
            A[i,j] = A[i,j] * B.A[i,j];
      }
      return this;
   }

   /* Element-by-element right division, C = A./B
   B    another matrix
   return     A./B
   */

   proc arrayRightDivide (B: unmanaged Matrix) {
      checkMatrixDimensions(B);
      var X = new unmanaged Matrix(m,n);
      var C = X.getArray();
      for (i,j) in {1..m, 1..n} {
            C[i,j] = A[i,j] / B.A[i,j];
      }
      return X;
   }

   /* Element-by-element right division in place, A = A./B
   B    another matrix
   return     A./B
   */

   proc arrayRightDivideEquals (B: unmanaged Matrix) {
      checkMatrixDimensions(B);
      for (i,j) in {1..m, 1..n} {
            A[i,j] = A[i,j] / B.A[i,j];
      }
      return this;
   }

   /* Element-by-element left division, C = A.\B
   B    another matrix
   return     A.\B
   */

   proc arrayLeftDivide (B: unmanaged Matrix) {
      checkMatrixDimensions(B);
      var X = new unmanaged Matrix(m,n);
      var C = X.getArray();
      for (i,j) in {1..m, 1..n} {
            C[i,j] = B.A[i,j] / A[i,j];
      }
      return X;
   }

   /* Element-by-element left division in place, A = A.\B
   B    another matrix
   return     A.\B
   */

   proc arrayLeftDivideEquals (B: unmanaged Matrix) {
      checkMatrixDimensions(B);
      for (i,j) in {1..m, 1..n} {
            A[i,j] = B.A[i,j] / A[i,j];
      }
      return this;
   }

   /* Multiply a matrix by a scalar, C = s*A
   s    scalar
   return     s*A
   */

   proc times (s:real) {
      var X = new unmanaged Matrix(m,n);
      var C = X.getArray();
      for (i,j) in {1..m, 1..n} {
            C[i,j] = s*A[i,j];
      }
      return X;
   }

   /* Multiply a matrix by a scalar in place, A = s*A
   s    scalar
   return     replace A by s*A
   */

   proc timesEquals (s:real) {
      for (i,j) in {1..m, 1..n} {
            A[i,j] = s*A[i,j];
      }
      return this;
   }

   /* Linear algebraic matrix multiplication, A * B
   B    another matrix
   return     Matrix product, A * B
   */

   proc times (B: unmanaged Matrix) {
      if (B.m != n) {
         assert(B.m != n, "Matrix inner dimensions must agree.");
      }
      var X = new unmanaged Matrix(m,B.n);
      var C = X.getArray();
      var Bcolj : [1..n] real;
      for j in 1..B.n {
         for k in 1..n {
            Bcolj[k] = B.A[k,j];
         }
         for i in 1..m {
            var Arowi = A[i,..];
            var s = (+ reduce (Arowi(1..n) * Bcolj(1..n)));
            C[i,j] = s;
         }
      }
      return X;
   }

   /* LU Decomposition
   return     LUDecomposition
   */

   proc lu () {
      return new unmanaged LUDecomposition(_to_unmanaged(this));
   }

   /* QR Decomposition
   return     QRDecomposition
   */

   proc qr () {
      return new unmanaged QRDecomposition(_to_unmanaged(this));
   }

   /* Cholesky Decomposition
   return     CholeskyDecomposition
   */

   proc chol () {
      return new unmanaged CholeskyDecomposition(_to_unmanaged(this));
   }

   /* Singular Value Decomposition
   return     SingularValueDecomposition
   */

   proc svd () {
      return new unmanaged SingularValueDecomposition(_to_unmanaged(this));
   }

   /* Eigenvalue Decomposition
   return     EigenvalueDecomposition
   */

   proc eig () {
      return new unmanaged EigenvalueDecomposition(_to_unmanaged(this));
   }

   /* Solve A*X = B
   B    right hand side
   return     solution if A is square, least squares solution otherwise
   */

   proc solve (B: unmanaged Matrix) {
      var toret = if(m == n) then (new unmanaged LUDecomposition(_to_unmanaged(this))).solve(B)
                             else (new unmanaged QRDecomposition(_to_unmanaged(this))).solve(B);
      return toret;
   }

   /* Solve X*A = B, which is also A'*X' = B'
   B    right hand side
   return     solution if A is square, least squares solution otherwise.
   */

   proc solveTranspose (B: unmanaged Matrix) {
      return transpose().solve(B.transpose());
   }

   /* Matrix inverse or pseudoinverse
   return     inverse(A) if A is square, pseudoinverse otherwise.
   */

   proc inverse () {
      return solve(identity(m,m));
   }

   /* Matrix determinant
   return     determinant
   */

   proc det () {
      var toret : unmanaged LUDecomposition = new unmanaged LUDecomposition(_to_unmanaged(this));
      return toret.det();
   }

   /* Matrix rank
   return     effective numerical rank, obtained from SVD.
   */

   proc rank () {
      var toret : SingularValueDecomposition = new unmanaged SingularValueDecomposition(_to_unmanaged(this));
      return toret.rank();
   }

   /* Matrix condition (2 norm)
   return     ratio of largest to smallest singular value.
   */

   proc cond () {
      var toret : SingularValueDecomposition = new unmanaged SingularValueDecomposition(_to_unmanaged(this));
      return toret.cond();
   }

   /* Matrix trace.
   return     sum of the diagonal elements.
   */

   proc trace () {
      var mm = min(m,n);
      var Aslice = {1..mm, 1..mm};
      var t = (+ reduce A(Aslice));
      return t;
   }

   /* Generate matrix with random elements
   m    Number of rows.
   n    Number of columns.
   return     An m-by-n matrix with uniformly distributed random elements.
   */

   proc random (m, n:int) {
      var A = new unmanaged Matrix(m,n);
      var randlist = new owned RandomStream(real, seed);
      randlist.fillRandom(A.A);
      return A;
   }

   /* Check if size(A) == size(B) */

   proc checkMatrixDimensions (B: unmanaged Matrix) {
      if (B.m != m || B.n != n) {
         assert((B.m != m || B.n != n), "Matrix dimensions must agree.");
      }
   }

}

proc random (m, n:int) {
   var A = new unmanaged Matrix(m,n);
   var randlist = new owned RandomStream(real, seed);
   randlist.fillRandom(A.A);
   return A;
}


/* QR Decomposition.

   For an m-by-n matrix A with m >= n, the QR decomposition is an m-by-n
   orthogonal matrix Q and an n-by-n upper triangular matrix R so that
   A = Q*R.

   The QR decomposition always exists, even if the matrix does not have
   full rank, so the constructor will never fail.  The primary use of the
   QR decomposition is in the least squares solution of nonsquare systems
   of simultaneous linear equations.  This will fail if isFullRank()
   returns false.
*/

class QRDecomposition {

   /* Row and column dimensions.
   column dimension.
   row dimension.
   */
   var m, n:int;

   /* Array for internal storage of decomposition.
   */
   var qrDom = {1..1,1..1};
   var QR : [qrDom] real;

   /* Array for internal storage of diagonal of R.
   */
   var rdiagDom = {1..1};
   var Rdiag : [rdiagDom] real;

   /* QR Decomposition, computed by Householder reflections.
       Structure to access R and the Householder vectors and compute Q.
   A    Rectangular matrix
   */

   proc init (A: unmanaged Matrix) {
      // Initialize.
      m = A.getRowDimension();
      n = A.getColumnDimension();

      qrDom = {1..m, 1..n};
      QR = A.getArrayCopy();
      rdiagDom = {1..n};
      this.complete();

      // Main loop.
      for k in rdiagDom {
         // Compute 2-norm of k-th column without under/overflow.
         var nrm = 0.0;
         for i in k..m {
            nrm = hypot(nrm,QR[i,k]);
         }

         if (nrm != 0.0) {
            // Form k-th Householder vector.
            if (QR[k,k] < 0) {
               nrm = -nrm;
            }
            for i in k..m {
               QR[i,k] /= nrm;
            }
            QR[k,k] += 1.0;

            // Apply transformation to remaining columns.
            for j in k+1..n {
               var s = 0.0;
               for i in k..m {
                  s += QR[i,k]*QR[i,j];
               }
               s = -s/QR[k,k];
               for i in k..m {
                  QR[i,j] += s*QR[i,k];
               }
            }
         }
         Rdiag[k] = -nrm;
      }
   }

   /* Is the matrix full rank?
   return     true if R, and hence A, has full rank.
   */

   proc isFullRank () {
      for j in Rdiag.domain {
         if (Rdiag[j] == 0) {
            return false;
         }
      }
      return true;
   }

   /* Return the Householder vectors
   return     Lower trapezoidal matrix whose columns define the reflections
   */

   proc getH () {
      var X = new unmanaged Matrix(m,n);
      var H = X.getArray();
      for (i,j) in {1..m, 1..n} {
            if (i >= j) {
               H[i,j] = QR[i,j];
            } else {
               H[i,j] = 0.0;
            }
      }
      return X;
   }

   /* Return the upper triangular factor
   return     R
   */

   proc getR () {
      var X = new unmanaged Matrix(n,n);
      // BUG HERE! Tried to make this a ref to speed things up - it got weird b/c
      // making this a ref instead of a var makes the output from getQ change.
      //
      var retR = X.A;
      for (i,j) in {1..n, 1..n} {
            if (i < j) {
               retR[i,j] = QR[i,j];
            } else if (i == j) {
               retR[i,j] = Rdiag[i];
            } else {
               retR[i,j] = 0.0;
            }
      }
      X.A = retR;
      return X;
   }

   /* Generate and return the (economy-sized) orthogonal factor
   return     Q
   */

   proc getQ () {
      var X = new unmanaged Matrix(m,n);
      var retQ = X.A;

      for k in 1..n-1 by -1 {
         for i in 1..m {
            retQ[i,k] = 0.0;
         }
         retQ[k,k] = 1.0;
         for j in k..n {
            if (QR[k,k] != 0) {
               var s = 0.0;
               for i in k..m {
                  s += QR[i,k]*retQ[i,j];
               }
               s = -s/QR[k,k];
               for i in k..m {
                  retQ[i,j] += s*QR[i,k];
               }
            }
         }
      }

      X.A = retQ;
      return X;
   }

   /* Least squares solution of A*X = B
   B    A Matrix with as many rows as A and any number of columns.
   return     X that minimizes the two norm of Q*R*X-B.
   */

   proc solve (B: unmanaged Matrix) {
      if (B.getRowDimension() != m) {
         assert(B.getRowDimension() != m, "Matrix row dimensions must agree.");
      }
      if (!this.isFullRank()) {
         assert(!this.isFullRank(), "Matrix is rank deficient.");
      }

      // Copy right hand side
      var nx = B.getColumnDimension();
      var X = B.getArrayCopy();

      // Compute Y = transpose(Q)*B
      for k in 1..n {
         for j in 1..nx {
            var s = 0.0;
            const kmrng = {k..m};
            for i in kmrng {
               s += QR[i,k]*X[i,j];
            }
            s = -s/QR[k,k];
            for i in kmrng {
               X[i,j] += s*QR[i,k];
            }
         }
      }

      // Solve R*X = Y;
      for k in 1..n by -1 {
         for j in 1..nx {
            X[k,j] /= Rdiag[k];
         }

         for i in 1..k-1 { // SPECIAL SAUCE FOR CHAPEL
            for j in 1..nx {
               X[i,j] -= X[k,j]*QR[i,k];
            }
         }

      }

      var toret = new unmanaged Matrix(X,n,nx);
      return toret.getMatrix(1,n,1,nx);
   }

}

/*
   Singular Value Decomposition.

   For an m-by-n matrix A with m >= n, the singular value decomposition is
   an m-by-n orthogonal matrix U, an n-by-n diagonal matrix S, and
   an n-by-n orthogonal matrix V so that A = U*S*V'.

   The singular values, sigma[k] = S[k,k], are ordered so that
   sigma[0] >= sigma[1] >= ... >= sigma[n-1].

   The singular value decomposition always exists, so the constructor will
   never fail.  The matrix condition number and the effective numerical
   rank can be computed from this decomposition.
*/

class SingularValueDecomposition {

   /* Row and column dimensions.
   row dimension.
   column dimension.
   */
   var m, n : int;

   /* Array for internal storage of singular values.
   */
   var sDom = {1..1};
   var s : [sDom] real;

   /* Arrays for internal storage of U and V.
   internal storage of U.
   internal storage of V.
   */
   var uDom = {1..1,1..1};
   var vDom = {1..1,1..1};

   var U : [uDom] real;
   var V : [vDom] real;

   /* Construct the singular value decomposition
       Structure to access U, S and V.
   Arg    Rectangular matrix
   */

   proc init (Arg: unmanaged Matrix) {

      // Derived from LINPACK code.
      // Initialize.
      m = Arg.getRowDimension();
      n = Arg.getColumnDimension();

      /* Tactics...This algorithm assumes a zero-index language
         since chapel is one-index, it was decided for the
         sake of developer sanity to use chapel's alias
         functionality to temporarily use zero-indexing
       */

      var aDom = {0..m-1, 0..n-1};
      ref A = Arg.getArrayCopy().reindex(aDom); // alias the matrix

      /* Apparently the failing cases are only a proper subset of (m<n),
      so let's not throw error.  Correct fix to come later?
      */
      var nu = min(m,n);

      sDom = {1..min(m+1,n)};
      uDom = {1..m, 1..nu};
      vDom = {1..n, 1..n};

      this.complete();

      ref S = s.reindex({0..min(m+1,n)-1}); // chapel aliases saved the day!
      ref UU = U[uDom].reindex({0..m-1,0..nu-1});
      ref VV = V[vDom].reindex({0..n-1, 0..n-1});

      var e : [0..n-1] real;
      var work : [0..m-1] real;

      var wantu = true;
      var wantv = true;

      // Reduce A to bidiagonal form, storing the diagonal elements
      // in s and the super-diagonal elements in e.

      var nct = min(m-1,n);
      var nrt = max(1,min(n-1,m));

      for k in 0..max(nct,nrt)-1 {
         if (k <= nct) {

            // Compute the transformation for the k-th column and
            // place the k-th diagonal in s[k].
            // Compute 2-norm of k-th column without under/overflow.
            S[k] = 0.0;
            for i in k..m-1 {
               S[k] = hypot(S[k],A[i,k]);
            }

            if (S[k] != 0.0) {
               if (A[k,k] < 0.0) {
                  S[k] = -S[k];
               }
               for i in k..m-1 {
                  A[i,k] /= S[k];
               }
               A[k,k] += 1.0;
            }
            S[k] = -S[k];
         }

         for j in k+1..n-1 {
            if ((k <= nct) & (S[k] != 0.0))  {

            // Apply the transformation.

               var t = 0.0;
               for i in k..m-1 {
                  t += A[i,k]*A[i,j];
               }
               t = -t/A[k,k];
               for i in k..m-1 {
                  A[i,j] += t*A[i,k];
               }
            }

            // Place the k-th row of A into e for the
            // subsequent calculation of the row transformation.

            e[j] = A[k,j];
         }

         if (wantu & (k < nct)) {

            // Place the transformation in U for subsequent back
            // multiplication.

            for i in k..m-1 {
               UU[i,k] = A[i,k];
            }
         }

         if (k < nrt) {

            // Compute the k-th row transformation and place the
            // k-th super-diagonal in e[k].
            // Compute 2-norm without under/overflow.
            e[k] = 0;
            for i in k+1..n-1 {
               e[k] = hypot(e[k],e[i]);
            }

            if (e[k] != 0.0) {
               if (e[k+1] < 0.0) {
                  e[k] = -e[k];
               }
               for i in k+1..n-1 {
                  e[i] /= e[k];
               }
               e[k+1] += 1.0;
            }

            e[k] = -e[k];

            if ((k+1 < m) & (e[k] != 0.0)) {

            // Apply the transformation.
               work(k+1..m-1) = 0.0;

               for (j,i) in {k+1..n-1, k+1..m-1} {
                     work[i] += e[j]*A[i,j];
               }
               for j in k+1..n-1 {
                  var t = -e[j]/e[k+1];
                  for i in k+1..m-1 {
                     A[i,j] += t*work[i];
                  }
               }
            }
            if (wantv) {

            // Place the transformation in V for subsequent
            // back multiplication.

               for i in k+1..n-1 {
                  VV[i,k] = e[i];
               }
            }
         }
      }

      // Set up the final bidiagonal matrix or order p.

      var p = min(n,m+1);
      if (nct < n) {
         S[nct] = A[nct,nct];
      }
      if (m < p) {
         S[p-1] = 0.0;
      }
      if (nrt+1 < p) {
         e[nrt] = A[nrt,p-1];
      }
      e[p-1] = 0.0;

      // If required, generate U.

      if (wantu) {
         for j in nct..nu-1 {
            for i in 0..m-1 {
               UU[i,j] = 0.0;
            }
            UU[j,j] = 1.0;
         }
         for k in 0..nct-1 by -1 {
            if (S[k] != 0.0) {
               for j in k+1..nu-1 {
                  var t = 0.0;
                  for i in k..m-1 {
                     t += UU[i,k]*UU[i,j];
                  }
                  t = -t/UU[k,k];
                  for i in k..m-1 {
                     UU[i,j] += t*UU[i,k];
                  }
               }
               for i in k..m-1 {
                  UU[i,k] = -UU[i,k];
               }
               UU[k,k] = 1.0 + UU[k,k];
               for i in 0..k-2 {
                  UU[i,k] = 0.0;
               }
            } else {
               for i in 0..m-1 {
                  UU[i,k] = 0.0;
               }
               UU[k,k] = 1.0;
            }
         }
      }

      // If required, generate V.

      if (wantv) {
         for k in 0..n-1 by -1 {
            if ((k < nrt) & (e[k] != 0.0)) {
               for j in k+1..nu-1 {
                  var t = 0.0;
                  for i in k+1..n-1 {
                     t += VV[i,k]*VV[i,j];
                  }
                  t = -t/VV[k+1,k];
                  for i in k+1..n-1 {
                     VV[i,j] += t*VV[i,k];
                  }
               }
            }
            for i in 0..n-1 {
               VV[i,k] = 0.0;
            }
            VV[k,k] = 1.0;
         }
      }

      // Main iteration loop for the singular values.

      var pp = p-1;
      var itr = 0;
      var eps = 2.0 ** -52.0;
      var tiny = 2.0 ** -966.0;

      while (p > 0) {
         var k,kase : int;

         // Here is where a test for too many iterations would go.

         // This section of the program inspects for
         // negligible elements in the s and e arrays.  On
         // completion the variables kase and k are set as follows.

         // kase = 1     if s(p) and e[k-1] are negligible and k<p
         // kase = 2     if s(k) is negligible and k<p
         // kase = 3     if e[k-1] is negligible, k<p, and
         //              s(k), ..., s(p) are not negligible (qr step).
         // kase = 4     if e(p-1) is negligible (convergence).

         for z in -1..p-2 by -1 {
            if (z == -1) {
               k = z;
               break;
            }

            if (abs(e[z]) <= tiny + eps*(abs(S[z]) + abs(S[z+1]))) {
               e[z] = 0.0;
               k = z;
               break;
            }
         }

         if (k == p-2) {
            kase = 4;
         }
         else {
            var ks : int;
            for ksk in k..p-1 by -1 {
               if (ksk == k) {
                  ks = ksk;
                  break;
               }
               var t = if(ksk != p) then abs(e[ksk]) else 0.0 +
                          if(ksk != k+1) then abs(e[ksk-1]) else 0.0;

               if (abs(S[ksk]) <= tiny + eps*t)  {
                  S[ksk] = 0.0;
                  ks = ksk;
                  break;
               }
            }

            if (ks == k) {
               kase = 3;
            } else if (ks == p-1) {
               kase = 1;
            } else {
               kase = 2;
               k = ks;
            }
         }
         k+=1;
         // Perform the task indicated by kase.

         select (kase) {

            // Deflate negligible s(p).

            when 1 {
               var f = e[p-2];
               e[p-2] = 0.0;
               for j in k..p-2 by -1 {
                  var t = hypot(S[j],f);
                  var cs = S[j]/t;
                  var sn = f/t;
                  S[j] = t;
                  if (j != k) {
                     f = -sn*e[j-1];
                     e[j-1] = cs*e[j-1];
                  }
                  if (wantv) {
                     for i in 0..n-1 {
                        t = cs*VV[i,j] + sn*VV[i,p-1];
                        VV[i,p-1] = -sn*VV[i,j] + cs*VV[i,p-1];
                        VV[i,j] = t;
                     }
                  }
               }
            }

            // Split at negligible s(k).

            when 2 {
               var f = e[k-1];
               e[k-1] = 0.0;
               for j in k..p-1 {
                  var t = hypot(S[j],f);
                  var cs = S[j]/t;
                  var sn = f/t;
                  S[j] = t;
                  f = -sn*e[j];
                  e[j] = cs*e[j];
                  if (wantu) {
                     for i in 0..m-1 {
                        t = cs*UU[i,j] + sn*UU[i,k-1];
                        UU[i,k-1] = -sn*UU[i,j] + cs*UU[i,k-1];
                        UU[i,j] = t;
                     }
                  }
               }
            }

            // Perform one qr step.

            when 3 {

               // Calculate the shift.

               var scale = max(max(max(max(abs(S[p-1]),abs(S[p-2])),abs(e[p-2])),
                                           abs(S[k])),abs(e[k]));

               var sp = S[p-1]/scale;
               var spm1 = S[p-2]/scale;
               var epm1 = e[p-2]/scale;
               var sk = S[k]/scale;
               var ek = e[k]/scale;
               var b = ((spm1 + sp)*(spm1 - sp) + epm1*epm1)/2.0;
               var c = (sp*epm1)*(sp*epm1);
               var shift = 0.0;
               if ((b != 0.0) | (c != 0.0)) {
                  shift = ((b*b)+c) ** 0.5;
                  if (b < 0.0) {
                     shift = -shift;
                  }
                  shift = c/(b + shift);
               }
               var f = (sk + sp)*(sk - sp) + shift;
               var g = sk*ek;

               // Chase zeros.

               for j in k..p-2 {
                  var t = hypot(f,g);
                  var cs = f/t;
                  var sn = g/t;
                  if (j != k) {
                     e[j-1] = t;
                  }
                  f = cs*S[j] + sn*e[j];
                  e[j] = cs*e[j] - sn*S[j];
                  g = sn*S[j+1];
                  S[j+1] = cs*S[j+1];
                  if (wantv) {
                     for i in 0..n-1 {
                        t = cs*VV[i,j] + sn*VV[i,j+1];
                        VV[i,j+1] = -sn*VV[i,j] + cs*VV[i,j+1];
                        VV[i,j] = t;
                     }
                  }
                  t = hypot(f,g);
                  cs = f/t;
                  sn = g/t;
                  S[j] = t;
                  f = cs*e[j] + sn*S[j+1];
                  S[j+1] = -sn*e[j] + cs*S[j+1];
                  g = sn*e[j+1];
                  e[j+1] = cs*e[j+1];
                  if (wantu && (j < m-1)) {
                     for i in 0..m-1 {
                        t = cs*UU[i,j] + sn*UU[i,j+1];
                        UU[i,j+1] = -sn*UU[i,j] + cs*UU[i,j+1];
                        UU[i,j] = t;
                     }
                  }
               }
               e[p-2] = f;
               itr = itr + 1;
            }

            // Convergence.

            when 4 {

               // Make the singular values positive.

               if (S[k] <= 0.0) {
                  S[k] = if(S[k] < 0.0) then -S[k] else 0.0;
                  if (wantv) {
                     for i in 0..pp-1 {
                        VV[i,k] = -VV[i,k];
                     }
                  }
               }

               // Order the singular values.

               while (k < pp) {
                  if (S[k] >= S[k+1]) {
                     break;
                  }
                  var t = S[k];
                  S[k] = S[k+1];
                  S[k+1] = t;
                  if (wantv && (k < n-1)) {
                     for i in 0..n-1 {
                        t = VV[i,k+1]; VV[i,k+1] = VV[i,k]; VV[i,k] = t;
                     }
                  }
                  if (wantu && (k < m-1)) {
                     for i in 0..m-1 {
                        t = UU[i,k+1]; UU[i,k+1] = UU[i,k]; UU[i,k] = t;
                     }
                  }
                  k+=1;
               }
               itr = 0;
               p-=1;
            }
         }
      }
   }

   /* Return the left singular vectors
   return     U
   */

   proc getU () {
      return new unmanaged Matrix(U,m,min(m+1,n));
   }

   /* Return the right singular vectors
   return     V
   */

   proc getV () {
      return new unmanaged Matrix(V,n,n);
   }

   /* Return the one-dimensional array of singular values
   return     diagonal of S.
   */

   proc getSingularValues () {
      return s;
   }

   /* Return the diagonal matrix of singular values
   return     S
   */

   proc getS () {
      var X = new unmanaged Matrix(n,n);
      var S = X.getArray();
      for i in 1..n {
         for j in 1..n {
            S[i,j] = 0.0;
         }
         S[i,i] = this.s[i];
      }
      return X;
   }

   /* Two norm
   return     max(S)
   */

   proc norm2 () {
      return s[1];
   }

   /* Two norm condition number
   return     max(S)/min(S)
   */

   proc cond () {
      return s[1]/s[min(m,n)-1];
   }

   /* Effective numerical matrix rank
   return     Number of nonnegligible singular values.
   */

   proc rank () {
      var eps = 2.0 ** -52.0;
      var tol = max(m,n)*s[1]*eps;
      var r = 0;

      for i in 1..s.domain.high {
         if (s[i] > tol) {
            r+=1;
         }
      }
      return r;
   }

}

} // end of module
