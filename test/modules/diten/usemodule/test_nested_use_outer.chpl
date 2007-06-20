module outermost {
  module middlemost {
    module innermost {
      def f(a: int) {
        writeln("innermost: a is ", a);
        writeln("innermost: b is ", b);
        writeln("innermost: c is ", c);
      }
    }
    var b = "b";
    def f(a: int) {
      writeln("middlemost: a is ", a);
      writeln("middlemost: b is ", b);
      writeln("middlemost: c is ", c);
    }
  }
  var c = "c";
  def f(a: int) {
    writeln("outermost: a is ", a);
    writeln("outermost: c is ", c);
  }
}

def main() {
  use outermost.middlemost.innermost;
  f(3);
  foo(4);
}

def foo(a: int) {
  use outermost;
  writeln("foo");
  f(a);
}

