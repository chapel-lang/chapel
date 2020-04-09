proc foo(x) where x.type == int {
  begin {
    writeln(x);
  }
}

foo(2);
