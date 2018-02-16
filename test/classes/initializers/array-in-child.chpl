class Parent {
  var D : domain(1);

  proc init(dom : domain(1)) {
    D = dom;

    initDone();
  }
}

class Child : Parent {
  var A : [D] int;

  proc init(dom : domain(1)) {
    super.init(dom);

    initDone();
  }
}





var c = new Child({1..9});

writeln(c);

c.A = 5;

writeln(c);

c.D = {1..5};

writeln(c);

delete c;