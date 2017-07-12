class Bar {
  var A, B : int;
  proc init(first, second=1) {
    A = first;
    B = second;
  }
}

class Foo {
  var B = new Bar(42, second=10);

  proc init() {
  }
}
