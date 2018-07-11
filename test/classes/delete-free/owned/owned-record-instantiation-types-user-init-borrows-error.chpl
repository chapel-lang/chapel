class MyClass {
  var x:int;
}

record GenericCollection {
  var field;
  proc init(type t) {
    var default:t;
    field = default;
  }
  proc init(arg) { // borrows but that might be surprising
    field = arg;
  }
}

{
  var b:GenericCollection(owned MyClass)
        = new GenericCollection(new owned MyClass());
  writeln("b ", b.type:string, " has field ", b.field.type:string);
}
