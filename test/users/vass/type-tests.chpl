// Check isClassType, isRecordType, isUnionType.

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
var cls: C;              test("class", cls);
var rec: R;              test("record", rec);
var uni: U;              test("union", uni);
var dm1 = new Block(LocaleSpace); test("dist", dm1);
var dm2 = new dmap(dm1); test("dmap", dm2);
var dom: domain(1);      test("domain", dom);
var arr: [dom] int;      test("array", arr);
var rng: range(int);     test("range", rng);
var tu1 = (1,);          test("tuple-1", tu1);
var tu2 = (1,2);         test("tuple-2", tu2);
var tu3 = (1,2,3);       test("tuple-3", tu3);
var atm: atomic int;     test("atomic", atm);
//var snc: sync int;       test("sync", snc);
//var sng: single int;     test("single", sng);
// these are temporary workarounds - see the comment below
var snc: sync int;       test("sync", snc.type);
var sng: single int;     test("single", sng.type);
compilerError("done"); // no need for a C compile

proc test(param msg, ref v) {
  compilerWarning(msg + " is a class:  " + isClassType(v.type):c_string);
  compilerWarning(msg + " is a record: " + isRecordType(v.type):c_string);
  compilerWarning(msg + " is a union:  " + isUnionType(v.type):c_string);
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

// This is a workaround for test/functions/vass/sync-by-ref.future,
// temporarily here to confirm that isRecordType et al. actually work.
// Once that future is fixed, we should probably remove this workaround.
proc test(param msg, type tp) {
  compilerWarning(msg + " - class:  " + isClassType(tp):c_string);
  compilerWarning(msg + " - record: " + isRecordType(tp):c_string);
  compilerWarning(msg + " - union:  " + isUnionType(tp):c_string);
  if isRangeType(tp)   then compilerWarning(msg + " is range");
  if isTupleType(tp)   then compilerWarning(msg + " is tuple");
  if isDmapType(tp)    then compilerWarning(msg + " is dmap");
  if isDomainType(tp)  then compilerWarning(msg + " is domain");
  if isArrayType(tp)   then compilerWarning(msg + " is array");
  if isAtomicType(tp)  then compilerWarning(msg + " is atomic");
  if isSyncType(tp)    then compilerWarning(msg + " is sync");
  if isSingleType(tp)  then compilerWarning(msg + " is single");
}
