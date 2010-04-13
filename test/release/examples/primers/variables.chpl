/*
 * Variables Example
 *
 * This example demonstrates variable declaration syntax.
 *
 */ 

//
// Variables are declared with the 'var' keyword.  Variable
// declarations must have a type, initializer, or both.  Here, we
// declare myVariable to be an int.  Since there was no initializer,
// it is initialized to the default value for the int type: 0.
//
var myVariable1: int;
writeln("myVariable1 = ", myVariable1);

//
// Here we declare a variable without a type.  Since 1.618 is a real
// value, the variable is inferred to be of type real.  Note that we
// print out the type by passing the type of the variable (via
// myVariable2.type) to the typeToString function.
//
var myVariable2 = 1.618;
writeln("myVariable2 = ", myVariable2, " (type = ",
        typeToString(myVariable2.type), ")");

//
// Instead of 'var', 'const' and 'param' can be used to declare
// runtime constants and compile-time constants respectively.  A
// 'const' must be initialized in place, but can have its value
// generated at runtime.  A 'param' must be known at compile time.
//
const myConst: real = sqrt(myVariable2);
param myParam = 3.14;
writeln("myConst = ", myConst, ", myParam = ", myParam);

//
// At module scope, all three variable kinds can be qualified by the
// 'config' keyword.  This allows the initial value to be overridden
// on the command line.  A 'config var' or 'config const' may be
// overridden when the program is executed; a 'config param' may be
// overridden when the program is compiled.  The comment following
// each declaration shows how the value can be modified.
//
config var cfgVar = "hello";         // ./a.out --cfgVar="world"
config const cfgConst: bool = false; // ./a.out --cfgConst=true
config param cfgParam = 4;           // chpl variables.chpl -s cfgParam=1
writeln("cfgVar = ", cfgVar,
        ", cfgConst = ", cfgConst,
        ", cfgParam = ", cfgParam);

//
// Variable types and values propagate to the left.  Here, 'a' is a
// real initialized to 1.0, 'b' and 'c' are integers initialized to 0,
// 'd' and 'e' are reals initialized to 1.0, and f is a real
// initialized to 0.0.
//
var a = 1.0, b, c: int, d, e = 1.0, f: real;
writeln("a = ", a, ", b = ", b, ", c = ", c,
        ", d = ", d, ", e = ", e, ", f = ", f);
