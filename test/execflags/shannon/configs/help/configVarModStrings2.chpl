module module1 {
  config var x: string = "that seems to work";
  proc main() {
    writeln("x is: ", x);
    useModule2(); // initialize module2 without bringing it into main's scope
  }
  proc useModule2() {
    use module2;
  }
}

module module2 {
  config var x: string = "the end";
  config var z: string = "this one doesn't require the module name";
}

