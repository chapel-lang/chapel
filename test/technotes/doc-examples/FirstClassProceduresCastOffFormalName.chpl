//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
proc foo(x: int, y: int): int { return x + y; }
proc bar(a: int, b: int): int { return a + b; }

// Here the formals of 'T' are anonymous.
type T = proc(_: int, _: int): int;

var x: T = foo;             // OK, T's formals are anonymous '_'.
writeln(foo.type:string);   // 'proc(x: int, y: int): int'
writeln(x.type:string);     // 'proc(_: int, _: int): int'
writeln(bar.type:string);   // 'proc(a: int, b: int): int'
x = bar;                    // OK!
/* STOP_EXAMPLE_0 */

assert(x(2, 2) == 4);

// They should all produce the same result.
const a = x(4, 4);
const b = foo(4, 4);
const c = bar(4, 4);
assert(a == b && b == c);
