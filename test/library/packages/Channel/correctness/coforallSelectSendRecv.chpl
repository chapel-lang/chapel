use Channel;

config const n = 4000;
config const size = 3;
var a, b, c, d : atomic int;

var chan1 = new channel(int, size);
var chan2 = new channel(int, size);

coforall i in 1..n with (ref chan1, ref chan2) {
    if i % 2 == 0 {
        var x1 : int;
        var x2 : int;

        var sel1 : SelectBaseClass = new shared SelectCase(x1, chan1, selectOperation.recv, 0);
        var sel2 : SelectBaseClass = new shared SelectCase(x2, chan2, selectOperation.recv, 1);

        var arr = [sel1, sel2];
        var success = selectProcess(arr);
        if success == 0 {
            a.add(x1);
        }
        else b.add(x2);
    }
    else {
        var x1 = i;
        var sel1 : SelectBaseClass = new shared SelectCase(x1, chan1, selectOperation.send, 0);
        var sel2 : SelectBaseClass = new shared SelectCase(x1, chan2, selectOperation.send, 1);

        var arr = [sel1, sel2];
        var success = selectProcess(arr);
        if success == 0 {
            c.add(i);
        }
        else d.add(i);

    }
}

writeln((a.read() == c.read()) && (b.read() == d.read()));
