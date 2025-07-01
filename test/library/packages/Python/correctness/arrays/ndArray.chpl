use Python;


var interp = new Interpreter();
var mod = interp.createModule(
  """
  import numpy as np

  def modify_idx(arr, idx, value):
      x = np.asarray(arr)
      x[idx] = value
  """.dedent());

var modify_idx = mod.get('modify_idx');

var np = interp.importModule('numpy');
var arange = np.get('arange');
var zeros = np.get('zeros');
var np_sum = np.get('sum');

proc getPyArray(type eltType, shape) {
  proc toNP(type t: int(64)) do return np.get('int64');
  proc toNP(type t: real(64)) do return np.get('float64');
  proc toNP(type t: bool) do return np.get('bool');
  proc toNP(type t) do compilerError("unsupported type");
  var dtype = toNP(eltType);
  var size = * reduce shape;
  var arr = if eltType == bool then
              zeros(shape, kwargs=["dtype"=>dtype:borrowed class?])
            else
              arange(size, kwargs=["dtype"=>dtype:borrowed class?])
                .call('reshape', shape);
  return arr;
}
proc getChplArray(type eltType, shape) {
  var ranges: shape.size * range(int);
  for param i in 0..#shape.size do
    ranges[i] = 0..#shape[i];
  var arr: [(...ranges)] eltType;
  for i in 0..#arr.size do
    arr[arr.domain.orderToIndex(i)] = (i+1):eltType;
  return arr;
}

proc main() {

  testArray(int, (2, 3, 2));
  testArray(real, (8, 2));
  testArray(bool, (4,));

}


