use MatrixMult;

proc main() {

  const D = {1..3,1..3};
  const DB = {2..4,2..4};
  var A,G: [D] real;
  var B: [DB] real;

  A = 2.0;
  B = 3.0;

  writeln("Testing multiplication of square matrices");
  writeln("-----------------------------------------------------------------");
  writeln();

  var C = A*B;
  G = A*B;

  writeln("A = ");
  writeln(A);
  writeln("Domain for A");
  writeln(A.domain);
  writeln("B = ");
  writeln(B);
  writeln("Domain for B");
  writeln(B.domain);
  writeln("var C = A*B =");
  writeln(C);
  writeln("Domain for C");
  writeln(C.domain);
  writeln("(G has been declared with same domain as A) G = A*B =");
  writeln(G);
  writeln("Domain for G");
  writeln(G.domain);

  writeln();
  writeln("Testing multiplication of a matrix by a scalar");
  writeln("-----------------------------------------------------------------");
  writeln();
 
  var F = 2*B;
  writeln("var F = 2*B =");
  writeln(F);
  B = 2*B;
  writeln("B = 2*B =");
  writeln(B);

  writeln();
  writeln("Testing multiplication of rectangular matrices of different types");
  writeln("-----------------------------------------------------------------");
  writeln();

  const D1 = {1..4,1..2};
  const D2 = {1..2,1..2};

  var L:[D1] real;
  var M: [D1] int;
  var N: [D2] int;
  var O: [D2] real;

  [i in D1] L(i) = i(0):real;
  [i in D1] M(i) = i(1); 
 
  N = 2;
  O = 3.0;

  var P = L*N;
  writeln("L = ");
  writeln(L);
  writeln("N = ");
  writeln(N);
  writeln("var P = L*N =");
  writeln(P);
  var Q = M*O;
  writeln("M = ");
  writeln(M);
  writeln("O = ");
  writeln(O);
  writeln("var Q = M*O =");
  writeln(Q);
  var R = N*N;
  writeln("N = ");
  writeln(N);
  writeln("var R = N*N = ");
  writeln(R);
  
}
