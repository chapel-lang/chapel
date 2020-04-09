proc f(a = 1, b = 2, c = 3) throws {
  writeln("in f ", a, b, c);
}


proc test() {
  try {
    f(c=99);
  } catch e {
    writeln("in catch");
  }
}

test();
