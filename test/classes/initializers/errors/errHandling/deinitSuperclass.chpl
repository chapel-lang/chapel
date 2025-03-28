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
  var d2 = new shared D(3);

  proc throwForMe() throws {
    throw new Error();
  }
  
  proc init(err: int) {
    this.err = err;
    init this;
  }
  
  proc postinit() throws {
    writeln("In C's postinit");
    if this.err == 3 then
      throw new Error();
  }

  proc deinit() {
    writeln("In C's deinit");
  }
}

class E : C {
  var r3 = new R(4);
  var d3 = new owned D(5);

  proc init(err: int) throws {
    super.init(err);
    init this;
    if this.err == 1 then {
      throwForMe();
    }
  }

  proc postinit() throws {
    writeln("In E's postinit");
    if this.err == 2 then {
      throw new Error();
    }
  }

  proc deinit() {
    writeln("In E's deinit");
  }
}

for mode in 0..3 {
  writeln("--- mode ", mode, " ---");
  try {
    var myE = new E(mode);
  } catch {
    writeln("Caught error");
  }
  writeln("Past creation of myE(", mode, ")");
}
