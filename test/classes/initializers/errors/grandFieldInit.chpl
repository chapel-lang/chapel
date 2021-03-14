
class GrandParent {
  var x : int;
}

class Parent : GrandParent {
  var y : int;
}

class Child : Parent {
  var z : real;

  proc init() {
    this.x = 5;
    super.init(1, 2);
  }
}

var c = new owned Child();
