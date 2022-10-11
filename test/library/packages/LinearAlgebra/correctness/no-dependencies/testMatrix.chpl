
use TestUtils;
use LinearAlgebra;


/* Return a square matrix (2D array) over domain ``{0..<rows, 0..<rows}``*/
{
  var m = 2;
  var rangeTrue : range = 0..<m;
  var A = Matrix(m);
  assertEqual(A.dom.ranges(0),rangeTrue,'row range incorrect');
  assertEqual(A.dom.ranges(1),rangeTrue,'column range incorrect');
}


/* Return a matrix (2D array) over domain ``{0..<rows, 0..<cols}``*/
{
  var n = 4;
  var m = 2;
  var rowRange : range = 0..<m;
  var colRange : range = 0..<n;
  var A = Matrix(m,n);
  assertEqual(A.dom.ranges(0),rowRange,'row range incorrect');
  assertEqual(A.dom.ranges(1),colRange,'column range incorrect');
}


/* Return a square matrix (2D array) over domain ``{space, space}`` */
{ // non-strided
  var rangeTrue = 0..<4;
  var A = Matrix(rangeTrue);
  assertEqual(A.dom.ranges(0),rangeTrue,'row range incorrect');
  assertEqual(A.dom.ranges(1),rangeTrue,'column range incorrect');
}
{ // strided
  var rangeTrue = 0..<4 by 2;
  var A = Matrix(rangeTrue);
  assertEqual(A.dom.ranges(0),rangeTrue,'row range incorrect');
  assertEqual(A.dom.ranges(1),rangeTrue,'column range incorrect');
}


/* Return a matrix (2D array) over domain ``{rowSpace, colSpace}`` */
{ // non-strided
  var rowRange = 0..<4;
  var colRange = 0..<2;
  var A = Matrix(rowRange,colRange);
  assertEqual(A.dom.ranges(0),rowRange,'row range incorrect');
  assertEqual(A.dom.ranges(1),colRange,'column range incorrect');
}
{ // strided
  var rowRange = 0..<4 by 2;
  var colRange = 0..<2 by 2;
  var A = Matrix(rowRange,colRange);
  assertEqual(A.dom.ranges(0),rowRange,'row range incorrect');
  assertEqual(A.dom.ranges(1),colRange,'column range incorrect');
}


/* Return a matrix (2D array) with domain and values of ``A``.
   ``A`` can be sparse (CS) or dense. */
{ // dense
  var m = 4;
  var n = 2;
  var rowRange = 0..<m;
  var colRange = 0..<n;
  var dom : domain = {rowRange,colRange};
  var A : [dom] int;
  A[0,0] = 1;
  A[3,0] = 1;
  var ACastReal = A : real; // can't call assertEqual(A : real,...)
  var AMatrix = Matrix(A,eltType=real);
  assertEqual(AMatrix.dom.ranges(0),rowRange,'row range incorrect');
  assertEqual(AMatrix.dom.ranges(1),colRange,'column range incorrect');
  if (AMatrix.eltType != real) then writeln('element type incorrect');
  assertEqual(ACastReal,AMatrix,'casting error');
}
{ // sparse
  var m = 4;
  var n = 2;
  var rowRange = 0..<m;
  var colRange = 0..<n;
  var dnsDomain : domain = {rowRange,colRange};
  var spsDom : sparse subdomain(dnsDomain);
  spsDom += (0,0);
  spsDom += (3,0);
  var A : [spsDom] int;
  A[0,0] = 1;
  A[3,0] = 1;
  var AMatrix = Matrix(A,eltType=real);
  if (AMatrix.eltType != real) then writeln('element type incorrect');
  if ((A[0,0] : real) != AMatrix[0,0]) then writeln('casting error');
  if ((A[3,0] : real) != AMatrix[3,0]) then writeln('casting error');
  if ((A[2,0] : real) != AMatrix[2,0]) then writeln('IRV error');
}

/* Return a matrix (2D array), given 2 or more vectors, such that the
   vectors form the rows of the matrix */
{ // no inference or casting
  var dom : domain = {0..<2,0..<2};
  var A : [dom] int;
  A[0,0] = 1;
  A[1,0] = 2;
  var AAssembled = Matrix(A[0,..],A[1,..]);
  assertEqual(AAssembled,A,'matrix assembly error');
  if (AAssembled.eltType != int) then writeln('type inference incorrect');
}
{ // casting
  var dom : domain = {0..<2,0..<2};
  var A : [dom] real;
  A[0,0] = 1.0;
  A[1,0] = 2.0;
  var AAssembled = Matrix([1,0],[2,0],eltType=real);
  assertEqual(AAssembled,A,'matrix assembly error (casting)');
  if (AAssembled.eltType != real) then writeln('casting incorrect');
}
{ // type inference
  var dom : domain = {0..<2,0..<2};
  var A : [dom] real;
  A[0,0] = 1.0;
  A[1,0] = 2.0;
  var AAssembled = Matrix([1,0.0],[2,0]);
  assertEqual(AAssembled,A,'matrix assembly error (inference)');
  if (AAssembled.eltType != real) then writeln('type inference incorrect');
}
