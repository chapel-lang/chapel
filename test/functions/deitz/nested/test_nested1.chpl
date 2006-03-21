fun foo(i : int) {
  fun bar(i : int) {
    writeln(i);
  }
  bar(i);
}

foo(2);
