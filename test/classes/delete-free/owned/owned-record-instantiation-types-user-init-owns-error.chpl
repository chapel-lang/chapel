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
  var b:GenericCollection(borrowed MyClass)
        = new GenericCollection(new borrowed MyClass());
  writeln("b ", b.type:string, " has field ", b.field.type:string);
}
