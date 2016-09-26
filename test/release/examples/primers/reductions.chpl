// Reductions

//
// This primer includes example reductions.
//
// An array is filled with random values and the locations of the
// maximum and minimum are found. The Euclidean norm of the array's
// columns is computed using ``+`` reductions over slices of ``A``.  Finally,
// an ``&&`` reduction is used to compute whether all values in ``A`` are
// greater than 0.25 and the results of the computations are printed.
//

//
// For an example usage of the standard module Random, see
// its :ref:`primer <primers-randomNumbers>`.
//
use Random; // For random number generation

config const seed = 31415; // Random generation seed
config const size = 10;    // The size of each side of the array

var A: [1..size, 1..size] real; // The 2D work array

/*
   Fill an array with random values
   --------------------------------

   Fill ``A`` with random real values between 0 and 1.
   Uses the NPB random number generator for historical reasons.
*/

fillRandom(A, seed, algorithm=RNG.NPB);
writeln("A is: "); writeln(A);
writeln();

/*
   Find the average value of an array
   -----------------------------------

   We find the average value of the array by summing over its elements
   and dividing by the number of elements it contains.
*/

var eltAvg = (+ reduce A) / size**2;
writeln("The average element of A has the value ", eltAvg);
writeln();

/*
   Find the 1-norm of an array
   ---------------------------

   We can find the 1-norm of ``A`` by summing over the absolute value of the
   elements.

   The expression ``abs(A)`` creates a new matrix which contains in each of its
   elements the absolute value of the corresponding element in ``A``.
   The ``+ reduce`` clause just sums these up.
*/

var oneNorm = + reduce abs(A);
writeln("The 1-norm of A is ", oneNorm);
writeln();

/*
   The Frobenius norm
   ------------------

   The `Frobenius norm`_ is the square root of the sum over all elements
   of their respective squares.

   The expression below can be broken down thus:

    1) The Frobenius norm is the square root of some quantity.
    2) The quantity is the sum over all elements of a matrix.
    3) That matrix is the promotion of ``A`` by ``**2``.
       That is, each of its elements is the square
       of the corresponding element of ``A``.
*/

var frobNorm = sqrt(+ reduce A**2);
writeln("The Frobenius norm of A is ", frobNorm);
writeln();

/*
   ``maxloc`` and ``minloc`` reductions
   ------------------------------------

   Apply ``minloc`` and ``maxloc`` reductions. We capture the results into
   the ``maxVal``, ``maxLoc``, ``minVal``, ``minLoc`` variables.

   ``maxloc`` and ``minloc`` reductions expect a 2-tuple argument
   that can be iterated over using zippered iteration. They produce
   a 2-tuple result. The first component of the result is the
   maximum (or minimum) over the first component of the argument.
   The second component of the result indicates its location, i.e.
   the corresponding element of the second component of the argument.
*/

var (maxVal, maxLoc) = maxloc reduce zip(A, A.domain);
var (minVal, minLoc) = minloc reduce zip(A, A.domain);
writeln("The maximum value in A is: A", maxLoc, " = ", maxVal);
writeln("The minimum value in A is: A", minLoc, " = ", minVal);
writeln("The difference is: ", maxVal - minVal);
writeln();

/*
   The Euclidean norm
   ------------------

   Compute `Euclidean norms`_ for each column using ``+`` reductions.
  
   Breaking down the statement below:

    1) ``vecNorms`` is a 1-D array containing ``size`` elements
       (indexed by ``1..size``).
    2) The elements of ``vecNorms`` are the square-root of some quantity.
    3) The quantity is the sum over all of the elements of some vector.
    4) The vector consists of the promotion of the j-th column of ``A``
       by ``**2``. That is, each element of that column vector is squared.
*/

var vecNorms = [j in 1..size] sqrt(+ reduce A(1..size, j)**2);
writeln("The Euclidean norm of each column is: ", vecNorms);
writeln();

/*
   ``&&`` reduction
   ----------------

   Use the ``&&`` reduction to compute if all values in ``A`` are greater
   than 0.25.

   The parenthesized value is the promotion of ``A`` by ``> 0.25``. This yields
   an array of the same size as ``A``, containing boolean values that are true
   if the corresponding element in ``A`` exceeds 0.25 and false otherwise.

   The clause ``&& reduce`` computes the result of applying the Boolean AND
   operator between all elements of the promoted array.
*/

var onlyBigValues = && reduce (A > 0.25);
if onlyBigValues then
  writeln("The values in A are all greater than 0.25");
else
  writeln("Some values in A are less than 0.25");
writeln();

//.. _Frobenius norm: https://en.wikipedia.org/wiki/Matrix_norm#Frobenius_norm
//.. _Euclidean norms: https://en.wikipedia.org/wiki/Norm_(mathematics)#Euclidean_norm
