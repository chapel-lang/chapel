module test {
  class MyClass { var x: int; }

  proc main() {
    var x = new owned MyClass();

    var nilable = x:borrowed MyClass?;

    var y = try! nilable:MyClass;
    writeln(y);

    var z = try! nilable:borrowed MyClass;
    writeln(z);
  }
}
