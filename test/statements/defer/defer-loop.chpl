proc foo() {
  defer {
    for i in 1..4 {
      writeln(i);
      if i == 3 then break;
    }
  }
}

foo();
