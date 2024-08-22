{
  writeln("Test 1");
  var A: [1..10] int;
  var B: [1..10] int = 1;

  proc foo() {
    writeln("foo called");
    return 1..3;
  }
  A[foo()] = B[foo()];
  writeln(A);
  writeln();
}

{
  writeln("Test 2");
  var A: [1..10] int;
  const B: [1..10] int = 1;

  proc foo() {
    writeln("foo called");
    return 1..3;
  }
  A[foo()] = B[foo()];
  writeln(A);
  writeln();
}

{
  writeln("Test 3");
  var A: [1..10, 1..10] int;
  var B: [1..10, 1..10] int = 1;

  proc foo() {
    writeln("foo called");
    return 1..3;
  }

  A[foo(), foo()] = B[foo(), foo()];
  writeln(A);
  writeln();
}
