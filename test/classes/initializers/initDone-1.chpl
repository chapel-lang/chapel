class MyClass {
  const x : int = 10;
  const y : int = 20;

  proc init(val : int) {
    super.init();

    y = val;

    initDone();

    y = y + 10;
  }
}

proc main() {
  var c : MyClass = new MyClass(50);

  writeln(c);

  delete c;
}
