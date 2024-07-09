class Parent {
  var parentInt : int;

  proc init() {
  }
}

class Child : Parent {
  var foo : int;

  proc init() {
    init this;
    parentInt = 5;
  }
}
