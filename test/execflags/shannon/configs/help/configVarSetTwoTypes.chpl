module module1 {
  use module2;
  config var x: int = 1;
  fun main() {
    writeln("x is: ", x);
  }
}

module module2 {
  config var x: float = 2.2;
  config var z: string = "this one doesn't require it";
}

