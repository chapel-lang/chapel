use Python;

config param testCase: int = 1;

proc main() {
  var interp = new Interpreter();
  var np = interp.importModule("numpy");

  var zeros = np.get("zeros");
  var dtInt = np.get("int64");
  var dtReal = np.get("float64");
  var dtReal32 = np.get("float32");

  var py_arr_int_3d =
    zeros(owned PyArray, (3, 3, 3), kwargs=["dtype"=>dtInt:borrowed class?]);
  var py_arr_real_2d =
    zeros(owned PyArray, (2, 2), kwargs=["dtype"=>dtReal:borrowed class?]);
  var py_arr_real32_1d =
    zeros(owned PyArray, (4,), kwargs=["dtype"=>dtReal32:borrowed class?]);

  writeln(py_arr_int_3d);
  writeln(py_arr_real_2d);
  writeln(py_arr_real32_1d);


  try {
    // error: index with wrong tuple
    py_arr_int_3d(int, (1, 2));
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: OOB
    py_arr_int_3d(int, (-1, -1, -1));
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: OOB
    py_arr_int_3d(int, (4, 0, 0));
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: bad type
    py_arr_int_3d(real, (1, 2, 2));
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: bad type
    for i in py_arr_int_3d.values(bool) {
      writeln(i);
    }
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: bad type
    forall i in py_arr_int_3d.values(real(32)) {
      writeln(i);
    }
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: index 2d with 1d
    var arr = py_arr_real_2d(real, (1,));
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: wrong rank
    var shape = py_arr_real32_1d.value(owned PyArray(rank=2)).shape();
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: index 1d with 2d
    var arr = py_arr_real32_1d(real(32), (1, 2));
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: wrong type
    var a = (py_arr_int_3d:owned PyArray(uint)).array(3);
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: wrong rank
    var a = (py_arr_int_3d:owned PyArray(int, 1)).array();
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: 0-dim array
    var np_scalar = np.call(owned PyArray, 'array', 17);
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: index 2d with 1d
    var arr = py_arr_real_2d(real, 1);
  } catch e {
    writeln("Caught error: ", e);
  }
  try {
    // error: index with scalar
    py_arr_int_3d(int, 1);
  } catch e {
    writeln("Caught error: ", e);
  }

  if testCase == 2 {
    // compile error: imag
    var arr = [1i, 2i];
    var pyArr = new Array(interp, arr);
  } else if testCase == 3 {
    // compile error: complex
    var arr = [2+1i, 3+2i];
    var pyArr = new Array(interp, arr);
  } else if testCase == 4 {
    // compile error: index generic pyarray
    py_arr_int_3d(1);
  } else if testCase == 5 {
    // compile error: index with scalar
    (py_arr_int_3d:owned PyArray(int, 3))(1);
  } else if testCase == 6 {
    // compile error: index with wrong tuple
    (py_arr_int_3d:owned PyArray(int, 3))(int, (1, 2));
  } else if testCase == 7 {
    // compile error: bad idx
    py_arr_int_3d(int, (1, 2, 3.0));
  } else if testCase == 8 {
    // compile error: no type
    for i in py_arr_int_3d.values() {
      writeln(i);
    }
  } else if testCase == 9 {
    // compile error: no type or rank
    var a = py_arr_int_3d.array();
  } else if testCase == 10 {
    // compile error: no rank
    var a = py_arr_int_3d.array(int);
  } else if testCase == 11 {
    // compile error: no rank
    var shape = py_arr_real_2d.shape();
  } else if testCase == 12 {
    // compile error: index 2d with 1d
    var arr = py_arr_real_2d.value(owned PyArray(rank=2))(real, (1,));
  }
}
