use Channel;

var chan1 = new channel(int);

chan1.close();
try {
  chan1.close();
} catch e {
  writeln("Error: ", e.message());
}
