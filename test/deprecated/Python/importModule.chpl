use Python;
var interp = new Interpreter();

var mod = interp.importModule("mymod", "print('hello from Python')");
writeln("module: ", mod.modName);
