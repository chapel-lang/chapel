config const help = false;

if help then printUsage();

proc printUsage() {
  writeln("This is my custom usage message!");
  exit(0); 
}

writeln("Hello!");
