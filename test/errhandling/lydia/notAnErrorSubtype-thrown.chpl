class Foo {
  var s: string = "I ain't no error subtype";
}

proc main() throws {
  try {
    throw new owned Foo();
  }
}
