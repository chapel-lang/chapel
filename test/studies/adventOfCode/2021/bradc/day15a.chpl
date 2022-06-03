use Heap, IO;

var rows, cols: int;

var D: domain(2);
var Risk, Dist: [D] int;
var Prev: [D] 2*int;

readRisk();
writeln(Risk);

const oldrows = rows, oldcols = cols;

rows *= 5;
cols *= 5;

const Dorig = D;
D = {1..rows, 1..cols};
writeln(Risk[Dorig]);
forall ij in D by (oldrows, oldcols) {
  const offset = ij / (oldrows, oldcols);
  Risk[Dorig.translate(ij-(1,1))] = Risk[Dorig] + offset(0) + offset(1);
}
forall r in Risk do
  while r > 9 do
    r -= 9;
writeln(Risk);

var Q: domain(2*int);
Dist = max(int);
Prev = (0,0);
forall ij in D with (ref Q) do
  Q.add(ij);
Dist(1,1) = 0;


// TODO: Must be a cleverer way, though maybe all that's missing is a
// priority queue?  I was too lazy, so just let it run for ~4 minutes
// on my Mac instead for the full problem size
while !Q.isEmpty() {
  if Q.size % 1000 == 0 then
    writeln(Q.size);
  const (d,u) = minloc reduce [v in Q] (Dist[v], v);
  Q -= u;
  if u == (rows, cols) {
    writeln(Dist[u]);
    break;
  }
  for (i,j) in {-1..1, -1..1} {
    if abs(i+j) == 1 {
      var v = u+(i,j);
      if (D.contains(v) && Q.contains(v)) {
        const alt = Dist(u) + Risk[v];
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
