record Matrix {
  type elt_type;
  var m, n: int;
  var D: domain(2) = {1..m, 1..n};
  var A: [D] elt_type;

  proc ref this(i: int, j: int) ref return A(i,j);
  proc const ref this(i: int, j: int) const ref return A(i,j);
}

proc Matrix.writeThis(f) throws {
  f.write(A);
}

proc Matrix.transpose() {
  var M = new Matrix(elt_type, n, m);
  for (i,j) in D do
    M(j,i) = this(i,j);
  return M;
}

proc +(M1: Matrix, M2: Matrix) {
  if M1.m != M2.m || M1.n != M2.n then
    halt("illegal matrix + operation");
  var M3 = new Matrix((M1(1,1)+M2(1,1)).type, M1.m, M1.n);
  M3.A = M1.A + M2.A;
  return M3;
}

proc *(M1: Matrix, M2: Matrix) {
  if M1.n != M2.m then
    halt("illegal matrix * operation");
  var M3 = new Matrix((M1(1,1)*M2(1,1)).type, M1.m, M2.n);
  [(i,j) in M3.D with (ref M1, ref M2, ref M3)] M3(i,j) =
   + reduce [k in M1.D.dim(2)] (M1(i,k) + M2(k,j));
  return M3;
}

////////
// test code

var m = 8, n = 4;
var M = new Matrix(real, m, n);
var N = new Matrix(real, m, n);

for (i,j) in {1..m, 1..n} {
  M(i,j) = i-1 + (j-1)*m;
  N(i,j) = (i-1)*n + (j-1);
}

writeln(M);
writeln();
writeln(N);
writeln();
writeln(M+N);
writeln();
writeln(M.transpose());
writeln();
writeln(M*M.transpose());
