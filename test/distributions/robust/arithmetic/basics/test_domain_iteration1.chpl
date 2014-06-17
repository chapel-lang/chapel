use driver_arrays;

proc foo(Dom, Arr) {
  for i in Dom do
    Arr(i) = next();

  forall i in Dom do
    Arr(i) += 2;
}

foo(Dom1D, A1D);
foo(Dom2D, A2D);
foo(Dom3D, A3D);
foo(Dom4D, A4D);
foo(Dom2D32, A2D32);

outputArrays();
