// DO NOT EDIT - THIS FILE IS GENERATED AUTOMATICALLY //

// most contents are copied from
//   isX.module-writeln.decls-chpl


/////////////////////////////////////////////////////////////////////////////
// declare some none-primitive types //

enum EnumType {
  eFirst,
  eSecond,
  eLast
}
const eInstance = EnumType.eSecond;

type HomTupType = 3*int;
const homInstance: HomTupType = (1011, 1012, 1013);

type HetTupType = (int, real, string);
const hetInstance = (101, 102.5, "hetInstance");

class ClassType {
  var zzz: int;
}
const cInstance = new borrowed ClassType(44444444);

record RecordSmall {
  var xxx: int;
}
const rSmall = new RecordSmall(xxx=600033);

union UnionType {
  var ufield111, ufield222: int;
}
var uInstanceVar: UnionType;
uInstanceVar.ufield222 = 339933;
const uInstance = uInstanceVar;

type DomType1 = domain(1);
const init1dom: DomType1 = {7770..7771};

type DomType2 = domain(2, uint(32), true);
const init2dom: DomType2 = {110001..110002:uint(32),
                            330004..330033:uint(32) by 3};

type ArrType1 = [init1dom] int;
const init1arr: ArrType1 = 11011;

type ArrType2 = [init2dom] uint(16);
const init2arr: ArrType2; // TODO: = 5555;



/////////////////////////////////////////////////////////////////////////////
// declare our varaibles //

var b0:  bool;
var b8:  bool(8);
var b16: bool(16);
var b32: bool(32);
var b64: bool(64);

var i8:  int(8);
var i16: int(16);
var i32: int(32);
var i64: int(64);

var u8:  uint(8);
var u16: uint(16);
var u32: uint(32);
var u64: uint(64);

var r32:  real(32);
var r64:  real(64);
var m32:  imag(32);
var m64:  imag(64);
var z64:  complex(64);
var z128: complex(128);

var str:    string;
var enm:    EnumType;
var homtup: HomTupType;
var hettup: HetTupType;

var cls:  borrowed ClassType = new borrowed ClassType();
var rec1: RecordSmall;
var unn:  UnionType;

var rng1: range;
var rng2: range(uint(8), BoundedRangeType.boundedNone, true);
var dmp = defaultDist;
var dom1: DomType1;
var dom2: DomType2;
var arr1: ArrType1;
var arr2: ArrType2;

var syInt:  sync int;
var syReal: sync real;
var siInt:  single int;
var siReal: single real;
var aInt:   atomic int;
var aReal:  atomic real;


/////////////////////////////////////////////////////////////////////////////

proc report(param is: bool, param msg: string) {
  if is then compilerWarning(msg);
}

proc test(arg) {
  compilerWarning(arg.type:string);
  report(isBool(arg), "isBool");
  report(isBoolValue(arg), "isBoolValue");
  report(isBoolType(arg.type), "isBoolType");
  report(isInt(arg), "isInt");
  report(isIntValue(arg), "isIntValue");
  report(isIntType(arg.type), "isIntType");
  report(isUint(arg), "isUint");
  report(isUintValue(arg), "isUintValue");
  report(isUintType(arg.type), "isUintType");
  report(isReal(arg), "isReal");
  report(isRealValue(arg), "isRealValue");
  report(isRealType(arg.type), "isRealType");
  report(isImag(arg), "isImag");
  report(isImagValue(arg), "isImagValue");
  report(isImagType(arg.type), "isImagType");
  report(isComplex(arg), "isComplex");
  report(isComplexValue(arg), "isComplexValue");
  report(isComplexType(arg.type), "isComplexType");
  report(isString(arg), "isString");
  report(isStringValue(arg), "isStringValue");
  report(isStringType(arg.type), "isStringType");
  report(isEnum(arg), "isEnum");
  report(isEnumValue(arg), "isEnumValue");
  report(isEnumType(arg.type), "isEnumType");
  report(isTuple(arg), "isTuple");
  report(isTupleValue(arg), "isTupleValue");
  report(isTupleType(arg.type), "isTupleType");
  report(isClass(arg), "isClass");
  report(isClassValue(arg), "isClassValue");
  report(isClassType(arg.type), "isClassType");
  report(isRecord(arg), "isRecord");
  report(isRecordValue(arg), "isRecordValue");
  report(isRecordType(arg.type), "isRecordType");
  report(isUnion(arg), "isUnion");
  report(isUnionValue(arg), "isUnionValue");
  report(isUnionType(arg.type), "isUnionType");
  report(isRange(arg), "isRange");
  report(isRangeValue(arg), "isRangeValue");
  report(isRangeType(arg.type), "isRangeType");
  report(isDmap(arg), "isDmap");
  report(isDmapValue(arg), "isDmapValue");
  report(isDmapType(arg.type), "isDmapType");
  report(isDomain(arg), "isDomain");
  report(isDomainValue(arg), "isDomainValue");
  report(isDomainType(arg.type), "isDomainType");
  report(isArray(arg), "isArray");
  report(isArrayValue(arg), "isArrayValue");
  report(isArrayType(arg.type), "isArrayType");
  report(isSync(arg), "isSync");
  report(isSyncValue(arg), "isSyncValue");
  report(isSyncType(arg.type), "isSyncType");
  report(isSingle(arg), "isSingle");
  report(isSingleValue(arg), "isSingleValue");
  report(isSingleType(arg.type), "isSingleType");
  report(isAtomic(arg), "isAtomic");
  report(isAtomicValue(arg), "isAtomicValue");
  report(isAtomicType(arg.type), "isAtomicType");
  report(isIntegral(arg), "isIntegral");
  report(isIntegralValue(arg), "isIntegralValue");
  report(isIntegralType(arg.type), "isIntegralType");
  report(isFloat(arg), "isFloat");
  report(isFloatValue(arg), "isFloatValue");
  report(isFloatType(arg.type), "isFloatType");
  report(isNumeric(arg), "isNumeric");
  report(isNumericValue(arg), "isNumericValue");
  report(isNumericType(arg.type), "isNumericType");
  report(isPrimitive(arg), "isPrimitive");
  report(isPrimitiveValue(arg), "isPrimitiveValue");
  report(isPrimitiveType(arg.type), "isPrimitiveType");
  if isTuple(arg) {
    report(isHomogeneousTuple(arg),"isHomogeneousTuple");
    report(isHomogeneousTupleValue(arg),"isHomogeneousTupleValue");
    report(isHomogeneousTupleType(arg.type),"isHomogeneousTupleType");
  }
  compilerWarning("");
}

test(b0);
test(b8);
test(b16);
test(b32);
test(b64);
test(i8);
test(i16);
test(i32);
test(i64);
test(u8);
test(u16);
test(u32);
test(u64);
test(r32);
test(r64);
test(m32);
test(m64);
test(z64);
test(z128);
test(str);
test(enm);
test(homtup);
test(hettup);
test(cls);
test(rec1);
test(unn);
test(rng1);
test(rng2);
test(dmp);
test(dom1);
test(dom2);
test(arr1);
test(arr2);
test(syInt);
test(syReal);
test(siInt);
test(siReal);
test(aInt);
test(aReal);

compilerError("done"); // no need to execute
