/*
This script generates the various combinations of lhs and rhs class types
for `lhs = rhs` in various contexts of initialization/assignment, fields/
vars, etc.

It is always an error upon --no-legacy-classes when
`lhs` is non-nilable while `rhs` is nilable or `nil`.

It is always OK when `lhs` is nilable and `rhs` is `nil`.

Otherwise, `lhs` and `rhs` are OK for nilability. Then, an error is given
for the combinations of memory management that are not "ok" in this table:

    | lhs       | owned | shared | borrowed | unmanaged |
    | :-------- | :---: | :----: | :------: | :-------: |
    | owned     |  ok   |        |          |           |
    | shared    |  ok   |  ok    |          |           |
    | borrowed  |  ok   |  ok    |  ok      |  ok       |
    | unmanaged |       |        |          |  ok       |
*/

use FileSystem;

enum MM { mOwn, mShr, mBor, mUnm, mNil };
use MM;

const allM = mOwn..mUnm;
const allMnil = mOwn..mNil;
proc isNil(arg:MM) return arg==mNil;

// Is 'lhs = rhs' OK w.r.t. memory management?
const mmOK = makeMMOK();

private proc makeMMOK() {
  var result: [allM, allMnil] bool;
  proc ok(lhs:MM, rhs:MM) { result[lhs,rhs] = true; }
  proc er(lhs:MM, rhs:MM) { result[lhs,rhs] = false; } //no-op, really

  // See the above table; plus a row for mNil.
  ok(mOwn,mOwn);  er(mOwn,mShr);  er(mOwn,mBor);  er(mOwn,mUnm);  ok(mOwn,mNil);
  ok(mShr,mOwn);  ok(mShr,mShr);  er(mShr,mBor);  er(mShr,mUnm);  ok(mShr,mNil);
  ok(mBor,mOwn);  ok(mBor,mShr);  ok(mBor,mBor);  ok(mBor,mUnm);  ok(mBor,mNil);
  er(mUnm,mOwn);  er(mUnm,mShr);  er(mUnm,mBor);  ok(mUnm,mUnm);  ok(mUnm,mNil);

  return result;
}

/////////////////////////////////////////////////////////////////////////////

proc MM.strm {
  select this {
    when mOwn do return "owned";
    when mShr do return "shared";
    when mBor do return "borrowed";
    when mUnm do return "unmanaged";
    when mNil do return "nil";
  }
  return "<invalid>";
}

proc bool.strn return if this then "oknil" else "nonnil";

proc str(nlb:bool, mm:MM)  return mm.strm + if nlb then "?" else "!";

proc errstr(errNlb:bool, errM:bool) return
  if errNlb then
    if errM then "errors: nlb, mm" else "error: nlb"
  else
    if errM then "error: mm" else "ok";

proc checkConfig(lhsNlb: bool, lhsM: MM, rhsNlb: bool, rhsM: MM,
                 skipIfNonNilableNil: bool)
{
  assert(lhsM != mNil);
  if rhsM == mNil && ! rhsNlb {
    if skipIfNonNilableNil then return true;
    else halt("invalid combination");
  }
  return false;
// another option: rhsNlb = rhsNlbArg | rhsM == nil;
}

/////////////////////////////////////////////////////////////////////////////
// todo: factor our common code between cInitVar() and similar procs below

config const ivDir = "nilability-init-var";

proc cInitVar(lhsNlb: bool, lhsM: MM, rhsNlb: bool, rhsM: MM,
              skipIfNNNil = false)
{
  if checkConfig(lhsNlb, lhsM, rhsNlb, rhsM, skipIfNNNil) then return;
  const errNlb = !lhsNlb && rhsNlb;
  const errM   = ! mmOK(lhsM, rhsM);
  const errS   = errstr(errNlb, errM);

  var path = "init-var-%s-%s-from-%s-%s".format(
    lhsNlb.strn, lhsM.strm, rhsNlb.strn, rhsM.strm);
  writeln(path);
  if ! ivDir.isEmpty() {
    mkdir(ivDir, parents=true); path = ivDir + "/" + path; }
  var chan = open(path + ".chpl", iomode.cw).writer();
  proc write(args...) { chan.writef((...args)); }

  var rhsDef = "", rhsUse = "rhs";
  if rhsM == mNil then rhsUse = "nil";
  else if rhsNlb then rhsDef = 'var rhs: %s MyClass?;'.format(rhsM.strm);
  else                rhsDef = 'var rhs = new %s MyClass();'.format(rhsM.strm);

  var lhsQ = if lhsNlb then "?" else "";

  var lastStmt = "";
  if errNlb || errM then lastStmt = 'compilerError("done");';
  else if rhsM == mUnm && !rhsNlb then lastStmt = 'delete rhs;';
    
  write("//  lhs: %s  rhs: %s  %s\n",
        str(lhsNlb, lhsM), str(rhsNlb, rhsM), errS);
  write("""
class MyClass {
  var x: int;
}

%s

var lhs: %s MyClass%s = %s;

%s
""", rhsDef, lhsM.strm, lhsQ, rhsUse, lastStmt);

  // now, the .good file
  var good = open(path + ".good", iomode.cw).writer();
  if errNlb || errM then
    good.writeln(errS);
  // else leave it empty
}

