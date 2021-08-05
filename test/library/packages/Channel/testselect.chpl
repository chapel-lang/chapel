use Channel;

var chan1 = new chan(int, 1);
var chan2 = new chan(int, 1);

var x1, x2 : int;

var arr : [0..#2] shared SelBaseClass = [new shared SelCase(x1, chan1, selOperation.recv, 0) : SelBaseClass, new shared SelCase(x2, chan2, selOperation.recv, 1) : SelBaseClass];

chan1.send(5);
chan2.send(4);

var success = selectProcess(arr);
writeln((success == 0 && x1 == 5) || (success == 1 && x2 == 4));
