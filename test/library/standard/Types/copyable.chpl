use Types;

proc checkit(type t,
             param expectCopyable,
             param expectRefCopyable,
             param expectAssignable,
             param expectRefAssignable) {

  if isCopyable(t) != expectCopyable then
    compilerError("isCopyable " + t:string + " did not match");

  if isRefCopyable(t) != expectRefCopyable then
    compilerError("isRefCopyable " + t:string + " did not match");
  
  if isAssignable(t) != expectAssignable then
    compilerError("isAssignable " + t:string + " did not match");

  if isRefAssignable(t) != expectRefAssignable then
    compilerError("isRefAssignable " + t:string + " did not match");

}

proc checkit(e,
             param expectCopyable,
             param expectRefCopyable,
             param expectAssignable,
             param expectRefAssignable) {

  if isCopyable(e) != expectCopyable then
    compilerError("isCopyable " + e.type:string + " did not match");

  if isRefCopyable(e) != expectRefCopyable then
    compilerError("isRefCopyable " + e.type:string + " did not match");
  
  if isAssignable(e) != expectAssignable then
    compilerError("isAssignable " + e.type:string + " did not match");

  if isRefAssignable(e) != expectRefAssignable then
    compilerError("isRefAssignable " + e.type:string + " did not match");

}


proc checkNormal(type t, e) {
  checkit(t, true, false, true, false);
  if t != e.type then
    compilerError("types do not match");
  checkit(e, true, false, true, false);
}
proc checkMutable(type t, e) {
  checkit(t, false, true, false, true);
  if t != e.type then
    compilerError("types do not match");
  checkit(e, false, true, false, true);
}
proc checkNo(type t, e) {
  checkit(t, false, false, false, false);
  if t != e.type then
    compilerError("types do not match");
  checkit(e, false, false, false, false);
}

class C { var x: int; }
record R { var y: int; }
record ContainingNilableOwned { var zq: owned C?; }
record ContainingNonNilableOwned { var z: owned C; }
record CustomContainingNilableOwned {
  var zq: owned C?;
  proc init=(other) {
    this.zq = new owned C();
  }
}
proc =(ref lhs: CustomContainingNilableOwned,
       const ref rhs: CustomContainingNilableOwned) {
  lhs.zq = new owned C();
}

record CustomContainingNonNilableOwned {
  var z: owned C;
  proc init=(other) {
    this.z = new owned C();
  }
}
proc =(ref lhs: CustomContainingNonNilableOwned,
       const ref rhs: CustomContainingNonNilableOwned) {
  lhs.z = new owned C();
}

enum MyEnum { val }

proc main() {

  var ownedC = new owned C();
  var ownedCQ = new owned C?();
  var sharedC = new shared C();
  var sharedCQ = new shared C?();
  var unmanagedC = new unmanaged C();
  var unmanagedCQ = new unmanaged C?();

  checkNormal(int, 1);
  checkNormal(real, 1.0);
  checkNormal(bool, true);
  checkNormal(MyEnum, MyEnum.val);
  checkNormal(R, new R(1));
  checkNormal(borrowed C, ownedC.borrow());
  checkNormal(borrowed C?, ownedCQ.borrow());
  checkNormal(unmanaged C, unmanagedC);
  checkNormal(unmanaged C?, unmanagedCQ);
  checkNormal(shared C, sharedC);
  checkNormal(shared C?, sharedCQ);

  // nilable owned can generally be copied / assigned but only from a ref
  checkMutable(owned C?, ownedCQ);

  // non-nilable owned cannot generally be copied / assigned 
  checkNo(owned C, ownedC);

  checkMutable(ContainingNilableOwned, new ContainingNilableOwned());
  checkNo(ContainingNonNilableOwned,
          new ContainingNonNilableOwned(new owned C()));

  checkNormal(CustomContainingNilableOwned, new CustomContainingNilableOwned());
  checkNormal(CustomContainingNonNilableOwned,
              new CustomContainingNonNilableOwned(new owned C()));

  delete unmanagedCQ;
  delete unmanagedC;
}
