//
// Create a record with a `writeThis` that always throws a user error and
// use that to verify correct behavior for `writeln` (it should fail with
// a general IoError).
//
use IO;

record foo : writeSerializable {
  var x: int = 0;

  proc serialize(writer:fileWriter(?), ref serializer) throws {
    throw new
      IllegalArgumentError('User error thrown from writeThis!');
    writer.write(x);
  }
}

proc main() {
  var x = new foo();
  try! stdout.write(x);
}

