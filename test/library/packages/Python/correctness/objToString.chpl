use Python, List, Set, Map;
var interp = new Interpreter();

var mod = interp.createModule("""
class my_class:
  def __init__(self, *args):
    self.args = args

  def __str__(self):
    return ', '.join(map(str, self.args))

  def __repr__(self):
    return f"my_class({', '.join(map(repr, self.args))})"
""");

proc test(obj: borrowed) {
  writeln("str: ", obj.str());
  writeln("repr: ", obj.repr());
  writeln("cast to string: ", obj: string);
  writeln("default: ", obj);
  writeln("================");
}

var my_class = mod.get('my_class');

test(new Value(interp, "hello"));
test(new Value(interp, interp.toPython(1)));
test(new Value(interp, [1, 2, 3]));

test(my_class(1, 2, 3));
test(my_class("hello", "world"));
test(my_class(1, "hello", 2, "world"));

var m = new map(string, int);
m["hello"] = 1;
m["world"] = 2;
test(my_class(new list([1, 2, 3]), new set(string, ["hello", "world"]), m));

