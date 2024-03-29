class Parent {
  var D : domain(1);

  proc init(dom : domain(1)) {
    D = dom;

    init this;
  }
}

class Child : Parent {
  var A : [D] int;

  proc init(dom : domain(1)) {
    super.init(dom);

    init this;
  }
}





var c = new unmanaged Child({1..9});

writeln(c);

c.A = 5;

writeln(c);

c.D = {1..5};

writeln(c);

delete c;
