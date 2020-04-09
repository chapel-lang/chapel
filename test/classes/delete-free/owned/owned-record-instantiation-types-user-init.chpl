class MyClass {
  var x:int;
}

record GenericCollection {
  var field;
  proc init(type t) {
    var default:t;
    field = default;
  }
  proc init(in field:owned) {
    this.field = field;
  }
  proc init(field:borrowed) {
    this.field = field;
  }
}

{
  var a = new GenericCollection(new owned MyClass());
  writeln("a ", a.type:string, " has field ", a.field.type:string);
}

{
  var b:GenericCollection(owned MyClass)
        = new GenericCollection(new owned MyClass());
  writeln("b ", b.type:string, " has field ", b.field.type:string);
}

{
  var c:GenericCollection(borrowed MyClass)
        = new GenericCollection(new borrowed MyClass());
  writeln("(borrowed) c ", c.type:string, " has field ", c.field.type:string);
}

{
  var a:GenericCollection(owned MyClass?);
  a.field = new owned MyClass();
  writeln("a ", a.type:string, " has field ", a.field.type:string);
}

{
  var b:GenericCollection(owned MyClass?);
  b.field = new owned MyClass();
  writeln("b ", b.type:string, " has field ", b.field.type:string);
}

{
  var c:GenericCollection(borrowed MyClass?);
  var other = new owned MyClass();
  c.field = other;
  writeln("(borrowed) c ", c.type:string, " has field ", c.field.type:string);
}
