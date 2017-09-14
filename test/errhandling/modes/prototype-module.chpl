prototype module mymodule {
  use ThrowError;

  proc propError() {
    throwAnError();
  }

  try {
    propError();
  } catch {
    writeln("in catch");
  }
}
