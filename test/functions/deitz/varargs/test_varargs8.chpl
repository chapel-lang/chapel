proc foo(ys : string ...?n) {
  for i in 1..n do
    writeln(ys(i));
}

foo("This", "function", "took", "five", "arguments!");
foo("And", "this", "function", "took", "six", "arguments!!");
