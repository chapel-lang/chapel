proc foo(a: int, b: int) {
}

proc foo(a: string, b: int) {
}

proc foo(a: string, b: string) {
}

proc foo(a: real, b: real) {
}

proc foo(a, b) where a.type == bytes && b.type == bytes {
}

foo(1, "hello");
