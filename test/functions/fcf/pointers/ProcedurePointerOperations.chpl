use Reflection;
use Types;

compilerAssert(useProcedurePointers);

inline proc printType(type t) do writeln(t:string);

var flag: atomic bool;

proc foo(): void { flag.write(true); }

inline proc invokeAndCheck(procPtr) {
  flag.clear();
  procPtr();
  assert(flag.read());
}

proc test0() {
  writeln(getRoutineName());

  const p = foo;
  param ok1 = chpl_enableProcPtrs(p.type);
  compilerAssert(ok1);
}

proc test1() {
  writeln(getRoutineName());

  // wide proc(): void
  const p1 = foo;
  assert(p1 != nil);
  assert(nil != p1);
  assert(!(p1 == nil));
  assert(!(nil == p1));
  assert(p1 == foo);
  invokeAndCheck(p1);

  // local proc(): void
  const p2 = chpl_toLocalProc(p1);
  assert(p2 != nil);
  assert(nil != p2);
  assert(!(p2 == nil));
  assert(!(nil == p2));
  invokeAndCheck(p2);

  // extern local proc(): void
  // Not legal to call as given, but we just check for 'nil'. 
  const p3 = __primitive("cast", chpl_toExternProcType(p2.type), p2);
  assert(p3 != nil);
  assert(nil != p3);
  assert(!(p3 == nil));
  assert(!(nil == p3));

  // extern wide proc(): void
  // Not legal to call as given, but we just check for 'nil'.
  const p4 = __primitive("cast", chpl_toExternProcType(p1.type), p1);
  assert(p4 != nil);
  assert(nil != p4);
  assert(!(p4 == nil));
  assert(!(nil == p4));
}

proc dupAndAssignTo(ref x) {
  var dup: x.type = x;
  x = dup;
  assert(x == dup);
}

proc test2() {
  writeln(getRoutineName());

  type T = proc(): void;

  var x1: T;                              // wide
  var x2: chpl_toLocalProcType(T);        // local
  var x3: chpl_toExternProcType(T);       // extern wide
  var x4: chpl_toExternProcType(x2.type); // extern local

  assert(x1 == nil);
  dupAndAssignTo(x1);

  assert(x2 == nil);
  dupAndAssignTo(x2);

  assert(x3 == nil);
  dupAndAssignTo(x3);

  assert(x4 == nil);
  dupAndAssignTo(x4);

  var y1 = foo;
  var y2 = chpl_toLocalProc(y1);

  assert(chpl_isWideProc(y1));
  assert(chpl_isLocalProc(y2));
  assert(chpl_isWideProc(y1.type));
  assert(chpl_isLocalProc(y2.type));

  assert(y1 != nil);
  const y1c = y1;
  dupAndAssignTo(y1);
  assert(y1c == y1);

  assert(y2 != nil);
  const y2c = y2;
  dupAndAssignTo(y2);
  assert(y2c == y2);
}


proc main() {
  test0();
  test1();
  test2();
}
