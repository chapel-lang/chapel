use Sort, Sort.ShellSort;

proc test(X) {
  writeln("[", X.domain, " (", X.domain.idxType:string, ")] int");
  var Y = X;
  writeln("unsorted: ", X);
  shellSort(X);
  writeln("s-sorted: ", X);
  sort(Y);
  writeln("  sorted: ", Y);
  writeln();
}

var A : [1:uint(8)..10:uint(8)] int;
forall i in A.domain do
  A[i] = 10-i;
test(A);

var B : [1:uint(8)..254:uint(8)] int;
forall i in B.domain do
  B[i] = B.size-i;

test(B);

var C : [-63:int(8)..63:int(8)] int;
forall i in C.domain do
  C[i] = C.size-i;

test(C);
