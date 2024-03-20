
use common;

{
  var D = createDom({1..10});

  var A: [D] real;
  var B: [D] real;
  var C: [D] real;

  forall i in D with (ref A) {
    forall j in 1..10 with (ref A) { // this'll make C[j] a candidate that's not optimized
      A[i] = B[i] + C[j];
    }
  }
}

{
  var D = createDom({1..10});

  var A: [D] real;
  var B: [D] real;
  var C: [D] real;

  forall i in D with (ref A) {
    on Locales[0] {
      A[i] = B[i] + C[i];
    }
  }

}
