const Space = {1..10};

// not exhaustive.
{
  use BlockDist;

  const D = Space dmapped new blockDist(Space);
  var A: [D] int = 1;
  var B: [D] int = 2;

  A[3..5] = B[3..5];
  writeln(A);
}

{
  use CyclicDist;

  const D = Space dmapped new cyclicDist(Space.first);
  var A: [D] int = 1;
  var B: [D] int = 2;

  A[3..5] = B[3..5];
  writeln(A);
}

{
  use StencilDist;

  const D = Space dmapped new stencilDist(Space);
  var A: [D] int = 1;
  var B: [D] int = 2;

  A[3..5] = B[3..5];
  writeln(A);
}
