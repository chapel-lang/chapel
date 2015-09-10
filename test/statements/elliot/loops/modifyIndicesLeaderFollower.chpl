// verify that const checking of loop indices is working correctly for
// leader/follower iters

forall (i, j) in zip(1..10, 1..10) {
  i = 1;
  j = 1;
}
