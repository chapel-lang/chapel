use Time;

param NUM = 10;

var signal: [1..NUM] sync bool;


begin {
  sleep( 2);
  for i in 1..NUM do
      signal[i].writeEF(true);
}

for i in 1..NUM do
  write( i, ":", signal[i].readFE());
writeln();
