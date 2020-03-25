class Parent { }
class Child : Parent { }

proc a(type t: Parent) {
  writeln("a ", t:string);
}
proc b(type t: borrowed Parent) {
  writeln("b ", t:string);
}
proc c(type t: unmanaged Parent) {
  writeln("c ", t:string);
}
proc d(type t: owned Parent) {
  writeln("d ", t:string);
}
proc e(type t: shared Parent) {
  writeln("e ", t:string);
}

a(owned Child);
d(owned Child);
a(shared Child);
e(shared Child);
