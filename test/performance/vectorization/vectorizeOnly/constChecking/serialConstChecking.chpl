// ensure that const checking of index variables isn't broken by 'foreach' for
// serial loops (that the ref versions isn't mistakenly called)

var A: [1..10] int;

// serial val iter
for i in 1..10 do i = 1;                  // illegal
foreach i in 1..10 do i = 1;         // illegal

// serial ref iter
for a in A do a = 1;                      // legal
foreach a in A do a = 1;             // legal

// serial zipped ref then val iter
for (a, i) in zip(A, 1..10) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

foreach (a, i) in zip(A, 1..10) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

foreach (a, i) in zip(A, 1..10) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

// serial zipped val then ref iter
for (i, a) in zip(1..10, A) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

foreach (i, a) in zip(1..10, A) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

foreach (i, a) in zip(1..10, A) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}
