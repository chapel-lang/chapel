use IO;

var cycle = 1;
var maxCycles = 2;
var CycleSpace = {1..maxCycles};
var Delta: [CycleSpace] int;

var line: string;
while readLine(line, stripNewline=true) {
  if cycle+1 > maxCycles {
    maxCycles *= 2;
    CycleSpace = {1..maxCycles};
  }

  const (op, space, val) = line.partition(" ");
  select op {
    when "noop" {
      cycle += 1;
    }
    when "addx" {
      cycle += 1;  // leave a gap for the stall
      Delta[cycle] = val:int;
      cycle += 1;
    }
  }
}

// shrink to necessary size
CycleSpace = {1..<cycle};

Delta[1] = 1;
writeln(Delta);
var X = + scan Delta;
writeln(X);
X *= CycleSpace;
writeln(X);
writeln(X[20..maxCycles by 40]);
