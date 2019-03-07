class C {
  var x : int;
}

record R {
  var c : unmanaged C;
  proc init() {
    writeln("default init");
  }
  proc init(x : int) {
    writeln("init: ", x);
    this.c = new unmanaged C(x);
  }
  proc init(other:R) {
    writeln("copy: ", other.c.x, " --> ", other.c.x + 1);
    this.c = new unmanaged C(other.c.x + 1);
  }
  proc deinit() {
    if c != nil {
      writeln("deinit: ", c.x);
      delete c;
    }
  }
}

// LHS of assignment is leaked
proc =(ref A : R, B : R) {
  if A.c != nil then delete A.c;
  if B.c != nil {
    A.c = new unmanaged C(B.c.x + 1);
    writeln("Assignment from R(", B.c, "), resulting in R(", A.c, ")");
  } else {
    A.c = nil;
  }
}

proc main() {
  var s = new R(0);
  {
    var lines : [1..0] R;

    lines.push_back(s);
    lines.pop_front();
  }
}
