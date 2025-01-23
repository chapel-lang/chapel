use Python, IO;

var interp = new Interpreter();


var MyClass =
  new Class(interp,
            interp.loadPickle(openReader("MyClass.pkl").readAll(bytes)));
var my_function =
  new Function(interp,
               interp.loadPickle(openReader("my_function.pkl").readAll(bytes)));

// run my_function
var res = my_function(owned ClassObject);
res.call(NoneType, "print");

// create an instance of MyClass
var obj = MyClass(10);
obj.call(NoneType, "print");

