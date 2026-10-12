union U {
  var x: int;
  var s: string;
  proc init() {
    writeln("initializing");
    x = 7;      // initialize 'x'
    init this;  // then assert the union is initialized
    writeln("x is ", x);
  }
}

var u:U;
writeln(u.x);
