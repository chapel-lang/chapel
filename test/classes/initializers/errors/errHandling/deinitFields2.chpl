record R {
  var x: int;

  proc deinit() {
    writeln("In R(", x, ")'s deinit");
  }
}

class D {
  var x: int;

  proc deinit() {
    writeln("In D(", x, ")'s deinit");
  }
}

class C {
  var err: int;
  var r: R;
  var d = new owned D(1);
  var r2 = new R(2);
  var d2 = new owned D(3);

  proc throwForMe() throws {
    throw new Error();
  }
  
  proc init(err: int) throws {
    this.err = err;
    init this;
    if this.err == 1 then
      throwForMe();
  }
  
  proc postinit() throws {
    if this.err then
      throw new Error();
  }

  proc deinit() {
    writeln("In C's deinit");
  }
}

for mode in 0..2 {
  try {
    var myC = new C(mode);
  } catch {
    writeln("Caught error");
  }
  writeln("Past creation of myC(", mode, ")");
}
