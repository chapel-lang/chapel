config param i: int = 1;

proc main() {
  if (i == 1) then
    compilerWarning("i is 1");
  else if (i == 0) then
    compilerWarning("i is 0");
  else
    compilerWarning("i is something else");
  
  writeln("Ran program");
}

