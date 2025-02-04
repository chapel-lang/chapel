use Python;


var code = """

import numpy as np

def gradModifyArg(arr, res):
    arr_np = np.asarray(arr)
    res_np = np.asarray(res)

    res_np[:] = np.gradient(arr_np)

def grad(arr):
    x = np.asarray(arr)
    return np.gradient(x)

""";

proc main() {
  var interp = new Interpreter();
  var mod = new Module(interp, '__empty__', code);
  var gradModifyArg = new Function(mod, 'gradModifyArg');
  var grad = new Function(mod, 'grad');

  var arr = [1, 2, 4, 7, 11, 16];
  var pyArr = new Array(interp, arr);


  var res: [arr.domain] real;
  {
    var pyRes = new Array(interp, res);
    gradModifyArg(NoneType, pyArr, pyRes);
  }
  write("gradModifyArg: ", res);
  write(" dom: ", res.domain);
  writeln(" eltType: ", res.eltType:string);

  res = 0.0;

  {
    var pyRes = grad(owned PyArray(real), pyArr);
    res = pyRes.array;
  }
  write("grad: ", res);
  write(" dom: ", res.domain);
  writeln(" eltType: ", res.eltType:string);
}
