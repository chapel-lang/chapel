use Heap, IO;

config const debug = false;

var rows, cols: int;

var D: domain(2);
var Risk, Dist: [D] int;
var Prev: [D] 2*int;

readRisk();
writeln(Risk);

var Q: domain(2*int);
Dist = max(int);
Prev = (0,0);
forall ij in D with (ref Q) do
  Q.add(ij);
Dist(1,1) = 0;

while !Q.isEmpty() {
  const (d,u) = minloc reduce [v in Q] (Dist[v], v);
  if debug {
    writeln("Processing ", u);
    writeln(Q);
  }
  Q -= u;
  if debug then
    writeln(Q);
  if u == (rows, cols) {
    writeln(Dist[u]);
    break;
  }
  for (i,j) in {-1..1, -1..1} {
    if abs(i+j) == 1 {
      var v = u+(i,j);
      if debug then
        writeln("Trying ", v);
      if (D.contains(v) && Q.contains(v)) {
        if debug then
          writeln("It's still in play");
        const alt = Dist(u) + Risk[v];
        if debug then
          writeln("Distance is ", alt);
        if alt < Dist[v] {
          Dist[v] = alt;
          Prev[v] = u;
        }
      }
    }
  }
    
}
  

proc readRisk() {
  var line: string;
  while readLine(line) {
    line = line.strip();
    rows += 1;
    if cols == 0 then cols = line.size;
    D = {1..rows, 1..cols};
    stringToRow(line, rows);
  }
}

proc stringToRow(str: string, row) {
  for (ch, c) in zip(str, 1..) do
    Risk[row, c] = ch: int;
}
