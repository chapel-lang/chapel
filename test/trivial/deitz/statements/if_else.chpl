/* else should bind to second "if" */
/* note indenting is misleading */

writeln("Start");
if true then
  if false then
    writeln("Wrong 1");
else
  writeln("Correct");
writeln("Stop");
