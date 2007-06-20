// This test and check_symbol_accesses test that the variable "a"
// can only be accessed by inner modules or by something that has done a
// "use outermost;".  check_symbol_accesses checks a legal access.
module outermost {
  var a = 3;
  module middlemost {
    module innermost {
      def foo() {
        a = 4;
      }
    }
  }
}

def main() {
  use outermost.middlemost.innermost;
  bar(); foo(); bar();
}

def bar() {
  writeln(a);
}
