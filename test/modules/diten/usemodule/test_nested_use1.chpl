module outermost {
  module middlemost {
    module innermost {
      def f() {
        writeln("hello from innermost");
      }
    }
  }
}

def main() {
  use outermost.middlemost.innermost;
}

