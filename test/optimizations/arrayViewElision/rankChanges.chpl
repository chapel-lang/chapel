config param dist = "stencil";

param aVal = 1, bVal = 2;

var arrRanges = (1..5, 1..5);

var (A, B) = createArrays();


// supported:
A[3, 1..3] = B[3, 1..3]; testAndReset();
A[1..3, 3] = B[1..3, 3]; testAndReset();

// supported:
A[3, ..] = B[3, ..]; testAndReset();
A[.., 3] = B[.., 3]; testAndReset();

// unsupported:
A[3, 1..3] = B[1..3, 3]; testAndReset();
A[1..3, 3] = B[3, 1..3]; testAndReset();

proc testAndReset() {
  writeln(A);
  writeln();
  A = 1;
}

proc createArrays() {
  select dist {
    when "default" {
      var A: [(...arrRanges)] int = aVal;
      var B: [(...arrRanges)] int = bVal;
      A = aVal;
      B = bVal;
      return (A, B);
    }
    when "block" {
      use BlockDist;
      var A = blockDist.createArray((...arrRanges), int);
      var B = blockDist.createArray((...arrRanges), int);
      A = aVal;
      B = bVal;
      return (A, B);
    }
    when "cyclic" {
      use CyclicDist;
      var A = cyclicDist.createArray((...arrRanges), int);
      var B = cyclicDist.createArray((...arrRanges), int);
      A = aVal;
      B = bVal;
      return (A, B);
    }
    when "stencil" {
      use StencilDist;
      var A = stencilDist.createArray((...arrRanges), int);
      var B = stencilDist.createArray((...arrRanges), int);
      A = aVal;
      B = bVal;
      return (A, B);
    }
  }
}

