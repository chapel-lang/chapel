class MyClass { var x:int; }

record Collection {
  type eltType;
  var element: eltType;
  proc init(type eltType: owned) {
    this.eltType = eltType;
  }
  proc init(in arg: owned) {
    this.eltType = arg.type;
    this.element = arg;
  }
}

proc ref Collection.addElement(in arg: owned) {
  element = arg;
}

proc test() {
  var c: Collection(owned MyClass?);
  c.addElement(new owned MyClass()); // transferred to element
}

test();
