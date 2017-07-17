class C {
  var a, b: int;
}

record R {
// private:
  var c, c2: C;
// public:
  proc init(a:int, b:int) {
    writeln("R");
    c = new C(a, b);
  }
  proc deinit() {
    writeln("~R");
    delete c;
  }
}

config var iters = 2;

proc main {
  for i in 1..#iters {
    var r = new R(1,2);
    writeln(i);
  }
}
