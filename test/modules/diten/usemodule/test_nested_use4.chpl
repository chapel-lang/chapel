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
  use notoutermost.middlemost.innermost;
}

