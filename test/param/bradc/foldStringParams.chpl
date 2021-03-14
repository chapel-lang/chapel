config param s = "true";

proc main() {
  if (s == "true") then
    compilerWarning("s is true");
  else if (s == "false") then
    compilerWarning("s is false");
  else
    compilerWarning("s is something else");

  writeln("Ran program");
}

