proc test(print_me: string) {
  proc int.print_me {
    writeln(this);
  }

  writeln(print_me);
}

test("foo");
