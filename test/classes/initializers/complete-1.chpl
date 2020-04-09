class MyClass {
  const x : int = 10;
  const y : int = 20;

  proc init(val : int) {
    super.init();

    y = val;

    complete();

    y = y + 10;
  }
}

proc main() {
  var c : owned MyClass = new owned MyClass(50);

  writeln(c);
}
