use Types;

proc checkit(type t,
             param expectCopyable,
             param expectConstCopyable,
             param expectAssignable,
             param expectConstAssignable,
             param expectDefaultInitable) {

  if isCopyable(t) != expectCopyable then
    compilerError("isCopyable " + t:string + " did not match");

  if isConstCopyable(t) != expectConstCopyable then
    compilerError("isConstCopyable " + t:string + " did not match");
  
  if isAssignable(t) != expectAssignable then
    compilerError("isAssignable " + t:string + " did not match");

  if isConstAssignable(t) != expectConstAssignable then
    compilerError("isConstAssignable " + t:string + " did not match");

  if isDefaultInitializable(t) != expectDefaultInitable then
    compilerError("isDefaultInitializable " + t:string + " did not match");
}

proc checkit(e,
             param expectCopyable,
             param expectConstCopyable,
             param expectAssignable,
             param expectConstAssignable,
             param expectDefaultInitable) {

  if isCopyable(e) != expectCopyable then
    compilerError("isCopyable " + e.type:string + " did not match");

  if isConstCopyable(e) != expectConstCopyable then
    compilerError("isConstCopyable " + e.type:string + " did not match");
  
  if isAssignable(e) != expectAssignable then
    compilerError("isAssignable " + e.type:string + " did not match");

  if isConstAssignable(e) != expectConstAssignable then
    compilerError("isConstAssignable " + e.type:string + " did not match");

  if isDefaultInitializable(e) != expectDefaultInitable then
    compilerError("isDefaultInitializable " + e.type:string + " did not match");
}


proc checkNormal(type t, e) {
  checkit(t, true, true, true, true, true);
  if t != e.type then
    compilerError("types do not match");
  checkit(e, true, true, true, true, true);
}
proc checkNormalNoDefault(type t, e) {
  checkit(t, true, true, true, true, false);
  if t != e.type then
    compilerError("types do not match");
  checkit(e, true, true, true, true, false);
}
proc checkMutable(type t, e) {
  checkit(t, true, false, true, false, true);
  if t != e.type then
    compilerError("types do not match");
  checkit(e, true, false, true, false, true);
}
proc checkNo(type t, e) {
  checkit(t, false, false, false, false, false);
  if t != e.type then
    compilerError("types do not match");
  checkit(e, false, false, false, false, false);
}

class C { var x: int; }
record R { var y: int; }
enum MyEnum { val }

record GenericRecord { var x; }

record GenericRecordDefault { type t = int; var z: t; }

proc main() {

  var ownedC = new owned C();
  var ownedCQ = new owned C?();
  var sharedC = new shared C();
  var sharedCQ = new shared C?();
  var unmanagedC = new unmanaged C();
  var unmanagedCQ = new unmanaged C?();

  var at: atomic int;
  var genericR = new GenericRecord(1);
  var genericRD: GenericRecordDefault;

  checkNormal(int, 1);
  checkNormal(real, 1.0);
  checkNormal(bool, true);
  checkNormal(MyEnum, MyEnum.val);
  checkNormal(R, new R(1));
  checkNormalNoDefault(borrowed C, ownedC.borrow());
  checkNormal(borrowed C?, ownedCQ.borrow());
  checkNormalNoDefault(unmanaged C, unmanagedC);
  checkNormal(unmanaged C?, unmanagedCQ);
  checkNormalNoDefault(shared C, sharedC);
  checkNormal(shared C?, sharedCQ);

  // nilable owned can generally be copied / assigned but only from a ref
  checkMutable(owned C?, ownedCQ);

  // non-nilable owned cannot generally be copied / assigned 
  checkNo(owned C, ownedC);

  var tup = ("a", "b", "c");
  checkNormal((string, string, string), tup);

  checkNormal(atomic int, at);
  checkNormal(GenericRecord(int), genericR);
  checkNormal(GenericRecordDefault, genericRD);

  delete unmanagedCQ;
  delete unmanagedC;
}
