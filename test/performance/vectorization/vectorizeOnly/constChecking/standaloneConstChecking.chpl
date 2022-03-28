// ensure that const checking of index variables isn't broken by the standalone
// 'foreach' iterator (check that the ref versions isn't mistakenly called)

var A: [1..10] int;

// standalone val iter
forall i in 1..10 do i = 1;                  // illegal
foreach i in 1..10 do i = 1;         // illegal

// standalone ref iter
forall a in A do a = 1;                      // legal
foreach a in A do a = 1;             // legal

// parallel zipped ref then val iter
forall (a, i) in zip(A, 1..10) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

foreach (a, i) in zip(A, 1..10) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

// parallel zipped val then ref iter
forall (i, a) in zip(1..10, A) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

foreach (i, a) in zip(1..10, A) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}
