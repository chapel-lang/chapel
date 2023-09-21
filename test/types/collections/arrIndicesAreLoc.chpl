use BlockDist;

var A = blockDist.createArray({1..10}, real);

forall i in A.indices with (ref A) do
  A[i] = here.id;

writeln(A);

on Locales[numLocales-1] {
  forall i in A.indices with (ref A) do
    A[i] = here.id;
}

writeln(A);
