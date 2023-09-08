// modified from https://github.com/chapel-lang/chapel/issues/22574
class MyClass { var field: int; }
operator MyClass.: (x: MyClass, type t: int) {
  writeln("In cast to int, x.type is ", x.type:string);
  return x.field;
}
operator MyClass.: (x: MyClass, type t: unmanaged) {
  writeln("In cast to unmanaged, x.type is ", x.type:string);
  return x.field;
}
proc main() {
  {
    var x = new MyClass(1);
    writeln(x : int);
    writeln(x : unmanaged);
    writeln(x : unmanaged class?);
  }
  {
    var x = new shared MyClass(2);
    writeln(x : int);
    writeln(x : unmanaged class);
    writeln(x : unmanaged class?);
  }
}
