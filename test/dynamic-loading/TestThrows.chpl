use DynamicLibrary;

config const path = "./lib/";

proc main() {
  var lib = load("TestThrowsLib", directory=path);
  var callThrows = lib.retrieve("callMe", proc(): void);
  callThrows();
}
