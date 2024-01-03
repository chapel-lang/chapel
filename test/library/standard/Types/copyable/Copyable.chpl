use Types;

// Common testing utilities for copyability, assignability, and
// default-initability.

// enable skipping test of default initializability, which can't be called
// on generic types at time of writing, so we can check the other properties on
// them
config param checkDefaultInitializable = true;

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

  if checkDefaultInitializable then
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

  if checkDefaultInitializable then
    if isDefaultInitializable(e) != expectDefaultInitable then
      compilerError("isDefaultInitializable " + e.type:string + " did not match");
}


proc checkNormal(type t, e, param onlyType:bool = false) {
  checkit(t, true, true, true, true, true);
  if !onlyType {
    if t != e.type then
      compilerError("types do not match");
    checkit(e, true, true, true, true, true);
  }
}
proc checkNormalNoDefault(type t, e, param onlyType:bool = false) {
  checkit(t, true, true, true, true, false);
  if !onlyType {
    if t != e.type then
      compilerError("types do not match");
    checkit(e, true, true, true, true, false);
  }
}
proc checkMutable(type t, e, param onlyType:bool = false) {
  checkit(t, true, false, true, false, true);
  if !onlyType {
    if t != e.type then
      compilerError("types do not match");
    checkit(e, true, false, true, false, true);
  }
}
proc checkNo(type t, e, param onlyType:bool = false) {
  checkit(t, false, false, false, false, false);
  if !onlyType {
    if t != e.type then
      compilerError("types do not match");
    checkit(e, false, false, false, false, false);
  }
}
