class MyClass {
  var x:int;
}

class GenericCollection {
  var field;
}

{
  var ownA = new owned GenericCollection(new owned MyClass());
  var a = ownA.borrow();
  writeln("a ", a.type:string, " has field ", a.field.type:string);
}

{
  var ownB = new owned GenericCollection(new owned MyClass());
  var b:borrowed GenericCollection(owned MyClass) = ownB.borrow();
  writeln("b ", b.type:string, " has field ", b.field.type:string);
}

{
  var ownInner = new owned MyClass();
  var ownC = new owned GenericCollection(ownInner.borrow());
  var c:borrowed GenericCollection(borrowed MyClass) = ownC.borrow();
  writeln("(borrowed) c ", c.type:string, " has field ", c.field.type:string);
}

{
  var empty:owned MyClass?;
  var ownA = new owned GenericCollection(empty);
  var a:borrowed GenericCollection(owned MyClass?)? = ownA.borrow();
  a!.field = new owned MyClass();
  writeln("a ", a.type:string, " has field ", a!.field.type:string);
}

{
  var empty:owned MyClass?;
  var ownB = new owned GenericCollection(empty);
  var b:borrowed GenericCollection(owned MyClass?)? = ownB.borrow();
  b!.field = new owned MyClass();
  writeln("b ", b.type:string, " has field ", b!.field.type:string);
}

{
  var empty:borrowed MyClass?;
  var ownC = new owned GenericCollection(empty);
  var c:borrowed GenericCollection(borrowed MyClass?)? = ownC.borrow();
  var other = new owned MyClass();
  c!.field = other;
  writeln("(borrowed) c ", c.type:string, " has field ", c!.field.type:string);
}
