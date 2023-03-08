module OuterModule {
  module test {
    proc getString() do return "Hello World";
  }

  use test;

  writeln(getString());
}