writeln(); writeln();
writeln("cInitVar: dir=", ivDir);
writeln(); writeln();
for lhs in allM {
  for rhs in allMnil {
    cInitVar(true, lhs, true, rhs);
    cInitVar(true, lhs, false, rhs, true);
    cInitVar(false, lhs, true, rhs);
    cInitVar(false, lhs, false, rhs, true);
    writeln();
  }
  writeln();
}

/////////////////////////////////////////////////////////////////////////////

config const dfiDir = "nilability-init-field-dflt";

proc cDefaultFieldInit(lhsNlb: bool, lhsM: MM, rhsNlb: bool, rhsM: MM,
                       skipIfNNNil = false)
{
  if checkConfig(lhsNlb, lhsM, rhsNlb, rhsM, skipIfNNNil) then return;
  const errNlb = !lhsNlb && rhsNlb;
  const errM   = ! mmOK(lhsM, rhsM);
  const errS   = errstr(errNlb, errM);

  var path = "init-field-dflt-%s-%s-from-%s-%s".format(
    lhsNlb.strn, lhsM.strm, rhsNlb.strn, rhsM.strm);
  writeln(path);
  if ! dfiDir.isEmpty() {
    mkdir(dfiDir, parents=true); path = dfiDir + "/" + path; }
  var chan = open(path + ".chpl", iomode.cw).writer();
  proc write(args...) { chan.writef((...args)); }

  var rhsDef = "", rhsUse = "rhs";
  if rhsM == mNil then rhsUse = "nil";
  else if rhsNlb then rhsDef = 'var rhs: %s MyClass?;'.format(rhsM.strm);
  else                rhsDef = 'var rhs = new %s MyClass();'.format(rhsM.strm);

  var lhsQ = if lhsNlb then "?" else "";

  var lastStmt = "";
  if errNlb || errM then lastStmt = 'compilerError("done");';
  else if rhsM == mUnm && !rhsNlb then lastStmt = 'delete rhs;';
    
  write("//  lhs: %s  rhs: %s  %s\n",
        str(lhsNlb, lhsM), str(rhsNlb, rhsM), errS);
  write("""
class MyClass {  var x: int;  }

%s

record MyRecord {
  var lhs: %s MyClass%s = %s;
}

var myr = new MyRecord();

%s
""", rhsDef, lhsM.strm, lhsQ, rhsUse, lastStmt);

  // now, the .good file
  var good = open(path + ".good", iomode.cw).writer();
  if errNlb || errM then
    good.writeln(errS);
  // else leave it empty
}

writeln(); writeln();
writeln("cDefaultFieldInit: dir=", dfiDir);
writeln(); writeln();
for lhs in allM {
  for rhs in allMnil {
    cDefaultFieldInit(true, lhs, true, rhs);
    cDefaultFieldInit(true, lhs, false, rhs, true);
    cDefaultFieldInit(false, lhs, true, rhs);
    cDefaultFieldInit(false, lhs, false, rhs, true);
    writeln();
  }
  writeln();
}

/////////////////////////////////////////////////////////////////////////////

config const afiDir = "nilability-init-field-arg";

