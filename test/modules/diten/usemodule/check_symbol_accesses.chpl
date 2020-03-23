module OuterModule {
  // This test and check_symbol_accesses* test that the variable "a"
  // can only be accessed by things that "use outermost;".
  // check_symbol_accesses2 and 3 check an illegal access.
  module outermost {
    var a = 3;
    module middlemost {
      module innermost {
        proc foo() {
          import OuterModule.outermost.a;
          a = 4;
        }
      }
    }
  }

  proc main() {
    use outermost.middlemost.innermost;
    bar(); foo(); bar();
  }

  proc bar() {
    use outermost;
    writeln(a);
  }
}
