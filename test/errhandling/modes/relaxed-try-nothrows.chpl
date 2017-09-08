pragma "error mode relaxed"
module mymodule {
  use ThrowError;

  proc propError() {
    throwAnError();
  }

  writeln("should not compile");

  try {
    propError();
  } catch {
    writeln("in catch");
  }
}
