module module1 {
  config var x = 1;
  def main() {
    writeln("x is: ", x);
    useModule2(); // initialize module2 without bringing it into main's scope
  }
  def useModule2() {
    use module2;
  }
}

module module2 {
  config var x = 2;
  config var z: real = 2.2;
}

