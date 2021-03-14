// ensure that const checking of index variables isn't broken by vectorizeOnly for
// serial loops (that the ref versions isn't mistakenly called)

var A: [1..10] int;

// serial val iter
for i in 1..10 do i = 1;                  // illegal
for i in vectorizeOnly(1..10) do i = 1;         // illegal

// serial ref iter
for a in A do a = 1;                      // legal
for a in vectorizeOnly(A) do a = 1;             // legal

// serial zipped ref then val iter
for (a, i) in zip(A, 1..10) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

for (a, i) in vectorizeOnly(A, 1..10) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

for (a, i) in zip(vectorizeOnly(A, 1..10)) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

// serial zipped val then ref iter
for (i, a) in zip(1..10, A) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

for (i, a) in vectorizeOnly(1..10, A) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}

for (i, a) in zip(vectorizeOnly(1..10, A)) {
  a = 1;                                  // legal
  i = 1;                                  // illegal
}
