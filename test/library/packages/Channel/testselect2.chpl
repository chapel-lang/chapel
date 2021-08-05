use Channel;

config const n = 4000;
config const size = 3;
var a, b, c, d : atomic int;

var chan1 = new chan(int, size);
var chan2 = new chan(int, size);

coforall i in 1..n with (ref chan1, ref chan2) {
    if i % 2 == 0 {
        var x1 : int;
        var x2 : int;

        var sel1 : SelBaseClass = new shared SelCase(x1, chan1, selOperation.recv, 0);
        var sel2 : SelBaseClass = new shared SelCase(x2, chan2, selOperation.recv, 1);

        var arr = [sel1, sel2];
        var success = selectProcess(arr);
        if success == 0 {
            a.add(x1);
        }
        else b.add(x2);
    }
    else {
        var x1 = i;
        var sel1 : SelBaseClass = new shared SelCase(x1, chan1, selOperation.send, 0);
        var sel2 : SelBaseClass = new shared SelCase(x1, chan2, selOperation.send, 1);

        var arr = [sel1, sel2];
        var success = selectProcess(arr);
        if success == 0 {
            c.add(i);
        }
        else d.add(i);

    }
}

writeln((a.read() == c.read()) && (b.read() == d.read()));
