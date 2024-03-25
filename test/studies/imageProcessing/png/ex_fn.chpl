
/*****
      ex_fn.chpl -
      Program demonstrating named arguments, default arguments, argument
      intents, and nested functions.  See text for description.

      Call:
        ex_fn

      c 2015-2018 Primordial Machine Vision Systems
*****/


writeln();
proc tstfn1(arg1 : int, arg2 : int) {
  writeln("tstfn1:  arg1 = ",arg1, "   arg2 = ", arg2);
}

tstfn1(arg2=3, arg1=1);


writeln();
proc tstfn2(arg1 : real = 2.718, arg2 : string = "dummy string") {
  writeln("tstfn2:  arg1 = ",arg1, "   arg2 = ", arg2);
}

tstfn2();
tstfn2(3.14);
tstfn2(arg2="a new string");
tstfn2(arg1=1.41, "another string");
tstfn2("another string", arg1=1.41);


writeln();
proc tstfn3a {
  writeln("tstfn3a:  no args");
}

proc tstfn3b() {
  writeln("tstfn3b:  no args");
}

tstfn3a;
tstfn3b();


writeln();
proc tstfn4(in arg1 : int, out arg2 : real, inout arg3 : int, ref arg4 : real,
            const in arg5 : int, const ref arg6 : real) {
  arg1 = arg1 - 1;
  arg2 = arg1 + 1;   /* This will be the original value of arg1. */
  arg3 = arg3 + arg5 - 2;
  arg4 = arg6 + 1;
  /* Either of these two would cause a compilation error.
     arg5 = arg1 - 1;
     arg6 = 2 * arg2;
  */
}

var call1 = 3;
var call2 = 1.618;
var call3 = 7;
var call4 = 29.98e-9;
var call5 = 5;
var call6 = 9.807;

tstfn4(call1, call2, call3, call4, call5, call6);
writeln("tstfn4:  call1 = ", call1, "   call2 = ", call2, "   call3 = ", call3);
writeln("         call4 = ", call4, "   call5 = ", call5, "   call6 = ", call6);

/* These are compilation errors - passed constants to output arguments. */
/*
const call7 = 3.0;
const call8 = 3;
tstfn4(call1, call7, call3, call4, call5, call6);
tstfn4(call1, call2, call8, call4, call5, call6);
tstfn4(call1, call2, call3, call7, call5, call6);
*/


writeln();
proc tstfn5(arg1 : int) {
  if (arg1 < 5) {
    return 5;
  } else if (arg1 < 10) {
    return 6.0;
  } else {
    return 1.0 + 1.0i;
  }
}

var ret5 = tstfn5(3);
writeln("tstfn5:  real part ", ret5.re, "   complex ", ret5.im);
ret5 = tstfn5(7);
writeln("tstfn5:  real part ", ret5.re, "   complex ", ret5.im);
ret5 = tstfn5(11);
writeln("tstfn5:  real part ", ret5.re, "   complex ", ret5.im);


writeln();
proc tstfn6(arg1 : int) {
  const val1 = 3.14;
  proc tstfn6b(arg1 : real) {
    writeln("tstfn6:  arg1 = ", arg1, "   val1 = ", val1);
  }
  tstfn6b(arg1 + val1);
}

tstfn6(3);
