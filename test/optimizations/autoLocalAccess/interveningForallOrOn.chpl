
use BlockDist;

{
  var D = newBlockDom({1..10});

  var A: [D] real;
  var B: [D] real;
  var C: [D] real;

  forall i in D {
    forall j in 1..10 {
      A[i] = B[i] + C[j];
    }
  }
}

{
  var D = newBlockDom({1..10});

  var A: [D] real;
  var B: [D] real;
  var C: [D] real;

  forall i in D {
    on Locales[0] {
      A[i] = B[i] + C[i];
    }
  }

}
