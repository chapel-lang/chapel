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
  var ownC = new owned MyClass(50);
  var c : borrowed MyClass = ownC.borrow();

  writeln(c);
}
