class MyClass {
  var x:int;
}

class GenericCollection {
  var field;
  proc init(type t) {
    var default:t;
    field = default;
  }
  proc init(in arg:owned) {
    field = arg;
  }
  proc init(arg:borrowed) {
    field = arg;
  }
}

{
  var a = new borrowed GenericCollection(new owned MyClass());
  writeln("a ", a.type:string, " has field ", a.field.type:string);
}

{
  var b:borrowed GenericCollection(owned MyClass)
        = new borrowed GenericCollection(new owned MyClass());
  writeln("b ", b.type:string, " has field ", b.field.type:string);
}

{
  var c:borrowed GenericCollection(borrowed MyClass)
        = new borrowed GenericCollection(new borrowed MyClass());
  writeln("(borrowed) c ", c.type:string, " has field ", c.field.type:string);
}

{
  var empty:owned MyClass?;
  var a:borrowed GenericCollection(owned MyClass?)? =
    new borrowed GenericCollection(empty);
  a!.field = new owned MyClass();
  writeln("a ", a.type:string, " has field ", a!.field.type:string);
}

{
  var empty:owned MyClass?;
  var b:borrowed GenericCollection(owned MyClass?)? =
    new borrowed GenericCollection(empty);
  b!.field = new owned MyClass();
  writeln("b ", b.type:string, " has field ", b!.field.type:string);
}

{
  var empty:borrowed MyClass?;
  var c:borrowed GenericCollection(borrowed MyClass?)? =
    new borrowed GenericCollection(empty);
  var other = new owned MyClass();
  c!.field = other;
  writeln("(borrowed) c ", c.type:string, " has field ", c!.field.type:string);
}
