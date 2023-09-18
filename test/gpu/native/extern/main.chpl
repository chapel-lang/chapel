module extern_c
{
  require "extern_c_test.c", "extern_c_test.h";
  extern proc hello_world(): void;
}

module main
{
  use extern_c;

  proc main(): int
  {
    writeln("hello world from chpl");
    hello_world();

    return 0;
  }
}
