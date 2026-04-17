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
  var mod = interp.createModule(code);
  var gradModifyArg = mod.get('gradModifyArg');
  var grad = mod.get('grad');

  {
    var arr = [1, 2, 4, 7, 11, 16];
    var pyArr = new Array(interp, arr);


    var res: [arr.domain] real;
    {
      var pyRes = new Array(interp, res);
      gradModifyArg(pyArr, pyRes);
    }
    write("gradModifyArg: ", res);
    write(" dom: ", res.domain);
    writeln(" eltType: ", res.eltType:string);

    res = 0.0;

    {
      var pyRes = grad(owned PyArray(real, 1), pyArr);
      res = pyRes.array();
    }
    write("grad: ", res);
    write(" dom: ", res.domain);
    writeln(" eltType: ", res.eltType:string);
  }

  {

    var arr = [1, 2, 6 ; 3, 4, 5];
    var pyArr = new Array(interp, arr);

    var asarray = interp.importModule('numpy').get('asarray');
    var gradient = interp.importModule('numpy').get('gradient');

    var res = gradient((owned PyArray?, owned PyArray?), asarray(pyArr));
    writeln("grad.0:\n", res(0));
    writeln("grad.1:\n", res(1));

  }

}
