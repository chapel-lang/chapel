class Parent {
  proc accepts_type(type t) {
    writeln("Parent.accepts_type");
  }
  proc returns_type() type {
    writeln("Parent.returns_type");
    return int;
  }
  proc accepts_param(param t) {
    writeln("Parent.accepts_param");
  }
  proc returns_param() param {
    writeln("Parent.returns_param");
    return 1;
  }

  proc returns_rtt() type {
    var A:[1..10] int;
    writeln("Parent.returns_rtt");
    return A.type;
  }
}

class Child : Parent {
  override proc accepts_type(type t) {
    writeln("Child.accepts_type");
  }
  override proc returns_type() type {
    writeln("Child.returns_type");
    return int;
  }
  override proc accepts_param(param t) {
    writeln("Child.accepts_param");
  }
  override proc returns_param() param {
    writeln("Child.returns_param");
    return 1;
  }
  override proc returns_rtt() type {
    var A:[1..10] int;
    writeln("Child.returns_rtt");
    return A.type;
  }
}

proc main() {
  var a = new borrowed Parent();
  var b = new borrowed Child();
  var c:borrowed Parent = new borrowed Child();
  a.accepts_type(int);
  b.accepts_type(int);
  c.accepts_type(int);
  a.returns_type();
  b.returns_type();
  c.returns_type();
  a.accepts_param(1);
  b.accepts_param(1);
  c.accepts_param(1);
  a.returns_param();
  b.returns_param();
  c.returns_param();
  a.returns_rtt();
  b.returns_rtt();
  c.returns_rtt();
}
