use Channel;

var chan1 = new chan(int, 1);
var chan2 = new chan(int, 1);

var x1, x2 : int;

var arr : [0..#2] shared SelectBaseClass = [new shared SelectCase(x1, chan1, selectOperation.recv, 0) : SelectBaseClass, new shared SelectCase(x2, chan2, selectOperation.recv, 1) : SelectBaseClass];

chan1.send(5);
chan2.send(4);

var success = selectProcess(arr);
writeln((success == 0 && x1 == 5) || (success == 1 && x2 == 4));
