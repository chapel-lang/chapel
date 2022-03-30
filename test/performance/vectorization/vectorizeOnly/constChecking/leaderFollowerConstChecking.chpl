// ensure that const checking of index variables isn't broken by 'foreach' for
// leader/follower loops (that the ref versions isn't mistakenly called)

var A: [1..10] int;

// l/f val iter
forall i in zip(1..10) do i = 1;                  // illegal
foreach i in zip(1..10) do i = 1;         // illegal

// l/f serial ref iter
forall a in zip(A) do a = 1;                      // legal
foreach a in zip(A) do a = 1;             // legal

// l/f zipped ref then val iter
forall (a, i) in zip(A, 1..10) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

foreach (a, i) in zip(A, 1..10) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

// l/f zipped val then ref iter
forall (i, a) in zip(1..10, A) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

foreach (i, a) in zip(1..10, A) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}
