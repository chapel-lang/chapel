use IO;

record Foo {
  var x: int;
}

operator Foo.<~>(const ref ch: channel, const x: Foo) const ref throws
  where ch.writing {

  try ch.readwrite(x.x);
  return ch;
}
proc main() {
  var foo = new Foo(3);
  stdout <~> foo <~> new ioNewline();
}
