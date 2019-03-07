use LinearAlgebra;

// Test to confirm we get an error thrown for invalid input

proc main() throws {

  var A = Matrix([1.0, 2.0, 3.0],
                 [3.0, 2.0, 4.0],
                 [1.0, 2.0, NAN]);

  try {
    var (u, s, vt) = svd(A);
  } catch e:LinearAlgebraError {
    writeln(e.info);
  }
}
