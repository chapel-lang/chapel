module module1 {
  use module2;
  config var x: string = "that seems to work";
  fun main() {
    writeln("x is: ", x);
  }
}

module module2 {
  config var x: int = 2;
  config var z: string = "this one doesn't require it";
}

