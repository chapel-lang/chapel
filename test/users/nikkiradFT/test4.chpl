//linear algebra
//compilation error
use LinearAlgebra;

var A = Matrix(4,4, eltType=int);
A = 3;
writeln(A);
//Prints:
  //   3 3 3 3
  //   3 3 3 3 
  //   3 3 3 3

writeln("matrix rank: ", A.rank); //2
writeln("matrix size: ", A.size); //16

//the shape is kind of redundant
@unstable "The function 'shape' within the module 'Linear Algebra' is unstable" //see .good file -- might not be correct
writeln("matrix shape: ", A.shape); //(4, 4)


//A.shape; definition is in the linear algebra library itself

/* 
put unstable on a statement instead of a symbol
putting it when it's not actually applicable -- so it can be used to track a different type of error
incorrect use of the unstable keyword, if they try to write on something it cant be applied to (a statement)
it will be given a syntax error because it won't recognize/parse it

3 3 3 3
3 3 3 3 
3 3 3 3
matrix rank: 2
matrix size: 16
matrix shape: (4, 4)
*/