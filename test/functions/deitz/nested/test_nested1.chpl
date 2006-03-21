function foo(i : int) {
  function bar(i : int) {
    writeln(i);
  }
  bar(i);
}

foo(2);
