class C {
  var a, b: int;
}

record R {
// private:
  var c, c2: unmanaged C;
// public:
  proc init(a:int, b:int) {
    c = new unmanaged C(a, b);
    c2 = new unmanaged C(a, b);
  }
  proc deinit() {
    delete c;
    delete c2;
  }
}

config var iters = 2;

proc f(r: R) { }

proc main {
  for i in 1..#iters {
    var r = new R(1,2);
    f(r);
    writeln(i);
  }
}
