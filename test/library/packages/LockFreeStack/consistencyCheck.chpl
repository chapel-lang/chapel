use LockFreeStack;
use Time;

config const N = 1024 * 1024;
config const printTiming = false;
var expected = + reduce (1..N);


var lfs = new LockFreeStack(int);

var timer = new Timer();
timer.start();

forall i in 1..N {
    lfs.push(i);
}

var total : int;
forall i in 1..N with (+ reduce total) {
    total += lfs.pop()[1];
}
assert(total == expected, total, "!=", expected);
timer.stop();
lfs.tryReclaim();
if printTiming then writeln("No Token: ", timer.elapsed());
timer.clear();

timer.start();
forall i in 1..N with (var tok = lfs.getToken()) {
    lfs.push(i, tok);
}

total = 0;
forall i in 1..N with (var tok = lfs.getToken(), + reduce total) {
    total += lfs.pop(tok)[1];
}
assert(total == expected, total, "!=", expected);
lfs.tryReclaim();
timer.stop();
if printTiming then writeln("Token: ", timer.elapsed());
timer.clear();

timer.start();
forall i in 1..N with (var tok = lfs.getToken()) {
    lfs.push(i, tok);
}

total = + reduce lfs.drain();
assert(total == expected, total, "!=", expected);
lfs.tryReclaim();
if printTiming then writeln("Drain: ", timer.elapsed());
timer.clear();
