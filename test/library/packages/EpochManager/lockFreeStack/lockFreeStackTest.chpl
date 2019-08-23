use LockFreeStack;

config const N = 1024 * 1024;

var lfs = new LockFreeStack(int);
forall i in 1..N with (var tok = lfs.getToken()) {
    lfs.push(i, tok);
}

var total : int;
forall i in 1..N with (var tok = lfs.getToken(), + reduce total) {
    total += lfs.pop(tok)[2];
}
var expected = + reduce (1..N);
assert(total == expected, total, "!=", expected);