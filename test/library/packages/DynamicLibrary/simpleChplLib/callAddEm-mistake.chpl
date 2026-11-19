use DynamicLibrary, ChplConfig;

const lib = try! load("addEm", directory="./lib");

try {
  // NOTE: The name "addem" is intentionally incorrect!
  const myAddEm = lib.retrieve("addem", proc(x: int, y: int): int);
  writeln(myAddEm(42, 45));
} catch e : DynLoadError {
  assert(e.message().find("addem") != -1);
  writeln('Caught error!');
} catch e {
  writeln(e);
}

// Explicit 'exit' call is no longer needed - teardown happens properly.
// exit(0);
