class MyClass {
  var x:int;
}

record GenericCollection {
  var field;
  proc init(type t) {
    var default:t;
    field = default;
  }
  proc init(field) { // borrows but that might be surprising
    this.field = field;
  }
}

{
  var b:GenericCollection(owned MyClass)
        = new GenericCollection(new owned MyClass());
  writeln("b ", b.type:string, " has field ", b.field.type:string);
}
