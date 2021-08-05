use Channel;

var chan1 = new chan(int);


begin {
    chan1.send(1);
    chan1.close();
}
var x1 : int;
var x = chan1.recv(x1);
var y1 : int;
var y = chan1.recv(y1);

writeln((x1, x));
writeln((y1, y));



