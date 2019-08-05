// This used to print out Child(int, int) and then Child(int, real)
// It should only print out Parent(int)

class Parent {
  type t;
  var x: t;

  proc parentMethod() {
    writeln("In parentMethod(", t:string, ") ", this.type:string);
  }
}

class Child : Parent {
  var y: t;

  type tt;
  var z: tt;
}

proc main() {
  var ci = new Child(int, 1, 2, int, 3);
  var cr = new Child(int, 1, 2, real, 3);

  ci.parentMethod();
  cr.parentMethod();
}
