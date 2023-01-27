use DistributedList;

config const n = 100;

const a = [i in 0..<n] i;
var dl = new distributedList(a);

record incrementByIdx {
    proc this(idx: int, ref value: int) {
        value += idx;
    }
}
var updater = new incrementByIdx();

forall i in 0..<n with (ref dl) {
    dl.update(i, updater);
}

writeln(&& reduce ([(x, i) in zip(dl, 0..)] x == i * 2));
