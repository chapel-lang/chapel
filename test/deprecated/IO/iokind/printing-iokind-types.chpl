use IO;
use Subprocess;

proc main() {
  writeln(fileReader(iokind.native, false):string);
  writeln(fileWriter(iokind.native, false):string);
  writeln(fileReader(false):string);
  writeln(fileWriter(false):string);
  writeln(subprocess(iokind.native, false):string);
  writeln(subprocess(false):string);
}

