/* else should bind to second "if" */
/* note indenting is misleading */

writeln("Start");
if true
  if false
    writeln("Wrong 1");
else
  writeln("Correct");
writeln("Stop");
