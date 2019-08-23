use LockFreeQueue;

config const N = 1024 * 1024;

var lfq = new LockFreeQueue(int);
forall i in 1..N with (var tok = lfq.getToken()) {
    lfq.enqueue(i, tok);
}

var total : int;
forall i in 1..N with (var tok = lfq.getToken(), + reduce total) {
    total += lfq.dequeue(tok)[2];
}
var expected = + reduce (1..N);
assert(total == expected, total, "!=", expected);