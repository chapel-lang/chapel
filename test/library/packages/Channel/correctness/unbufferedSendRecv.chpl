use Channel;

var chan1 = new channel(int);

begin {
  chan1.send(5);
}
var recv1 : int = 1;
chan1.recv(recv1);
writeln("Received unbuffered ", recv1);
