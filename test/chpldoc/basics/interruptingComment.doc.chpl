record R {
  @chpldoc.nodoc
  var x /* : int */ = 10;

  proc write() /* : int */ {
    writeln(x);
  }

  proc otherFunc() {
    writeln(x*2);
  }
}
