use Python, Map;

var interp = new Interpreter();

proc main() {
  // Use a lambda instead of importing a module and a function to avoid memory
  // leaks that aren't our fault.
  var getDictFunc = interp.compileLambda("lambda : dict(first = [1, 2, 3], second = [1, 3, 5, 7], third = ['foo', 'bar'])");

  var d = getDictFunc(owned PyDict);
  // Would print the dictionary, but Python's dictionary printing leaks memory
  // Furthermore, those leaks vary depending on the Python version used
  writeln("d size ", d.size);

  // Would print the list result but that also leaks memory because of Python
  var l = d.get("second");

  var l2 = d.get("third");

  d.del("second");
  writeln("d size after deleting second ", d.size);

  // What happens if I swap 'em?
  d.set("second", l2);
  d.set("third", l);
  writeln("d size after swap ", d.size);

  d.clear();
  writeln("d size after clear ", d.size);
}
