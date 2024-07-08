use IO;

var line: string;

while readLine(line, stripNewline=true) {
  writeln(line, " (", line.size, ")");
  if line.size {
    const calories = line:int;
    writeln(calories);
    writeln();
  }
}
