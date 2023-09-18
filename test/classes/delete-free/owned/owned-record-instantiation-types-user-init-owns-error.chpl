class MyClass {
  var x:int;
}

record GenericCollection {
  var field;
  proc init(type t) {
    var default:t;
    field = default;
  }
  proc init(arg:owned) {
    field = arg;
  }
}

{
  var o = new MyClass();
  var b:GenericCollection(borrowed MyClass) = new GenericCollection(o.borrow());
  writeln("b ", b.type:string, " has field ", b.field.type:string);
}
