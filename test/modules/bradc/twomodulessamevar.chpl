module foo {
  var x: int;
  var y: string = "this is a test";

  fun main() {
    writeln("x is: ", x);
    writeln("y is: ", y);
  }
}

module bar {
  var x: float;
  var y: string = "this is another";
}
