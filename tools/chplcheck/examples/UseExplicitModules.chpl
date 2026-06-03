/*
  Code should use explicit module declarations rather than
  relying on auto-inserted implicit modules.
*/

// This code relies on an implicit module which violates the rule
writeln("Hello, World!");

// This code uses an explicit module which is correct
module MyModule {
  proc main() {
    writeln("Hello, World!");
  }
}
