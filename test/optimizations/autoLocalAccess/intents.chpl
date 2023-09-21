use BlockDist;

var D = blockDist.createDomain(1..10, 1..10);

{
  var A: [D] 2*int;
  forall i in D with (ref A) {
    A[i] = i;
  }
}

/*
 Not optimized today: setting up a->A matching happens during normalize in a way
 that I can't grok in a 5-minute check. Initial analysis for this optimization
 happens before normalization, so we don't have the a->A relation readily
 available just yet. Which is admittedly strange.

{
  var A: [D] 2*int;
  forall i in D with (ref a = A) {
    a[i] = i;
  }
}

*/
