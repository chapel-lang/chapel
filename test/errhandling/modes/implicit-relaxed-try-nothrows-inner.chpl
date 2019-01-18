
module mymodule {
  use ExampleErrors;

  proc propError() {
    throwAnError();
  }

  proc test() {
    writeln("in test");

    try {
      propError();
    } catch {
      writeln("in catch");
    }
  }
}

use mymodule;
writeln("In outer implicit module");
test();
