fun foo(i : int) {
  writeln("foo of ", i);
  fun bar(i : int) {
    writeln("bar of ", i);
    car(i);
  }
  fun car(i : int) {
    writeln("car of ", i);
  }
  bar(i);
}

foo(5);
