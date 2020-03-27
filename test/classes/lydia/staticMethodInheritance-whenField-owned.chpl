// Tracks calling a static method defined on a parent type from a child type
class Parent {
  proc type foo() param {
    return 3;
  }
}

class Child: Parent {
}

record Wrapper {
  var x;
}

{
  var p = new owned Parent();
  var c = new owned Child();
  var w1 = new Wrapper(p);
  var w2 = new Wrapper(c);

  writeln(w1.x.type: string);
  writeln(w2.x.type: string);
  writeln(w1.x.type.foo());
  writeln(w2.x.type.foo());
}
