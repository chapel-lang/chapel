
use OwnedObject;

class Foo {
  proc foobar() throws {
    throw new Error();
  }
}

proc main() {
  var f = new Owned(new Foo());
  try {
    f.foobar();
  } catch e : Error {
    writeln("SUCCESS");
  }
}
