use Channel;

var chan1 = new channel(int, 5);

begin {
    chan1.send(4);
}
var recv1 : int;
chan1.recv(recv1);
writeln("Received buffered ", recv1);
