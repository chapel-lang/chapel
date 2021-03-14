class Bar {
  var A, B : int;
  proc init(first, second=1) {
    A = first;
    B = second;
  }
}

class Foo {
  var B = new unmanaged Bar(42, second=10);

  proc init() {
  }
}
