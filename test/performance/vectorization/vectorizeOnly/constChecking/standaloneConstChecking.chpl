// ensure that const checking of index variables isn't broken by the standalone
// vectorizeOnly iterator (check that the ref versions isn't mistakenly called)

var A: [1..10] int;

// standalone val iter
forall i in 1..10 do i = 1;                  // illegal
forall i in vectorizeOnly(1..10) do i = 1;         // illegal

// standalone ref iter
forall a in A do a = 1;                      // legal
forall a in vectorizeOnly(A) do a = 1;             // legal

// parallel zipped ref then val iter (standalone vectorizeOnly though)
forall (a, i) in zip(A, 1..10) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

forall (a, i) in vectorizeOnly(A, 1..10) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

// parallel zipped val then ref iter (standalone vectorizeOnly though)
forall (i, a) in zip(1..10, A) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

forall (i, a) in vectorizeOnly(1..10, A) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}
