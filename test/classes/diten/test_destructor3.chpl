class C {
  var a, b: int;
}

record R {
// private:
  var c, c2: C;
// public:
  def R(a:int, b:int) {
    c = new C(a, b);
    c2 = new C(a, b);
  }
  def ~R() {
    delete c;
    delete c2;
  }
}

config var iters = 2;

def f(r: R) { }

def main {
  for i in 1..#iters {
    var r = new R(1,2);
    f(r);
    writeln(i);
  }
}
