use IO;

var line: string;

iter getOutputs() {
  while (stdin.readline(line)) {
    //  writeln("Got: ", line);
    var parts: [1..2] string = line.split(" | ");
    //  writeln("Got: ", (parts[1], parts[2]));
    var outdigits: [1..4] string = parts[2].strip().split(" ");
    //    writeln("Got output digits: ", outdigits);
    for digit in outdigits do
      yield digit;
  }
}

var Outputs = getOutputs();
/*
for output in Outputs {
  const size = output.size;
  if 
    writeln(output, ": ", output.size);
}
*/

proc uniqueDigit(str: string) {
  const size = str.size;
  return (size >=  2 && size <= 4) || size == 7;
}

const count = + reduce uniqueDigit(Outputs);
writeln(count);


//writeln(Outputs);s
