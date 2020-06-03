class Parent { type t; }
class Child : Parent { }

proc f(type t: borrowed Parent) {
  writeln(t:string);
}

f(unmanaged Child(int));
