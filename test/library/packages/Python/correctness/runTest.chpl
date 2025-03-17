use Python;

var interp = new Interpreter();

// run some code and set a global
interp.run("""
print("the message is:", msg)
global theMsg
theMsg = msg
""", ["msg" => "Hello, World!"]);

// declare a function
interp.run("""
def add(a, b):
  return a + b
""");

// get the function
var add: owned Value? = interp.get("add");

// pass the function as a FCP and call it
interp.run("""
print("the message is:", theMsg)
print("adding 1 and 2:", add(1, 2))
print("using the FCP to add 1 and 2:", myAdd(1, 2))
""", ["myAdd" => add:borrowed]);


