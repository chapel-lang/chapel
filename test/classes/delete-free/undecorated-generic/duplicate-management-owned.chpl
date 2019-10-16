class MyClass { }

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

proc ts(type t) type {
  return (shared t);
}
{
  type e1 = ts(owned MyClass);
  writeln(e1:string);
  type e2 = ts(shared MyClass);
  writeln(e2:string);
  type e3 = ts(unmanaged MyClass); // One day this should be an error
  writeln(e3:string);
  type e4 = ts(borrowed MyClass); // One day this should be an error
  writeln(e4:string);
  type e5 = ts(MyClass);
  writeln(e5:string);
}
