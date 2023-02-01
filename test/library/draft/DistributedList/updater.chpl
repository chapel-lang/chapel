use DistributedList;

config const n = 100;

const a = [i in 0..<n] i;
var dl = new distributedList(a);

record incrementByIdx {
    proc this(idx: int, ref value: int) {
        value += idx;
        return idx;
    }
}
var updater = new incrementByIdx(),
    sum = 0;

forall i in 0..<n with (ref dl, + reduce sum) {
    sum += dl.update(i, updater);
}

writeln(sum == n*(n-1)/2);
writeln(&& reduce ([(x, i) in zip(dl, 0..)] x == i * 2));
