class NoFieldError: Error {
}

class DupFieldError: Error {
}

union u {
  var x: int;
  var y: int;
  var z: real;

  proc init(x: int = 0, y: int = 0, z: real = 0.0) throws {
    if x == 0 then
      this.x = x;
    if y == 0 then
      this.y = y;
    if z == 0 then
      this.z = z;

    init this;

    throwingHelper();

    // the compiler should really permit this at the top-level and do
    // this transformation directly if need be (#26438)
    proc throwingHelper() throws {
      if x == 0 && y == 0 && z == 0.0 then
        throw new NoFieldError();

      if (+ reduce [x != 0, y != 0, z != 0.0]) != 1 then
        throw new DupFieldError();
    }
    
  }
}

try {
  var u1: u;
} catch e {
  writeln("u1 got ", e);
}

try {
  var u2 = new u();
} catch e {
  writeln("u2 got ", e);
}

try {
  var u3 = new u(x=1);
} catch e {
  writeln("u3 got ", e);
}

try {
  var u4 = new u(y=2);
} catch e {
  writeln("u4 got ", e);
}


try {
  var u5 = new u(z=5.6);
} catch e {
  writeln("u5 got ", e);
}

try {
  var u6 = new u(x=1, y=2);
} catch e {
  writeln("u6 got ", e);
}

try {
  var u7 = new u(x=1, z=3.4);
} catch e {
  writeln("u7 got ", e);
}

try {
  var u8 = new u(y=2, z=3.4);
} catch e {
  writeln("u8 got ", e);
}

try {
  var u9 = new u(x=1, y=2, z=3.4);
} catch e {
  writeln("u9 got ", e);
}

var u10 = new u(z=1.2, y=3, x=4);
