//linear algebra
//compilation error
use LinearAlgebra;

@unstable("This Matrix which uses 'Linear Algebra' is unstable") var A = Matrix(4,4, eltType=int);
A = 3;
writeln(A);
//Prints:
  //   3 3 3 3
  //   3 3 3 3
  //   3 3 3 3
  //   3 3 3 3

writeln("matrix rank: ", A.rank); //2
writeln("matrix size: ", A.size); //16

//the shape is kind of redundant
writeln("matrix shape: ", A.shape); //(4, 4)

