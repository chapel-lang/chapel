class Parent {
  var parentInt : int;

  proc init() {
  }
}

class Child : Parent {
  var foo : int;

  proc init() {
    this.initDone();
    parentInt = 5;
  }
}
