
config const pycPath: string;

use Python;

var interp = new Interpreter();
var modBytes = interp.loadPycFile(pycPath);

var mod = interp.importModule("mod", modBytes);
// get MyClass and my_function
var MyClass = mod.get("MyClass");
var my_function = mod.get("my_function");

// run my_function
var v = my_function();
v.call("print");

// create an instance of MyClass
var obj = MyClass(10);
obj.call("print");



