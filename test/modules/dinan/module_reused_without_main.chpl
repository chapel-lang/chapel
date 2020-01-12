module OuterModule {
  module test {
    proc getString() return "Hello World";
  }

  use test;

  writeln(getString());
}
