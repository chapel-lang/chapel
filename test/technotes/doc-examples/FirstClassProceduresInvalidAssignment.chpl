//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
proc foo(x: int, y: int): int { return x + y; }
proc bar(a: int, b: int): int { return a + b; }
writeln(foo.type:string);   // 'proc(x: int, y: int): int'
var x = foo;                // 'x' is typed 'proc(x: int, y: int): int'
writeln(bar.type:string);   // 'proc(a: int, b: int): int'
x = bar;                    // Error!
/* STOP_EXAMPLE_0 */
