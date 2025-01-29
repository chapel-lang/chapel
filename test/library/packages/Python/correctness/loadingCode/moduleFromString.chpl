
use Python;

var modStr = """
def foo(x):
  print('hello from Python:', x)
""";

var interp = new Interpreter();

var mod = new Module(interp, "mymod", modStr);
var foo = mod.getAttr(owned Function, "foo");
foo(NoneType, 1);
foo(NoneType, [1,2,3]);
foo(NoneType, "str");
