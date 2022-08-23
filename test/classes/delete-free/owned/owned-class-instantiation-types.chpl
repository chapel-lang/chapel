class MyClass {
  var x:int;
}

class GenericCollection {
  var field;
}

{
  var a = (new owned GenericCollection(new owned MyClass())).borrow();
  writeln("a ", a.type:string, " has field ", a.field.type:string);
}

{
  var b:borrowed GenericCollection(owned MyClass)
        = (new owned GenericCollection(new owned MyClass())).borrow();
  writeln("b ", b.type:string, " has field ", b.field.type:string);
}

{
  var c:borrowed GenericCollection(borrowed MyClass)
        = (new owned GenericCollection((new owned MyClass()).borrow())).borrow();
  writeln("(borrowed) c ", c.type:string, " has field ", c.field.type:string);
}

{
  var empty:owned MyClass?;
  var a:borrowed GenericCollection(owned MyClass?)? =
    (new owned GenericCollection(empty)).borrow();
  a!.field = new owned MyClass();
  writeln("a ", a.type:string, " has field ", a!.field.type:string);
}

{
  var empty:owned MyClass?;
  var b:borrowed GenericCollection(owned MyClass?)? =
    (new owned GenericCollection(empty)).borrow();
  b!.field = new owned MyClass();
  writeln("b ", b.type:string, " has field ", b!.field.type:string);
}

{
  var empty:borrowed MyClass?;
  var c:borrowed GenericCollection(borrowed MyClass?)? =
    (new owned GenericCollection(empty)).borrow();
  var other = new owned MyClass();
  c!.field = other;
  writeln("(borrowed) c ", c.type:string, " has field ", c!.field.type:string);
}
