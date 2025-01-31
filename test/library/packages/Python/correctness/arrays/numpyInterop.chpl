use Python;


var code = """
import numpy as np

def saxpy(x, y, a):
  x_np = np.asarray(x)
  y_np = np.asarray(y)
  return a*x_np + y_np

def numpyAssign(arr):
  np_arr = np.asarray(arr)
  np_arr[:] = 100
""";

proc main() {

  var interp = new Interpreter();
  var m = new Module(interp, '__empty__', code);
  var saxpy = new Function(m, 'saxpy');
  var numpyAssign = new Function(m, 'numpyAssign');

  {
    var x = [1, 2, 3];
    var y = [4, 5, 6];
    var a = 2;

    var xArr = new Array(interp, x);
    var yArr = new Array(interp, y);

    var result = saxpy(owned Value, xArr, yArr, a);
    writeln("saxpy on 1D arrays: ", result);
  }
  writeln();
  {
    var x = [[1.0, 2.0], [3.0, 4.0]];
    var y = [[5.0, 6.0], [7.0, 8.0]];
    var a = 17.0;

    var xArr = new Array(interp, x);
    var yArr = new Array(interp, y);

    var result = saxpy(owned Value, xArr, yArr, a);
    writeln("saxpy on nested arrays: ", result);
  }
  writeln();
  {
    var x = [1, 2, 3, 4];
    writeln("Before numpyAssign: ", x);
    {
      var xArr = new Array(interp, x);
      numpyAssign(NoneType, xArr);
    }
    writeln("After numpyAssign: ", x);
  }


}
