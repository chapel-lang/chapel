// From test/release/examples/benchmarks/lcals/LCALSMain.chpl
//
//  proc initData(ra: LCALS_Overlapping_Array_3D(real), id: int) {...}
//  var RealArray_3D_2xNx4: [...] owned LCALS_Overlapping_Array_3D(real) ...;
//  initData(s_loop_data.RealArray_3D_2xNx4[i], i+1);

class OAR {
  type t;
}

proc ASDF(ra: OAR(bool)) {  // 'ra' is generic over management strategy
  compilerWarning("ra: ", ra.type:string);
}

var ELM = new owned OAR(bool);
ASDF(ELM);

// Recreating the above with user records

record GRec {
  param p;
}

proc getGeneric(dummyArg) type  return GRec;

proc test1(ref formal1: GRec) {
  compilerWarning("formal1: ", formal1.type:string);
}

proc test2(ref formal2: getGeneric(0)) {
  compilerWarning("formal2: ", formal2.type:string);
}

var act = new GRec(5);
test1(act);
test2(act);

compilerError("done");
