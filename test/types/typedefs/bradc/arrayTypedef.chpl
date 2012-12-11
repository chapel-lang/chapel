type ArrType = [1..8] real;

config const doPrint=true;

proc main() {
  var Arr: ArrType;
  if doPrint then
    writeln("Arr is: ", Arr);
}
