// Test casting from a PyArray to a Chapel array
use Python;

proc main() {
  var interp = new Interpreter();
  var mod = interp.importModule("usePythonArray");
  var doit = mod.get('doit');


  var pyRes = doit(owned PyArray(int), 10, 11, 12, 13);
  var chapelArr = pyRes: [0..3] int;
  writeln(chapelArr);
  // Check casting to an array with a different starting index (but still the
  // same size)
  var chapelArr2 = pyRes: [1..4] int;
  writeln(chapelArr2);
}
