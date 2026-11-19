use DynamicLibrary, ChplConfig;

const lib = try! load("addEm", directory="./lib");

const myAddEm = lib.retrieve("addEm", proc(x: int, y: int): int);

writeln(myAddEm(42, 45));
