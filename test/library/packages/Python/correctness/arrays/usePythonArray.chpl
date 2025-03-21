use Python;

proc main() {
  var interp = new Interpreter();
  var mod = interp.importModule("usePythonArray");
  var doit = mod.get('doit');


  var pyRes = doit(owned PyArray(int), 10, 11, 12, 13);
  var res = pyRes.array;
  write("arr: ", res);
  write(" dom: ", res.domain);
  writeln(" eltType: ", res.eltType:string);

}
