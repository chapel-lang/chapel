// ensure that in-intent formals are codegen-ed correctly, see #23419
// for each arg type, this test has an 'export' proc with an in-intent formal
// that (1) assigns into it then (2) calls another fn twice, passing the formal by ref

/// bool ///

proc updateBool(ref updateArg: bool) {
  write(updateArg.type:string, " updateArg: ", updateArg);
  updateArg = !updateArg;
  writeln(" --> ", updateArg);
}

export proc exportBool(in exportArg: bool) {
  exportArg = false;
  updateBool(exportArg);
  updateBool(exportArg);
  writeln(exportArg.type:string, " finally:   ", exportArg);
}

/// int(64) ///

proc updateInt64(ref updateArg: int(64)) {
  write(updateArg.type:string, " updateArg: ", updateArg);
  updateArg += 1000;
  writeln(" --> ", updateArg);
}

export proc exportInt64(in exportArg: int(64)) {
  exportArg = 1064;
  updateInt64(exportArg);
  updateInt64(exportArg);
  writeln(exportArg.type:string, " finally:   ", exportArg);
}

/// uint(64) ///

proc updateUint64(ref updateArg: uint(64)) {
  write(updateArg.type:string, " updateArg: ", updateArg);
  updateArg += 2000;
  writeln(" --> ", updateArg);
}

export proc exportUint64(in exportArg: uint(64)) {
  exportArg = 2064;
  updateUint64(exportArg);
  updateUint64(exportArg);
  writeln(exportArg.type:string, " finally:   ", exportArg);
}

/// int16 ///

proc updateInt16(ref updateArg: int(16)) {
  write(updateArg.type:string, " updateArg: ", updateArg);
  updateArg += 1002;
  writeln(" --> ", updateArg);
}

export proc exportInt16(in exportArg: int(16)) {
  exportArg = 1016;
  updateInt16(exportArg);
  updateInt16(exportArg);
  writeln(exportArg.type:string, " finally:   ", exportArg);
}

/// uint8 ///

proc updateUint8(ref updateArg: uint(8)) {
  write(updateArg.type:string, " updateArg: ", updateArg);
  updateArg += 100;
  writeln(" --> ", updateArg);
}

export proc exportUint8(in exportArg: uint(8)) {
  exportArg = 8;
  updateUint8(exportArg);
  updateUint8(exportArg);
  writeln(exportArg.type:string, " finally:   ", exportArg);
}


/// real ///

proc updateReal(ref updateArg: real) {
  write(updateArg.type:string, " updateArg: ", updateArg);
  updateArg += 1.1;
  writeln(" --> ", updateArg);
}

export proc exportReal(in exportArg: real) {
  exportArg = 10.64;
  updateReal(exportArg);
  updateReal(exportArg);
  writeln(exportArg.type:string, " finally:   ", exportArg);
}

/// imag ///

proc updateImag(ref updateArg: imag) {
  write(updateArg.type:string, " updateArg: ", updateArg);
  updateArg += 1.0i;
  writeln(" --> ", updateArg);
}

export proc exportImag(in exportArg: imag) {
  exportArg = 1.64i;
  updateImag(exportArg);
  updateImag(exportArg);
  writeln(exportArg.type:string, " finally:   ", exportArg);
}

/// complex ///

proc updateComplex(ref updateArg: complex) {
  write(updateArg.type:string, " updateArg: ", updateArg);
  updateArg += 1.1+2.01i;
  writeln(" --> ", updateArg);
}

export proc exportComplex(in exportArg: complex) {
  exportArg = 10+20i;
  updateComplex(exportArg);
  updateComplex(exportArg);
  writeln(exportArg.type:string, " finally:   ", exportArg);
}

/// record ///

import CTypes;

extern record RR {
  //we can hide this field: var i1: CTypes.c_int;
  var i2: CTypes.c_int;
}

extern var externRecord: RR;
var chapelRecord: RR;
chapelRecord.i2 = 1023;

proc updateRec(ref updateArg: RR) {
  write(updateArg.type:string, " updateArg: ", updateArg);
  updateArg.i2 += 1000;
  writeln(" --> ", updateArg);
}

export proc exportRec(in exportArg: RR) {
  exportArg = chapelRecord;
  updateRec(exportArg);
  updateRec(exportArg);
  writeln(exportArg.type:string, " finally:   ", exportArg);
}

/// string ///

proc updateString(ref updateArg: string) {
  write(updateArg.type:string, " updateArg: ", updateArg);
  updateArg += " there";
  writeln(" --> ", updateArg);
}

export proc exportString(in exportArg: string) {
  exportArg = "hi";
  updateString(exportArg);
  updateString(exportArg);
  writeln(exportArg.type:string, " finally:   ", exportArg);
}

exportBool(true);
exportInt64(4564);
exportUint64(4654);
exportInt16(616);
exportUint8(88);
exportReal(64.46);
exportImag(64.64i);
exportComplex(22+33i);
exportRec(externRecord);
exportString("a string");
