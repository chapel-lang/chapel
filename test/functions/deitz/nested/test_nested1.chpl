proc foo(i : int) {
  proc bar(i : int) {
    writeln(i);
  }
  bar(i);
}

foo(2);
