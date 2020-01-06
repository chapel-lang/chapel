//
// Create a record with a `writeThis` that always throws a user error and
// use that to verify correct behavior for `writeln` (it should fail with
// a general IOError).
//
use IO;

record foo {
  var x: int = 0;

  proc writeThis(ch: channel) throws {
    throw new
      IllegalArgumentError('User error thrown from writeThis!');
    ch <~> x;
  }
}

proc main() {
  var x = new foo();
  writeln(x);
}

