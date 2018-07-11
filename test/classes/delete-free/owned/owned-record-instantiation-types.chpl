class MyClass {
  var x:int;
}

record GenericCollection {
  var field;
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
  var a:GenericCollection(owned MyClass);
  a.field = new owned MyClass();
  writeln("a ", a.type:string, " has field ", a.field.type:string);
}

{
  var b:GenericCollection(owned MyClass);
  b.field = new owned MyClass();
  writeln("b ", b.type:string, " has field ", b.field.type:string);
}

{
  var c:GenericCollection(borrowed MyClass);
  c.field = new owned MyClass();
  writeln("(borrowed) c ", c.type:string, " has field ", c.field.type:string);
}
