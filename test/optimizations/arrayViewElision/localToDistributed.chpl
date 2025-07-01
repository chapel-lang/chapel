const Space = {1..10};

// not exhaustive.
{
  use BlockDist;

  const D = Space dmapped new blockDist(Space);
  var A: [D] int = 1;
  var B: [Space] int = 2;

  A[3..5] = B[3..5];
  writeln(A);
}

{
  // this is not supported today. Relevant doi functions need to be adjusted.
  // This is not an AVE limitation.
  use CyclicDist;

  const D = Space dmapped new cyclicDist(Space.first);
  var A: [D] int = 1;
  var B: [Space] int = 2;

  A[3..5] = B[3..5];
  writeln(A);
}

{
  use StencilDist;

  const D = Space dmapped new stencilDist(Space);
  var A: [D] int = 1;
  var B: [Space] int = 2;

  A[3..5] = B[3..5];
  writeln(A);
}