proc cArgumentFieldInit(lhsNlb: bool, lhsM: MM, rhsNlb: bool, rhsM: MM,
                        skipIfNNNil = false)
{
  if checkConfig(lhsNlb, lhsM, rhsNlb, rhsM, skipIfNNNil) then return;
  const errNlb = !lhsNlb && rhsNlb;
  const errM   = ! mmOK(lhsM, rhsM);
  const errS   = errstr(errNlb, errM);

  var path = "init-field-arg-%s-%s-from-%s-%s".format(
    lhsNlb.strn, lhsM.strm, rhsNlb.strn, rhsM.strm);
  writeln(path);
  if ! afiDir.isEmpty() {
    mkdir(afiDir, parents=true); path = afiDir + "/" + path; }
  var chan = open(path + ".chpl", iomode.cw).writer();
  proc write(args...) { chan.writef((...args)); }

  var rhsDef = "", rhsUse = "rhs";
  if rhsM == mNil then rhsUse = "nil";
  else if rhsNlb then rhsDef = 'var rhs: %s MyClass?;'.format(rhsM.strm);
  else                rhsDef = 'var rhs = new %s MyClass();'.format(rhsM.strm);

  var lhsQ = if lhsNlb then "?" else "";

  var lastStmt = "";
  if errNlb || errM then lastStmt = 'compilerError("done");';
  else if rhsM == mUnm && !rhsNlb then lastStmt = 'delete rhs;';
    
  write("//  lhs: %s  rhs: %s  %s\n",
        str(lhsNlb, lhsM), str(rhsNlb, rhsM), errS);
  write("""
class MyClass {  var x: int;  }

%s

record MyRecord {
  var lhs: %s MyClass%s;
  proc init(in rhs) where ! isSubtype(rhs.type, MyRecord) {
    lhs = rhs;
  }
}

var myr = new MyRecord(%s);

%s
""", rhsDef, lhsM.strm, lhsQ, rhsUse, lastStmt);

  // now, the .good file
  var good = open(path + ".good", iomode.cw).writer();
  if errNlb || errM then
    good.writeln(errS);
  // else leave it empty
}

writeln(); writeln();
writeln("cArgumentFieldInit: dir=", afiDir);
writeln(); writeln();
for lhs in allM {
  for rhs in allMnil {
    cArgumentFieldInit(true, lhs, true, rhs);
    cArgumentFieldInit(true, lhs, false, rhs, true);
    cArgumentFieldInit(false, lhs, true, rhs);
    cArgumentFieldInit(false, lhs, false, rhs, true);
    writeln();
  }
  writeln();
}

/////////////////////////////////////////////////////////////////////////////

config const asgnDir = "nilability-assign";

proc cAssign(lhsNlb: bool, lhsM: MM, rhsNlb: bool, rhsM: MM,
             skipIfNNNil = false)
{
  if checkConfig(lhsNlb, lhsM, rhsNlb, rhsM, skipIfNNNil) then return;
  const errNlb = !lhsNlb && rhsNlb;
  const errM   = ! mmOK(lhsM, rhsM);
  const errS   = errstr(errNlb, errM);

  var path = "assign-%s-%s-from-%s-%s".format(
    lhsNlb.strn, lhsM.strm, rhsNlb.strn, rhsM.strm);
  writeln(path);
  if ! asgnDir.isEmpty() {
    mkdir(asgnDir, parents=true); path = asgnDir + "/" + path; }
  var chan = open(path + ".chpl", iomode.cw).writer();
  proc write(args...) { chan.writef((...args)); }

  var rhsDef = "", rhsUse = "rhs";
  if rhsM == mNil then rhsUse = "nil";
  else if rhsNlb then rhsDef = 'var rhs: %s MyClass?;'.format(rhsM.strm);
  else                rhsDef = 'var rhs = new %s MyClass();'.format(rhsM.strm);

  var lhsDef = "";
  if lhsNlb then lhsDef = 'var lhs: %s MyClass?;'.format(lhsM.strm);
  else           lhsDef = 'var lhs = new %s MyClass();'.format(lhsM.strm);

  var compErr = if errNlb || errM then 'compilerError("done");\n' else '';
  var delRhs  = if rhsM == mUnm && !rhsNlb then 'delete rhs;\n' else '';
  var delLhs  = if lhsM == mUnm && !lhsNlb then 'delete lhs;\n' else '';
    
  write("//  lhs: %s  rhs: %s  %s\n",
        str(lhsNlb, lhsM), str(rhsNlb, rhsM), errS);
  write("""
class MyClass {  var x: int;  }

%s
%s

lhs = %s;

%s%s%s
""",
lhsDef, rhsDef, rhsUse, compErr, delRhs, delLhs);

  // now, the .good file
  var good = open(path + ".good", iomode.cw).writer();
  if errNlb || errM then
    good.writeln(errS);
  // else leave it empty
}

writeln(); writeln();
writeln("cAssign: dir=", asgnDir);
writeln(); writeln();
for lhs in allM {
  for rhs in allMnil {
    cAssign(true, lhs, true, rhs);
    cAssign(true, lhs, false, rhs, true);
    cAssign(false, lhs, true, rhs);
    cAssign(false, lhs, false, rhs, true);
    writeln();
  }
  writeln();
}
