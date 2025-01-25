
config const pycPath: string;

use Python;

var interp = new Interpreter();
var modBytes = interp.loadPycFile(pycPath);

var mod = new Module(interp, "mod", modBytes);
// get MyClass and my_function
var MyClass = new Class(mod, "MyClass");
var my_function = new Function(mod, "my_function");

// run my_function
var v = my_function(owned Value);
v.call(NoneType, "print");

// create an instance of MyClass
var obj = MyClass(10);
obj.call(NoneType, "print");



