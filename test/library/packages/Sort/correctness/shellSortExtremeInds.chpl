use Sort.ShellSort;

var A : [0x7fffffffffffff70..0x7ffffffffffffff0 ] int;
forall i in A.domain do
  A[i] = (A.size - (i - A.domain.low));

writeln(A);
shellSort(A, start=0x7fffffffffffff80, end=0x7ffffffffffffff0);
writeln(A);
