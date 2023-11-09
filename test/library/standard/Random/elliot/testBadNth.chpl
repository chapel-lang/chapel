use Random;

config const getNth = false;

var r = new randomStream(real, 314159265);

if getNth {
  r.getNth(-1);
} else {
  r.skipToNth(-1);
}
