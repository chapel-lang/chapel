class MyClass { }

proc factory(type t) {
  return new owned t();
}

var a1 = factory(owned MyClass);
writeln("a1:", a1.type:string);
var a2 = factory(shared MyClass);
writeln("a2:", a2.type:string);
var a3 = factory(unmanaged MyClass);
writeln("a3:", a3.type:string);
var a4 = factory(borrowed MyClass);
writeln("a4:", a4.type:string);
var a5 = factory(MyClass);
writeln("a5:", a5.type:string);

proc tu(type t) type {
  return (unmanaged t);
}
{
  type b1 = tu(owned MyClass);
  writeln("b1:", b1:string);
  type b2 = tu(shared MyClass);
  writeln("b2:", b2:string);
  type b3 = tu(unmanaged MyClass);
  writeln("b3:", b3:string);
  type b4 = tu(borrowed MyClass);
  writeln("b4:", b4:string);
  type b5 = tu(MyClass);
  writeln("b5:", b5:string);
}

proc tb(type t) type {
  return (borrowed t);
}
{
  type c1 = tb(owned MyClass);
  writeln("c1:", c1:string);
  type c2 = tb(shared MyClass);
  writeln("c2:", c2:string);
  type c3 = tb(unmanaged MyClass);
  writeln("c3:", c3:string);
  type c4 = tb(borrowed MyClass);
  writeln("c4:", c4:string);
  type c5 = tb(MyClass);
  writeln("c5:", c5:string);
}

proc to(type t) type {
  return (owned t);
}
{
  type d1 = to(owned MyClass);
  writeln(d1:string);
  type d2 = to(shared MyClass);
  writeln(d2:string);
  type d3 = to(unmanaged MyClass); // One day this should be an error
  writeln(d3:string);
  type d4 = to(borrowed MyClass); // One day this should be an error
  writeln(d4:string);
  type d5 = to(MyClass);
  writeln(d5:string);
}
