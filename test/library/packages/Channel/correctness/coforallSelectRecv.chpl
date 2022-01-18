use Channel;

config const n = 4000;
config const size = 3;
var a, b, c : atomic int;

var chan1 = new channel(int, size);
var chan2 = new channel(int, size);

coforall i in 1..n with (ref chan1, ref chan2) {
  if i % 4 == 0 {
    chan1.send(i);
    a.add(i);
  }
  else if i % 4 == 1 {
    chan2.send(i);
    b.add(i);
  }
  else {
    var x1 : int, x2 : int;
    var sel1 : SelectBaseClass = new shared SelectCase(x1, chan1, selectOperation.recv, 0);
    var sel2 : SelectBaseClass = new shared SelectCase(x2, chan2, selectOperation.recv, 1);

    var arr = [sel1, sel2];
    selectProcess(arr);
    c.add(x1);
    c.add(x2);
  }
}

writeln((a.read() + b.read()) == c.read());
