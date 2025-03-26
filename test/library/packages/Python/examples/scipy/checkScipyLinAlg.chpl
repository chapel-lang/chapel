use Python;

proc main() {
  var interp = new Interpreter();

  // imports from SciPy
  var scipy = interp.importModule("scipy");
  var linalg = interp.importModule("scipy.linalg");
  var normFunc = linalg.get("norm");

  var a = [1, 3, 5, 7];

  var res = normFunc(real, a);
  writeln(res);

  var eigvalsFunc = linalg.get("eigvals");

  var b = [[0.0, -1.0], [1.0, 0.0]]; // nested array

  var res2 = eigvalsFunc(b);
  writeln(res2);
}
