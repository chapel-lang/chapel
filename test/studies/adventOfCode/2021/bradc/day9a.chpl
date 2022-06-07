use IO;

var rows, cols: int;

var D: domain(2);
var Height: [D] int;

readHeights();
Height[0, ..] = 9;
Height[rows+1, ..] = 9;
Height[.., 0] = 9;
Height[.., cols+1] = 9;
writeln(Height);

var Visited: [D] atomic bool;

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

proc findBasinSize(r, c): int {
  if Height[r,c] == 9 || Visited[r, c].read() {
    return 0;
  } else {
    Visited[r, c].write(true);
    return 1 + (findBasinSize(r+1, c) +
                findBasinSize(r-1, c) +
                findBasinSize(r, c-1) +
                findBasinSize(r, c+1));
  }
}

proc getBasinSize(r, c) {
  if (Height[r, c] < Height[r+1, c] &&
      Height[r, c] < Height[r, c+1] &&
      Height[r, c] < Height[r-1, c] &&
      Height[r, c] < Height[r, c-1]) {
    return findBasinSize(r, c);
  } else {
    return 0;
  }
}

var basinSizes = [(r, c) in {1..rows, 1..cols}] getBasinSize(r, c);
writeln(basinSizes);

iter findMax(k) {
  for i in 1..k {
    const (biggest, loc) = maxloc reduce zip(basinSizes, {1..rows, 1..cols});
    yield biggest;
    basinSizes[loc] = 0;
  }
}

const total = * reduce findMax(3);
writeln(total);
