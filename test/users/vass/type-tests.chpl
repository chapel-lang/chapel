// Check isClassType, isRecordType, isUnionType.
// There are also more extensive tests in: test/users/vass/isX/
use BlockDist;

class C  { var cl:int; }
record R { var re:int; }
union U  { var u1, u2: int; }

var b: bool;             test("bool", b);
var i: int;              test("int", i);
var ui: uint;            test("uint", ui);
var r: real;             test("real", r);
var cmp: complex;        test("complex", cmp);
var img: imag;           test("imag", img);
var str: string;         test("string", str);
var cls: borrowed C = new borrowed C();     test("class", cls);
var rec: R;              test("record", rec);
var uni: U;              test("union", uni);
var dm1 = new unmanaged Block(LocaleSpace); test("dist", dm1);
var dm2 = new dmap(dm1); test("dmap", dm2);
var dom: domain(1);      test("domain", dom);
var arr: [dom] int;      test("array", arr);
var rng: range(int);     test("range", rng);
var tu1 = (1,);          test("tuple-1", tu1);
var tu2 = (1,2);         test("tuple-2", tu2);
var tu3 = (1,2,3);       test("tuple-3", tu3);
var atm: atomic int;     test("atomic", atm);
var snc: sync int;       test("sync", snc);
var sng: single int;     test("single", sng);
var oc: owned C = new owned C();            test("owned class", oc);
compilerError("done"); // no need for a C compile

proc test(param msg, v) {
  compilerWarning(msg + " is a class:  " + isClassType(v.type):string);
  compilerWarning(msg + " is a record: " + isRecordType(v.type):string);
  compilerWarning(msg + " is a union:  " + isUnionType(v.type):string);
  if isRangeType(v.type)  then compilerWarning(msg + " is a range");
  if isTupleType(v.type)  then compilerWarning(msg + " is a tuple");
  if isDmapType(v.type)   then compilerWarning(msg + " is a dmap");
  if isDomainType(v.type) then compilerWarning(msg + " is a domain");
  if isArrayType(v.type)  then compilerWarning(msg + " is an array");
  if isAtomicType(v.type) then compilerWarning(msg + " is an atomic");
  if isSyncType(v.type)   then compilerWarning(msg + " is a sync");
  if isSingleType(v.type) then compilerWarning(msg + " is a single");
// Todo: would be nice to add versions of the above tests on variables
// instead of types, e.g. isTuple(v) in addition to isTupleType(v.type).
}
