proc funcThatThrows() throws {
  throw new Error("whee!");
  return 10;
}

class Foo {
  var x = funcThatThrows();
}

proc main() {
  try {
    var a = new Foo(2); // Doesn't throw, no call to func
    writeln(a);
    var b = new Foo(); // Does throw, calls throwing func
    writeln(b);
  } catch e {
    // Won't actually catch, default init isn't marked as throws
    writeln(e.message());
  }
}
