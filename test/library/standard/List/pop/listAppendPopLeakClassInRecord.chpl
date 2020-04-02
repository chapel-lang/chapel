private use List;

class C {
  var x : int;
}

record R {
  var c : unmanaged C?;
  proc init() {
    writeln("default init");
  }
  proc init(x : int) {
    writeln("init: ", x);
    this.c = new unmanaged C(x);
  }
  proc init=(other:R) {
    writeln("copy: ", other.c!.x, " --> ", other.c!.x + 1);
    this.c = new unmanaged C(other.c!.x + 1);
  }
  proc deinit() {
    if c != nil {
      writeln("deinit: ", c!.x);
      delete c;
    }
  }
}

//
// List currently uses a primitive move instead of an assignment internally,
// so this routine is unused.
//
proc =(ref A : R, B : R) {
  if A.c != nil then delete A.c;
  if B.c != nil {
    A.c = new unmanaged C(B.c!.x + 1);
    writeln("Assignment from R(", B.c, "), resulting in R(", A.c, ")");
  } else {
    A.c = nil;
  }
}

proc main() {
  // First and only init is here.
  var r = new R(0);

  // Start test in new scope to avoid firing destructor of r.
  {
    var lines: list(R);

    lines.append(r);

    const elem = lines.pop(0);
  }
}
