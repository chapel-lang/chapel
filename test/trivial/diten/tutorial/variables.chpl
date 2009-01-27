/*
 * Variables Example
 *
 * This example demonstrates variable declaration syntax.
 *
 */ 


//
// Variables are declared with the 'var' keyword. Variable declarations
// must have a type, initialization expression, or both. Here, we declare
// myVariable to be an int. Since there was no initializer, it is initialized
// the the default value for the int type: 0.
//
var myVariable1: int;
writeln(myVariable1);

//
// This declaration omits the type. Since 1.618 is a real value, the variable
// is inferred to be of type real.
//
var myVariable2 = 1.618;
writeln(myVariable2);

//
// Instead of 'var', 'const' or 'param' can be used to declare runtime
// constants and compile time constants respectively. A 'const' must be
// initialized in place, but can have its value generated at runtime. A
// 'param' must be known at compile time.
const myConst: real = sqrt(myVariable2);
param myParam = 3.14;
writeln(myConst, ", ", myParam);

//
// At module scope, all three variable kinds can also be declared with the
// 'config' keyword. This allows the initial value to be overridden on the
// command line. A 'config var' or 'config const' can be overridden when
// the program is executed, a 'config param' can be overridden when compiled.
// The comment following each declaration shows how the value can be modified.
//
config var cfgVar = "hello";         // ./a.out --cfgVar="world"
config const cfgConst: bool = false; // ./a.out --cfgConst=true
config param cfgParam = 4;           // chpl variables.chpl -s cfgParam=1
writeln(cfgVar, ", ", cfgConst, ", ", cfgParam);

//
// Variable types and values can be cascaded from the right. Here, 'a' is a
// real with the value 1.0, 'b' and 'c' are integers with default value,
// 'd' and 'e' are reals with value 1.0 and f is a real with default value.
//
var a = 1.0, b, c: int, d, e = 1.0, f: real;
writeln(a, ", ", b, ", ", c, ", ", d, ", ", e, ", ", f);

