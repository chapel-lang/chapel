class C {
  var a, b: int;
}

record R {
// private:
  var c, c2: C;
// public:
  proc R(a:int, b:int) {
    c = new C(a, b);
    c2 = new C(a, b);
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
