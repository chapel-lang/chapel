use Python;

var interp = new Interpreter();

var code = """
def foo():
  return 42
""";
var code2 = """
def foo():
  return 55
""";

var mod = new Module(interp, "mymod", code);
var mod2 = new Module(interp, "mymod", code2);

writeln(mod.get("foo")()); // 42
writeln(mod2.get("foo")()); // 55
writeln(mod.get("foo")()); // 55, the old module was replaced
