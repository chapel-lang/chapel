var a : int = 1;
var b : int = 0;

writeln("Start");
if b then
  a = 0;

if a then
  writeln("Good");

if b then writeln("Good");

if a then
  if b then
    writeln("Wrong 1");
else
  writeln("Correct");

if a then
  if b then
    writeln("Wrong 1");
else
  writeln("Correct");

if true then
  if false then
    writeln("Wrong 1");
else
  writeln("Correct");
writeln("Stop");
