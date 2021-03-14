class Parent { }
class Child : Parent { }

proc a(type t: Parent) {
  writeln(t:string);
}

a(Child);
