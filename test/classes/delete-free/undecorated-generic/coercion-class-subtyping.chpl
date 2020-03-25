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

a(borrowed Child);
b(borrowed Child);
a(unmanaged Child);
c(unmanaged Child);
