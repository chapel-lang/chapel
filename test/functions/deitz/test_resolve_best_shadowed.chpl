bar1();
def bar1() {
  def foo(i: int) {
    writeln("outermost foo");
  }
  bar2();
  def bar2() {
    def foo(i: int) {
      writeln("middle foo");
    }
    bar3();
    def bar3() {
      def foo(r: real) {
        writeln("innermost foo, but worst match");
      }
      foo(1);
    }
  }
}
