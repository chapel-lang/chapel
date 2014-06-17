proc foo(n : int) : int {
  if (n == 1) {
    return 1;
  } else if (n == 2) {
    return 1;
  } else {
     return foo(n-2) + foo(n-1);
  }
}

writeln(foo(10));
