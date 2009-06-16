class C {
  var a, b: int;
}

record R {
// private:
  var c: C;
// public:
  def R(a:int, b:int) {
    writeln("R");
    c = new C(a, b);
  }
  def ~R() {
    writeln("~R");
    delete c;
  }
}

config var iters = 2;

def main {
  for i in 1..#iters {
    var r = new R(1,2);
    writeln(i);
  }
}
