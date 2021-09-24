use Channel;

class C {
    var x : int;
}
var chan1 = new channel(shared C?, 1);

begin {
    var b = new shared C(2)?;
    chan1.send(b);
}

var c : shared C?;
chan1.recv(c);

writeln(c!);

