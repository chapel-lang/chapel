
use JSON;
use IO;

union U {
  var x : int;
  var y : real;
  var z : string;
}

// Unions are unstable, so this is just to lock in current behavior
proc main() {
  var u : U;
  u.z = "test";

  writeln(u);

  var w = stdout.withSerializer(new jsonSerializer());
  w.writeln(u);
}
