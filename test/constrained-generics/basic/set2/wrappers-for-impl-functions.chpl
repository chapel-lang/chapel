
interface IFC {
  proc take(in arg: Self): Self;
  Self implements StdOps;
}

interface StdOps(Val) {
  operator ==(lhs: Val, rhs: Val): bool;
  proc chpl__initCopy(arg: Val, definedConst: bool): Val;
  proc write(arg: Val): bool;
}


proc ic(arg1: ?Q, arg2: Q) where Q implements IFC {
  write("ic   arg1 "); write(arg1); write("  arg2 "); write(arg2);
  writeln(if arg1 == arg2 then "  equal" else "  different");
  return take(arg1);
}

/////////////////////////////////

proc take(in arg: real) param : int {
  writeln("take real ", arg);  // not printed
  return 543;
}

implements StdOps(int);
implements IFC(int);
writeln(ic(345,345));

/////////////////////////////////

proc take(in arg: string, extraArg = 357): string {
  writeln("take string ", arg);
  return arg;
}

implements StdOps(string);
implements IFC(string);
writeln(ic("hi", "there"));

/////////////////////////////////

proc take(arg: bool): bool(8) {
  writeln("take bool ", arg);
  return arg;
}

implements StdOps(bool(8));
implements IFC(bool(8));
writeln(ic(false: bool(8), true: bool(8)));

/////////////////////////////////

class MyClass { var x: int; }

proc take(in arg: MyClass): arg.type {
  writeln("take ", arg, " : ", arg.type:string);
  return arg;
}

const c1 = new MyClass(1);
const c2 = new MyClass(2);

implements StdOps(borrowed MyClass);
implements IFC(borrowed MyClass);

ic(c1.borrow(), c2.borrow());
ic(c2.borrow(), c2.borrow());

// NB 'owned MyClass' does not implement StdOps --
// because its chpl__initCopy() takes its arg by ref intent.
//
//implements StdOps(owned MyClass);

/////////////////////////////////

record RR { var xx: int; }

proc take(in arg: RR): RR {
  writeln("take RR ", arg);
  return arg;
}

RR implements StdOps;
RR implements IFC;

var rr = new RR(55);
var qq = ic(rr, new RR(66));
writeln("qq ", qq);

