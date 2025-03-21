use Python;

var interp = new Interpreter();

// run some code that sets a global
interp.run("""
global myGlobal
myGlobal = 42
""");

// get a global value
var myGlobal = interp.get("myGlobal");
writeln("myGlobal from get: ", myGlobal);

// get a built-in function
var dict = interp.get("dict");
var d = dict(owned PyDict);
d.set("a", 1);
d.set(2, "b");
writeln("dict: ", d);

// use set to change the global
interp.set("myGlobal", "hello world");

// use get to get the global
var myGlobal3 = interp.get(string, "myGlobal");
writeln("myGlobal from get: ", myGlobal3);

// delete myGlobal
interp.del("myGlobal");

try {
  // try and get the global again
  var global = interp.get("myGlobal");
  writeln("global still exists! ", global);
} catch e {
  writeln("myGlobal is gone");
}

