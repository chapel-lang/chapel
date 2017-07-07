class Parent {
  var parentInt : int;

  proc init() {
  }
}

class Child : Parent {
  var foo : int;

  proc init() {
    super.init();
    parentInt = 5;
  }
}
