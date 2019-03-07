class Foo {
  var s: string = "I ain't no error subtype";
}

proc main() throws {
  try {
    throw new owned Foo();
  } catch e1: Foo {
    writeln("Wait, how did I catch a Foo, that's not an error subtype");
    throw e1;
  }
}
