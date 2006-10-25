module foo {
  var x: int;
  var y: string = "this is a test";

  def main() {
    writeln("x is: ", x);
    writeln("y is: ", y);
  }
}

module bar {
  var x: real;
  var y: string = "this is another";
}
