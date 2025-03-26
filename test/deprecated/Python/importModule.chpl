use Python;
var interp = new Interpreter();

var mod = interp.importModule("mymod", "print('hello from Python')");
interp.flush();
writeln("module: ", mod.modName);
