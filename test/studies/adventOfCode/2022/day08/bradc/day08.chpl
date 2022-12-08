const Lines = readLines();

iter readLines() {
  use IO;

  var line: string;
  while readLine(line, stripNewline=true) do
    yield line;
}

const numRows = Lines.size,
      numCols = Lines[0].size,
      ForestSpace = {0..<numRows, 0..<numCols};

var Forest = [(r,c) in ForestSpace] Lines[r][c]:int;

proc visible((r,c), height: int) {
  return && reduce (Forest[..<r, c] < height) ||
         && reduce (Forest[r+1.., c] < height) ||
         && reduce (Forest[r, ..<c] < height) ||
         && reduce (Forest[r, c+1..] < height);
}

writeln(+ reduce visible(ForestSpace, Forest));
