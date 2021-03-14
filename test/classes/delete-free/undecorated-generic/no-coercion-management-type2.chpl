class Parent { }
class Child : Parent { }

proc f(type t: borrowed Child) {
  writeln(t:string);
}

f(unmanaged Child);
