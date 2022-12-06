use IO, Set;

config const matchSize = 4;

var line: bytes;
while readLine(line) {
  writeln(line);

  for i in 0..<(line.size-matchSize) {
    var s: set(uint(8));

    for ch in line[i..#matchSize] do
      s.add(ch);

    if s.size == matchSize {
      writeln(i+matchSize);
      break;
    }
  }
}