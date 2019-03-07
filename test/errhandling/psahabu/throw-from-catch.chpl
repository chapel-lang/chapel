use ExampleErrors;

proc rethrow() throws {
  try {
    throwAnError();
  } catch err {
    writeln("rethrowing error");
    throw err;
  }
}

proc throwDifferent() throws {
  try {
    throwAnError();
  } catch {
    writeln("throwing different error");
    throw new owned Error();
  }
}

try {
  rethrow();
} catch {
  writeln("success: rethrew error");
}

try {
  throwDifferent();
} catch {
  writeln("success: threw different error");
}
