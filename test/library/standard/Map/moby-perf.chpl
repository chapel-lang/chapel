use IO;
use Map;
use Time;

config const inputFile = "moby.txt";
config const repeat = 10;
config const timing = false;

proc main() {
  var reader = open(inputFile, iomode.r).reader();

  var m = new map(string, string, parSafe=false);

  var t: Timer;
  t.start();
  for line in reader.lines() {
    for i in 1..repeat {
      for word in line.split() {
        m.add(word, word);
      }
    }
  }
  t.stop();

  if timing then
    writeln("Chapel map elapsed time: ", t.elapsed(), " s");
}
