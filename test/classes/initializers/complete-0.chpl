class MyClass {
  const x : int = 10;
  const y : int = 20;

  proc init(val : int) {
    super.init();

    y = val;

    complete();
  }
}

proc main() {
  var c : borrowed MyClass = new borrowed MyClass(50);

  writeln(c);
}
