use Channel;

config const debug = false;
var chan1 = new channel(int);
begin {
  try {
    chan1.send(5);
  } catch e {
    if debug then writeln("Error: ", e.message());
  }
}

chan1.close();
var x : int;
var ok = chan1.recv(x);
if debug then writeln((x, ok));
