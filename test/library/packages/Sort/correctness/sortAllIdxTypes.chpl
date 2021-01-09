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

proc testsize(param s) {
  var A : [1:uint(s)..10:uint(s)] int;
  forall i in A.domain do
    A[i] = (10-i):int;
  test(A);

  var B : [1:uint(s)..254:uint(s)] int;
  forall i in B.domain do
    B[i] = (B.size-i):int;

  test(B);

  var C : [-63:int(s)..63:int(s)] int;
  forall i in C.domain do
    C[i] = C.size-i;

  test(C);
}

testsize(8);
testsize(16);
testsize(32);
testsize(64);
