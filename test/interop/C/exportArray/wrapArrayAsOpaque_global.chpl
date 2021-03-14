use BlockDist;
const D = {1..5} dmapped Block({1..5});
var global: [D] int;

// returns getExternalArrayType(makeBlockArray_chpl.ret)
export proc makeBlockArray(): chpl_opaque_array {
  var ret = convertToExternalArray(makeBlockArray_chpl());
  return ret;
}

proc makeBlockArray_chpl(): [D] int {
  return global;
}

export proc makeBlockArrayRef(): chpl_opaque_array {
  var ret = convertToExternalArray(makeBlockArrayRef_chpl());
  return ret;
}

proc makeBlockArrayRef_chpl() ref : [D] int {
  return global;
}


// argument type is getExternalArrayType(printBlock_chpl's x)
export proc printBlock(const ref x: chpl_opaque_array) {
  type x_type = [D] int;
  ref chpl_x = makeArrayFromOpaque(x,
                                   __primitive("static field type", x_type,
                                               "_instance"));
  printBlock_chpl(chpl_x);
}

proc printBlock_chpl(x: [D] int) {
  var output = "";
  var first = true;
  for idx in x.dom {
    if (first) {
      first = false;
    } else {
      output += ", ";
    }
    output += idx: string + "=";
    output += x[idx]: string;
  }
  writeln(output);
}

export proc addEltBlock(ref x: chpl_opaque_array, idx: int, val: int) {
  type x_type = [D] int;
  ref chpl_x = makeArrayFromOpaque(x,
                                   __primitive("static field type", x_type,
                                               "_instance"));
  addEltBlock(chpl_x, idx, val);
}

proc addEltBlock(x: [D] int, idx: int, val: int) {
  x[idx] = val;
}
