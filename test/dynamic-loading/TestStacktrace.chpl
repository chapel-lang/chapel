use DynamicLibrary;

config const path = "./lib/";

proc main() {
  var lib = load("TestStacktraceLib", directory=path);
  var callMe = lib.retrieve("callMe", proc(): void);
  callMe();
}
