// Make sure that, for the most part, you can use the new syntax
// when compiling in legacy mode.

type T1 = proc(x: int, y: int): int;
type T2 = func(int, int, int);

assert(T1 != T2);

type T3 = proc(_: int, _: int): int;

// TODO: These are not equal? Huh...
writeln(T2:string);
writeln(T3:string);

const p1 = lambda(x: int, y: int): int { return x + y; };
const p2 = proc(x: int, y: int): int { return x + y; };

assert(p1.type == p2.type);

writeln(p1(2, 2));
writeln(p2(2, 2));

