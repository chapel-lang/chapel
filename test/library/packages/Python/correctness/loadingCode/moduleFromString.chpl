
use Python;

var modStr = """
def foo(x):
  print('hello from Python:', x)
""";

var interp = new Interpreter();

var mod = interp.importModule("mymod", modStr);
var foo = mod.get(owned Function, "foo");
foo(1);
foo([1,2,3]);
foo("str");
