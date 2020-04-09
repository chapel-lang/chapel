
proc foo(x : int) param {
  for i in 1..x do assert(i <= x);
  return 0;
}

foo(10);
