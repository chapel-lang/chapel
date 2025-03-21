use Python, IO;

var interp = new Interpreter();

// must manually hold the GIL, since `loadPickle` is an internal function
var g = new GIL();
var MyClass =
  new Class(interp,
            interp.loadPickle(openReader("MyClass.pkl").readAll(bytes)));
var my_function =
  new Function(interp,
               interp.loadPickle(openReader("my_function.pkl").readAll(bytes)));
g.release();

// run my_function
var res = my_function();
res.call("print");

// create an instance of MyClass
var obj = MyClass(10);
obj.call("print");

