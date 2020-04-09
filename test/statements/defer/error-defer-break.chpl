proc foo() {

  for i in 1..10 {
    writeln("i is ", i);
    defer {
      writeln("in defer");
      break;
    }
  }
}

foo();
