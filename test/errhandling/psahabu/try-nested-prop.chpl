use ExampleErrors;

pragma "use default init"
class SpecificError : Error { }

proc nestedTries() throws {
  try {
    writeln("outer try");
    try {
      writeln("inner try");
      throw new OtherError();
    } catch err: SpecificError {
      writeln("fail: error should not be caught here");
    }
    writeln("fail: inner try did not throw");
  } catch err: SpecificError {
    writeln("fail: error should not be caught here");
  }
}

try {
  writeln("calling nestedTries");
  nestedTries();
} catch {
  writeln("successfully propagated");
}
