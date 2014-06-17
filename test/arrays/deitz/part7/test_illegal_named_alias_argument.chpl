proc foo(A: [1..3] int) {
  writeln(A);
}

var B: [2..4] int;

foo(A=>B);
