class Generic {
  param x = 1;
}

class Parent {
  var x: borrowed Generic(?);
  proc init(x: borrowed Generic(?)) {
    this.x = x;
  }
}

class Child: Parent(?) {
  proc init(x: borrowed Generic(?)) {
    super.init(x);
  }
}

var g = new Generic();
var c = new Child(g);
