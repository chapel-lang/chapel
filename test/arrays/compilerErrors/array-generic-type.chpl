// Should this be allowed?
// For now, generate a parser error.

type T = [];

proc test(type arg) { }
test(T);
test([]);
