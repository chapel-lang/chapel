class Base {
  proc init() {
  }
}

class Fancy : Base {
  proc init() {
  }
}

class Simple : Base {
  proc init() {
  }
}

var f = new Fancy();
var s = new Simple();

delete f;
delete s;