proc testArray(type eltType, shape) {
  param rank = shape.size;

  var chplArr = getChplArray(eltType, shape);

  writeln("Testing array with type '", chplArr.type:string,
          "' and shape ", chplArr.shape);

  writeln("calling modify_idx(pyArr, 0, 100)");
  modify_idx(new Array(interp, chplArr), 0, 100);
  writeln("after calling modify_idx:\n", chplArr);

  writeln("====");

  var pyArr_genericValue = getPyArray(eltType, shape);
  writeln("numpy array:\n", pyArr_genericValue);

  writeln("====");

  {
    var pyArr_genericPyArr = pyArr_genericValue.value(owned PyArray);
    writeln("pyArr_genericPyArr.ndim: ", pyArr_genericPyArr.ndim);
    writeln("pyArr_genericPyArr.size: ", pyArr_genericPyArr.size);
    writeln("pyArr_genericPyArr.shape: ", pyArr_genericPyArr.shape(rank));
    var idx: rank*int;
    for param i in 0..#rank do idx[i] = 1;
    writeln("pyArr_genericPyArr.this(",idx,"): ",
      pyArr_genericPyArr[eltType, idx]);
    for e in pyArr_genericPyArr.values(eltType) {
      writeln("element: ", e);
    }
    var sum = 0:(if eltType == bool then int else eltType);
    forall e in pyArr_genericPyArr.values(eltType) with (+ reduce sum) {
      sum reduce= e;
    }
    writeln("sum: ", sum:eltType);
    writeln("np.sum: ", np_sum(pyArr_genericPyArr):eltType);
    writeln("pyArr_genericPyArr.array: ",
      pyArr_genericPyArr.array(eltType, rank));
  }

  writeln("====");

  {
    var pyArr_typeSpecified = pyArr_genericValue.value(owned PyArray(eltType));
    writeln("pyArr_typeSpecified.ndim: ", pyArr_typeSpecified.ndim);
    writeln("pyArr_typeSpecified.size: ", pyArr_typeSpecified.size);
    writeln("pyArr_typeSpecified.shape: ", pyArr_typeSpecified.shape(rank));
    var idx: rank*int;
    writeln("pyArr_typeSpecified.this(",idx,"): ",
      pyArr_typeSpecified[idx]);
    for e in pyArr_typeSpecified.values() {
      writeln("element: ", e);
    }
    var sum = 0:(if eltType == bool then int else eltType);
    forall e in pyArr_typeSpecified.values() with (+ reduce sum) {
      sum reduce= e;
    }
    writeln("sum: ", sum:eltType);
    writeln("np.sum: ", np_sum(pyArr_typeSpecified):eltType);
    writeln("pyArr_typeSpecified.array: ", pyArr_typeSpecified.array(rank));
  }

  writeln("====");

  {
    var pyArr_rankSpecified =
      pyArr_genericValue.value(owned PyArray(rank=rank));
    writeln("pyArr_rankSpecified.ndim: ", pyArr_rankSpecified.ndim);
    writeln("pyArr_rankSpecified.size: ", pyArr_rankSpecified.size);
    writeln("pyArr_rankSpecified.shape: ", pyArr_rankSpecified.shape());
    var idx: rank*int;
    for param i in 0..#rank do idx[i] = shape(i)-1;
    writeln("pyArr_rankSpecified.this(",idx,"): ",
      pyArr_rankSpecified[eltType, idx]);
    for e in pyArr_rankSpecified.values(eltType) {
      writeln("element: ", e);
    }
    var sum = 0:(if eltType == bool then int else eltType);
    forall e in pyArr_rankSpecified.values(eltType) with (+ reduce sum) {
      sum reduce= e;
    }
    writeln("sum: ", sum:eltType);
    writeln("np.sum: ", np_sum(pyArr_rankSpecified):eltType);
    writeln("pyArr_rankSpecified.array: ",
      pyArr_rankSpecified.array(eltType));
  }

  writeln("====");

  {
    var pyArr_fullySpecified = pyArr_genericValue.value(owned PyArray(eltType, rank));
    writeln("pyArr_fullySpecified.ndim: ", pyArr_fullySpecified.ndim);
    writeln("pyArr_fullySpecified.size: ", pyArr_fullySpecified.size);
    writeln("pyArr_fullySpecified.shape: ", pyArr_fullySpecified.shape());
    var idx: rank*int;
    writeln("pyArr_fullySpecified.this(",idx,"): ",
      pyArr_fullySpecified[idx]);
    for e in pyArr_fullySpecified.values() {
      writeln("element: ", e);
    }
    var sum = 0:(if eltType == bool then int else eltType);
    forall e in pyArr_fullySpecified.values() with (+ reduce sum) {
      sum reduce= e;
    }
    writeln("sum: ", sum:eltType);
    writeln("np.sum: ", np_sum(pyArr_fullySpecified):eltType);
    writeln("pyArr_fullySpecified.array: ",
      pyArr_fullySpecified.array());
  }

  writeln("====");

  {
    var pyArr_fullySpecified = pyArr_genericValue.value(owned PyArray(eltType, rank));

    {
      var idx: rank*int;
      pyArr_fullySpecified[idx] = 100:eltType;
      writeln("pyArr_fullySpecified.this(",idx,"): ",
        pyArr_fullySpecified[idx]);
    }

    {
      var idx: rank*int;
      for param i in 0..#rank do idx[i] = 1;
      pyArr_fullySpecified[idx] = 9:eltType;
      writeln("pyArr_fullySpecified.this(",idx,"): ",
        pyArr_fullySpecified[idx]);
    }

    {
      var idx: rank*int;
      for param i in 0..#rank do idx[i] = shape(i)-1;
      pyArr_fullySpecified[idx] = 7:eltType;
      writeln("pyArr_fullySpecified.this(",idx,"): ",
        pyArr_fullySpecified[idx]);
    }

    if shape.size == 1 {
      writeln("pyArr_fullySpecified.this(1): ", pyArr_fullySpecified.this(1));
    }

    for e in pyArr_fullySpecified.values() {
      writeln("element: ", e);
      e = 0:eltType;
    }
    writeln("pyArr_fullySpecified: ", pyArr_fullySpecified);
    forall e in pyArr_fullySpecified.values() {
      e = 1:eltType;
    }
    writeln("pyArr_fullySpecified: ", pyArr_fullySpecified);
  }

  writeln("========================================");
}
