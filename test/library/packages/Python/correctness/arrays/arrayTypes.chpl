use Python, CTypes;


proc main() {
  var interp = new Interpreter();
  var array = interp.importModule('array').get('array');
  var memoryview = interp.get('memoryview');
  var np = interp.importModule('numpy');

  var getBuffer = interp.createModule("""
  import struct
  def getBuffer(fmt, *vals):
    n = len(vals)
    buf = struct.pack(fmt*n, *vals)
    return memoryview(buf).cast(fmt, shape=(n,))
  """.dedent()).get('getBuffer');

  proc testArray(type eltType, fmt: string) {
    writeln("Testing format: ", fmt);
    var pyArray = array(owned PyArray(eltType, 1), fmt, [1, 2, 3]);
    writeln("Python array: ", pyArray);
    var chplArray = pyArray.array();
    writeln("Chapel array: ", chplArray);
  }
  writeln("Arrays:");
  testArray(c_schar, 'b');
  testArray(c_uchar, 'B');
  testArray(c_short, 'h');
  testArray(c_ushort, 'H');
  testArray(c_int, 'i');
  testArray(c_uint, 'I');
  testArray(c_long, 'l');
  testArray(c_ulong, 'L');
  testArray(c_longlong, 'q');
  testArray(c_ulonglong, 'Q');
  testArray(c_float, 'f');
  testArray(c_double, 'd');

  try {
    var pyArray = array(owned PyArray(int, 1), '?', [1, 2, 3]);
    var chplArray = pyArray.array();
    writeln("Expected to fail:", pyArray, chplArray, sep=" ");
  } catch e {
    writeln("Caught expected error: ", e);
  }
  try {
    var pyArray = array(owned PyArray(int, 1), 'x', [1, 2, 3]);
    var chplArray = pyArray.array();
    writeln("Expected to fail:", pyArray, chplArray, sep=" ");
  } catch e {
    writeln("Caught expected error: ", e);
  }
  try {
    var pyArray = array(owned PyArray(int(8), 1), 'q', [1, 2, 3]);
    var chplArray = pyArray.array();
    writeln("Expected to fail:", pyArray, chplArray, sep=" ");
  } catch e {
    writeln("Caught expected error: ", e);
  }
  try {
    var pyArray = array(owned PyArray(int, 1), 'L', [1, 2, 3]);
    var chplArray = pyArray.array();
    writeln("Expected to fail:", pyArray, chplArray, sep=" ");
  } catch e {
    writeln("Caught expected error: ", e);
  }
  try {
    var pyArray = array(owned PyArray(uint, 1), 'l', [1, 2, 3]);
    var chplArray = pyArray.array();
    writeln("Expected to fail:", pyArray, chplArray, sep=" ");
  } catch e {
    writeln("Caught expected error: ", e);
  }


  // writeln("Memory views:");
  // proc testMemoryView(type eltType, fmt: string, args...) {
  //   writeln("Testing format: ", fmt);
  //   var pyArray = getBuffer(owned PyArray(eltType, 1), fmt, (...args));
  //   writeln("Python array: ", pyArray);
  //   var chplArray = pyArray.array();
  //   writeln("Chapel array: ", chplArray);
  // }
  // TODO: these cases require us to represent non-writable buffers
  // testMemoryView(bool, '?', true, false);
  // testMemoryView(c_char, 'c', 0x90, 0x91, 0x92);
  // testMemoryView(c_schar, 'b', -1, -2, -3);
  // testMemoryView(c_uchar, 'B', 1, 2, 3);
  // testMemoryView(c_short, 'h', -1, -2, -3);
  // testMemoryView(c_ushort, 'H', 1, 2, 3);
  // testMemoryView(c_int, 'i', -1, -2, -3);
  // testMemoryView(c_uint, 'I', 1, 2, 3);
  // testMemoryView(c_long, 'l', -1, -2, -3);
  // testMemoryView(c_ulong, 'L', 1, 2, 3);
  // testMemoryView(c_longlong, 'q', -1, -2, -3);
  // testMemoryView(c_ulonglong, 'Q', 1, 2, 3);
  // testMemoryView(c_float, 'f', 1.0, 2.0, 3.0);
  // testMemoryView(c_double, 'd', 1.0, 2.0, 3.0);
  // testMemoryView(c_ssize_t, 'n', -1, -2, -3);
  // testMemoryView(c_size_t, 'N', 1, 2, 3);

  proc testNumpy(type eltType, numpyType, args...) {
    writeln("Testing numpy type (1d array): np.", numpyType);
    var dt = np.get(numpyType);
    var pyArray = np.call('array', owned PyArray(eltType, 1), args, kwargs=["dtype"=>dt:borrowed class?]);
    writeln("Python array: ", pyArray);
    var chplArray = pyArray.array();
    writeln("Chapel array: ", chplArray);

    writeln("Testing numpy type (nd array): np.", numpyType);
    const ncol = args.size;
    var pyArray2 = np.call('array', args, kwargs=["dtype"=>dt:borrowed class?])
                     .call(owned PyArray(eltType, 2), 'reshape', (ncol, 1));
    writeln("Python array: ", pyArray2);
    var chplArray2 = pyArray2.array();
    writeln("Chapel array: ", chplArray2);
  }
  testNumpy(bool, 'bool', true, false);
  testNumpy(c_schar, 'byte', -1, -2, -3);
  testNumpy(c_uchar, 'ubyte', 1, 2, 3);
  testNumpy(c_short, 'short', -1, -2, -3);
  testNumpy(c_ushort, 'ushort', 1, 2, 3);
  testNumpy(c_int, 'intc', -1, -2, -3);
  testNumpy(c_uint, 'uintc', 1, 2, 3);
  testNumpy(c_long, 'long', -1, -2, -3);
  testNumpy(c_ulong, 'ulong', 1, 2, 3);
  testNumpy(c_longlong, 'longlong', -1, -2, -3);
  testNumpy(c_ulonglong, 'ulonglong', 1, 2, 3);
  testNumpy(c_float, 'single', 1.0, 2.0, 3.0);
  testNumpy(c_double, 'double', 1.0, 2.0, 3.0);
  testNumpy(c_ssize_t, 'intp', -1, -2, -3);
  testNumpy(c_size_t, 'uintp', 1, 2, 3);

  testNumpy(int(8), 'int8', -1, -2, -3);
  testNumpy(uint(8), 'uint8', 1, 2, 3);
  testNumpy(int(16), 'int16', -1, -2, -3);
  testNumpy(uint(16), 'uint16', 1, 2, 3);
  testNumpy(int(32), 'int32', -1, -2, -3);
  testNumpy(uint(32), 'uint32', 1, 2, 3);
  testNumpy(int(64), 'int64', -1, -2, -3);
  testNumpy(uint(64), 'uint64', 1, 2, 3);
  testNumpy(real(32), 'float32', 1.0, 2.0, 3.0);
  testNumpy(real(64), 'float64', 1.0, 2.0, 3.0);

}
