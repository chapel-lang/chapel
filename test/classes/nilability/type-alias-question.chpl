module test {
  class MyClass { var x: int; }

  proc main() {
    type t = MyClass?;
    writeln(t:string);

    type u = MyClass;
    writeln((u?):string);
  }
}
