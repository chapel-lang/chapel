use IO;

config const debug = false;

iter getOutputs() {
  var line: string;

  while readLine(line) {
    if debug then
      writeln("Got: ", line);
    var parts: [1..2] string = line.split(" | ");
    if debug then
      writeln("Got: ", (parts[1], parts[2]));
    var outdigits: [1..4] string = parts[2].strip().split(" ");
    if debug then
      writeln("Got output digits: ", outdigits);

    for digit in outdigits do
      yield digit;
  }
}

var Outputs = getOutputs();

if debug then
  for output in Outputs do
    writeln(output, ": ", output.size);

proc uniqueDigit(str: string) {
  const size = str.size;
  return (size >=  2 && size <= 4) || size == 7;
}

const count = + reduce uniqueDigit(Outputs);
writeln(count);

if debug then
  writeln(Outputs);
