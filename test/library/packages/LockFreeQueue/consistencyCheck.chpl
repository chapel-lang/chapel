use LockFreeQueue;
use Time;

config const N = 1024 * 1024;
config const printTiming = false;
var expected = + reduce (1..N);


var lfq = new LockFreeQueue(int);

var timer = new Timer();
timer.start();

forall i in 1..N {
    lfq.enqueue(i);
}

var total : int;
forall i in 1..N with (+ reduce total) {
    total += lfq.dequeue()[1];
}
assert(total == expected, total, "!=", expected);
timer.stop();
lfq.tryReclaim();
if printTiming then writeln("No Token: ", timer.elapsed());
timer.clear();

timer.start();
forall i in 1..N with (var tok = lfq.getToken()) {
    lfq.enqueue(i, tok);
}

total = 0;
forall i in 1..N with (var tok = lfq.getToken(), + reduce total) {
    total += lfq.dequeue(tok)[1];
}
assert(total == expected, total, "!=", expected);
lfq.tryReclaim();
timer.stop();
if printTiming then writeln("Token: ", timer.elapsed());
timer.clear();

timer.start();
forall i in 1..N with (var tok = lfq.getToken()) {
    lfq.enqueue(i, tok);
}

total = + reduce lfq.drain();
assert(total == expected, total, "!=", expected);
lfq.tryReclaim();
if printTiming then writeln("Drain: ", timer.elapsed());
timer.clear();
