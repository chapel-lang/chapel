
use BlockDist;

{
  var D = newBlockDom({1..10});

  var A: [D] real;
  var B: [D] real;
  var C: [D] real;

  forall (i, loopIdx) in zip(D, 1..) {
    forall j in 1..10 {
      A[i] = B[i] + C[j] * loopIdx;
    }
  }
}

{
  var D = newBlockDom({1..10});

  var A: [D] real;
  var B: [D] real;
  var C: [D] real;

  forall (i, loopIdx) in zip(D, 1..) {
    on Locales[0] {
      A[i] = B[i] + C[i] * loopIdx;
    }
  }

}
