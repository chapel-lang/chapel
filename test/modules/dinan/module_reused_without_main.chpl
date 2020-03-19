module OuterModule {
  module test {
    proc getString() return "Hello World";
  }

  use this.test;

  writeln(getString());
}
