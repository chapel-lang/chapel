module OuterModule {
  // This test and check_symbol_accesses2 test that the variable "a"
  // can only be accessed by inner modules or by something that has done a
  // "use outermost;".  check_symbol_accesses2 checks an illegal access.
  module outermost {
    var a = 3;
    module middlemost {
      module innermost {
        proc foo() {
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
