use ThrowError;

proc doesNotThrow() {
  try {
    throwAnError();
  } catch e: OtherError {
    writeln("should not run");
  }
}

doesNotThrow();
