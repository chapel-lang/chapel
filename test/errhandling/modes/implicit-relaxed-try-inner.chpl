
module mymodule {
  use ExampleErrors;

  proc propError() throws {
    throwAnError();
  }

  proc test() {
    writeln("in test");
    try {
      propError();
    } catch {
      writeln("OK");
    }
  }
}

use mymodule;
writeln("In outer implicit module");
test();
