//
// Miscellaneous assignments that exposed issues while reworking the strided
// transfer optimization.
//

use util;

proc main() {
  {
    var A, B : [1..10, 1..10, 1..10, 1..10] int;
    const left = {1..10 by 3, 1..1, 1..3, 1..10};
    const right = {1..10 by 3, 10..10, 1..9 by 3, 1..10};
    stridedAssign(A[left], B[right]);
  }

  {
    var A, B : [1..10, 1..10, 1..10] int;
    const left = {1..10, 1..1, 1..3};
    const right = {1..10, 10..10, 1..9 by 3};
    stridedAssign(A[left], B[right]);
  }

  {
    var A : [1..20, 1..10, 1..10] int;
    var B : [1..10, 1..20, 1..10] int;
    const left = {1..20 by 2, 1..10, 1..10};
    const right = {1..10, 1..20 by 2, 1..10};
    stridedAssign(A[left], B[right]);
  }

  {
    var A, B : [1..10, 1..10, 1..10] int;
    const left = {1..10, 6..10, 1..10};
    stridedAssign(A[left], B[1..10, 1..10 by 2, 1..10]);
  }

  {
    var A, B : [1..10, 1..10] int;
    stridedAssign(A[1..10, 1..5], B[1..10, 1..10 by 2]);
  }

  {
    var A : [1..10, 1..10] int;
    var B : [1..10, 1..20] int;
    const left = {1..10, 1..10};
    const right = {1..10, 1..20 by 2};
    stridedAssign(A[left], B[right]);
  }

  {
    var A, B : [1..4, 1..4, 1..4] int;
    const dom = {1..2 by 1, 1..3 by 1, 2..4 by 4};
    stridedAssign(A[dom], B[dom]);
  }

  {
    var A, B : [1..1, 1..1, 1..10 by 20, 1..10] int;
    stridedAssign(A, B);
  }

  {
    var A, B : [1..10, 1..10, 1..10] int;
    const dom = (1..10, 1, 1..10);
    stridedAssign(A[(...dom)], B[(...dom)]);
  }

  {
    var A, B : [1..20 by 2, 1..10, 1..30 by 3] int;
    stridedAssign(A, B);
  }

  {
    var A, B : [1..4] int;
    const left = 2..4 by 2 align 1;
    const right = 1..3 by 2 align 0;
    stridedAssign(A[left], B[right]);
  }

  {
    var A, B : [2..6 by -2] int;
    stridedAssign(A, B);
  }

  {
    var A, B : [1..4, 1..4, 1..4, 1..4] int;
    stridedAssign(A[1, 1..4, 1..4, 1], B[1..4, 1, 1, 1..4]);
  }
  {
    // Based on #9457
    var A : [1..8] int = 1..8;
    ref reA = A.reindex(1..8);
    var temp = reA[5..8];
    stridedAssign(reA[1..4], temp);
  }
}
