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
  try {
    stdout.write(x);
  } catch err: SystemError {
    writeln("SystemError");
  } catch err: IllegalArgumentError {
    writeln("IllegalArgumentError"); 
  } catch err {}

  //
  // You have to use `clearError` here to prevent a crash when destroying
  // the internal channel.
  //
  stdout.clearError();  
}

