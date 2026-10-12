union U {
  var x: int;
  var s: string;
  proc init() {
    writeln("initializing");
    init this;  // default init
    x = 7;      // then assign to 'x'
    writeln("x is ", x);
  }
}

var u:U;
writeln(u.x);
