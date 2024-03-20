// https://github.com/chapel-lang/chapel/issues/20481#issuecomment-1224354640

use Reflection;

record Foo {
    type t = int(32);
    var s = "hi";
}

var foo = new Foo();

for param i in 0..<getNumFields(foo.type) {
    if isType(getField(foo,i)) {
        continue;
    }
    writeln(getField(foo, i));
}

/////////////////////////////////////////////////////////////////////////////

config param flag = true;

proc p() type {
  if flag then
    return int;

  return string;
}

compilerWarning(p():string);

proc p(param flag) {
  if flag then
    return 3.5;

  return "bye";
}

writeln(p(true));
writeln(p(false));

/////////////////////////////////////////////////////////////////////////////

class Parent {
  proc f() throws {
    return here;
  }
  iter itr(): int {
    if flag then return;
    yield "hi"; // note that this line is ignored
  }
}

class Child: Parent {
  override proc f(): locale throws {
    throw new Error("in Child.f()");
  }
  override iter itr(): int {
    if flag then return;
  }
}

var obj = new Parent();
classtest();

obj = new Child();
classtest();

proc classtest() {
  try { writeln(obj.f()); }
  catch e { writeln(e); }

  for idx in obj.itr() {
    compilerWarning(idx.type:string, 0);
    writeln(idx);
  }
}

/////////////////////////////////////////////////////////////////////////////

proc t1(arg:int) {
  writeln("in t1");
  if arg > 1 {
    return 55;
    writeln("after return 55");
    compilerError("after return 55");
  } else {
    return 77;
    writeln("after return 77");
    compilerError("after return 77");
  }
  writeln("after if arg > 1");
  compilerError("after if arg > 1");
}

proc t2() throws {
  writeln("in t2");
  throw new Error();
  writeln("after throw");
  compilerError("after throw");
}

proc main {
  writeln(t1(1), t1(2));
  try { t2(); } catch { }
  halt("in main");
  writeln("after halt");
  compilerError("after halt");
}
