module module1 {
  config var x: integer = 1;
  function main() {
    writeln("x is: ", x);
  }
}

module module2 {
  config var x: float = 2.2;
  config var z: string = "this one doesn't require it";
}

