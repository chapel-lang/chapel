config param s: string = "true";

def main() {
  if (s == "true") then
    compilerWarning("s is true");
  else if (s == "false") then
    compilerWarning("s is false");
  else
    compilerWarning("s is something else");

  writeln("Ran program");
}

