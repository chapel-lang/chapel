use Sort.ShellSort;

var A : [1:uint(8)..10:uint(8)] int;
forall i in A.domain do
  A[i] = 10-i;

writeln(A);
shellSort(A);
writeln(A);
