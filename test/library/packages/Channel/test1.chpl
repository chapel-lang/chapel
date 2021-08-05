use Channel;

var chan1 = new chan(int);

begin {
   chan1.send(5);
}
var recv1 : int = 1;
chan1.recv(recv1);
writeln("Received unbuffered ", recv1);

var chan2 = new chan(int, 5);

begin {
   chan2.send(4);
}
var recv2 : int;
chan2.recv(recv2);
writeln("Received buffered ", recv2);


