// IC function calls required functions on the interfaces
// of the associated constraints.

interface I1 {
  proc chpl__initCopy(arg: Self, definedConst: bool): Self;
  proc write(arg: Self): void;
  proc reqFn1(arg: Self);
  proc get1(arg: Self): AT1;
  type AT1;
  AT1 implements I2;
}

interface I2 {
  proc chpl__initCopy(arg: Self, definedConst: bool): Self;
  proc write(arg: Self): void;
  proc reqFn2(arg: Self);
  proc get2(arg: Self): AT2;
  type AT2;
  AT2 implements I3;
}

interface I3 {
  proc chpl__initCopy(arg: Self, definedConst: bool): Self;
  proc write(arg: Self): void;
  proc reqFn3(arg: Self);
  proc get3(arg: Self): AT3;
  type AT3;
  // AT3 implements ... //and so on
}

/////////////////////////////////

proc icFun(x1: ?Q1) where Q1 implements I1 {
  writeln("icFun()");

  reqFn1(x1);  // this is the base case, see the previous example

  const x2 = get1(x1);  // x2: x1.AT1
  reqFn2(x2);   // legal because AT1 implements I2

  const x3 = get2(x2);   // x3: x1.AT1.AT2
  reqFn3(x3);   // legal because AT2 implements I3

  get3(x3);   // x1.AT1.AT2.AT3
  // and so on
}

/////////////////////////////////

icFun(1);

// int --> all ATs are ints

int implements I1;
proc reqFn1(arg: int) { writeln("reqFn1.int(", arg, ")"); }
proc get1(arg: int) return arg * 10;
proc int.AT1 type return get1(this).type;

int implements I2;
proc reqFn2(arg: int) { writeln("reqFn2.int(", arg, ")"); }
proc get2(arg: int) return arg * 20;
proc int.AT2 type return get2(this).type;

int implements I3;
proc reqFn3(arg: int) { writeln("reqFn3.int(", arg, ")"); }
proc get3(arg: int) return arg * 30;
proc int.AT3 type return get3(this).type;

/////////////////////////////////

icFun(2.2);

// real --> ATs are: complex -> string -> int

real implements I1;
proc reqFn1(arg: real) { writeln("reqFn1.real(", arg, ")"); }
proc get1(arg: real) return (arg, -arg): complex;
proc real.AT1 type return get1(this).type;

complex implements I2;
proc reqFn2(arg: complex) { writeln("reqFn2.complex(", arg, ")"); }
proc get2(arg: complex) return arg: string;
proc complex.AT2 type return get2(this).type;

string implements I3;
proc reqFn3(arg: string) { writeln("reqFn3.string(", arg, ")"); }
proc get3(arg: string) return arg.size;
proc string.AT3 type return get3(this).type;
