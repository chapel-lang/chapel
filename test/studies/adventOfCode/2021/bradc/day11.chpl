use IO;

config const steps = 100;

var n = 10;

var D = {1..n, 1..n};
var BigD = {0..n+1, 0..n+1};
var Energy: [BigD] atomic int;

readEnergies();
writeln("Before any steps:\n", Energy[D]);
var flashes: atomic int;
for i in 1..steps {
  Energy[D].add(1);
  var Flashed: [D] bool;
  do {
    var flashed = false;
    forall ij in D with (ref flashed) do
      if (Energy[ij].read() > 9 && Flashed[ij] == false) {
        Flashed[ij] = true;
        flashes.add(1);
        flashed = true;
        forall off in {-1..1, -1..1} do
          if off != (0,0) then
            Energy[ij+off].add(1);
      }
  } while flashed;
  forall ij in D do
    if Flashed[ij] then
      Energy[ij].write(0);
  writeln("After step ", i, ":\n", Energy[D]);
}
writeln("total flashes: ", flashes.read());


proc readEnergies() {
  var line: string;
  var row = 0;
  while readLine(line) {
    line = line.strip();
    row += 1;
    stringToRow(line, row);
  }
}

proc stringToRow(str: string, row) {
  for (ch, c) in zip(str, 1..) do
    Energy[row, c].write(ch: int);
}
