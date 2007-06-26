module outermost {
  use useMe;
  module middlemost {
    module innermost {
      def f() {
        writeln("Here's f!");
        // Should have access to symbols in module useMe
        writeln(a);
        g();
      }
    }
  }
}

module useMe {
  var a = 3 + 4i;
  def g() {
    writeln("Here's g!");
  }
}

def main() {
  use outermost.middlemost.innermost;
  f();
}

