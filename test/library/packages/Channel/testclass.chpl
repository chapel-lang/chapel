use Channel;

class C {
    var x : int;
}
var chan1 = new chan(unmanaged C?, 1);

var b = new unmanaged C(2);

begin {
    chan1.send(b);
}

var c : unmanaged C?;
chan1.recv(c);

writeln(c!);
delete b;

