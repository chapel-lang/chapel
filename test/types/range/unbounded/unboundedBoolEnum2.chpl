enum color {red, orange, yellow, green, blue, indigo, violet};
use color;

for i in ..blue do
  writeln(i);

writeln("---");

for i in green.. by -1 do
  writeln(i);

writeln("---");

for i in ..blue by 2 do
  writeln(i);

writeln("---");

for i in green.. by -2 do
  writeln(i);

writeln("---");

for i in ..true do
  writeln(i);

writeln("---");

for i in ..true by 2 do
  writeln(i);

writeln("---");

for i in false.. by -1 do
  writeln(i);

writeln("---");

for i in false.. by -2 do
  writeln(i);

writeln("---");

for i in ..false do
  writeln(i);

writeln("---");

for i in true.. by -1 do
  writeln(i);

writeln("---");

var re: range(color, boundKind.neither) = ..;
var rb: range(bool, boundKind.neither) = ..;

for i in re do
  writeln(i);

writeln("---");

for i in rb do
  writeln(i);

writeln("---");

var reps: range(color, boundKind.neither, strides = strideKind.any) = .. by 2;
var rbps: range(bool, boundKind.neither, strides = strideKind.any) = .. by 2;

for i in reps do
  writeln(i);

writeln("---");

for i in rbps do
  writeln(i);

writeln("---");

var rens: range(color, boundKind.neither, strides = strideKind.any) = .. by -2;
var rbns: range(bool, boundKind.neither, strides = strideKind.any) = .. by -2;

for i in rens do
  writeln(i);

writeln("---");

for i in rbns do
  writeln(i);

writeln("---");

testit(..blue);
testit(green.. by -1);
testit(..true);
testit(false.. by -1);
testit(..false);
testit(true.. by -1);
testit(..blue by 2);
testit(green.. by -2);

proc testit(r) {
  for i in r do
    writeln(i);
  writeln("---");
}
