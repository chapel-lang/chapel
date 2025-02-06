
use Python;

var modStr = """
def foo(x):
  print('hello from Python:', x)
""";

var interp = new Interpreter();

var mod = new Module(interp, "mymod", modStr);
var foo = mod.getAttr(owned Function, "foo");
foo(1);
foo([1,2,3]);
foo("str");
