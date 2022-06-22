use IO;

config const debug = false;

var rows, cols: int;

var D: domain(2);
var Height: [D] int;

readHeights();
Height[0, ..] = 9;
Height[rows+1, ..] = 9;
Height[.., 0] = 9;
Height[.., cols+1] = 9;
writeln(Height);

proc stringToRow(str: string, row) {
  for (ch, c) in zip(str, 1..) do
    Height[row, c] = ch: int;
}

proc readHeights() {
  var line: string;
  while readLine(line) {
    line = line.strip();
    rows += 1;
    if cols == 0 then cols = line.size;
    D = {0..rows+1, 0..cols+1};
    stringToRow(line, rows);
  }
}

writeln(Height.domain);
forall (r, c) in {1..rows, 1..cols} {
  if (Height[r, c] < Height[r+1, c] &&
      Height[r, c] < Height[r, c+1] &&
      Height[r, c] < Height[r-1, c] &&
      Height[r, c] < Height[r, c-1]) {
    if debug then
      writeln("Found low point at ", (r, c), " = ", Height[r, c]);
  }
}

proc getLowPointRisk(r, c) {
  if (Height[r, c] < Height[r+1, c] &&
      Height[r, c] < Height[r, c+1] &&
      Height[r, c] < Height[r-1, c] &&
      Height[r, c] < Height[r, c-1]) {
    return Height[r, c] + 1;
  } else {
    return 0;
  }
}

const risklevel = + reduce [(r, c) in {1..rows, 1..cols}] getLowPointRisk(r, c);

writeln(risklevel);
