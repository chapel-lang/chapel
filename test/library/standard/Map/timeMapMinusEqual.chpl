use Map, Time;

config const size = 1_000_000, trials = 10;
config param printTimes = false;

var m1 = new map(int, real);
var m2 = new map(int, real);

for i in 0..#size {
  m2.add(i, i:real);
}

m1 = m2;

var opTime = new Timer();
var totTime = new Timer();
totTime.start();
for trial in 0..#trials {
  opTime.start();
  doMinusEqual(m1, m2);
  opTime.stop();
  m1 = m2;
}
totTime.stop();

if printTimes {
  writeln("-= time = ", opTime.elapsed());
  writeln("Total time = ", totTime.elapsed());
}

proc doMinusEqual(ref m1, const ref m2) {
  m1 -= m2;
}
