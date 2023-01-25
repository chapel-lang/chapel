use IO;

var f: file;

try {
  try {
    f = open("foo", iomode.cw);
    if (!f.isOpen()) {
      writeln("There was a problem with the file");
    }
    writeln(f.path);
    f.close();
  } catch e: SystemError {
    writeln("open error:  " + e.details);  
  }
  if (!f.isOpen()) {
    writeln("File was closed successfully");
  } else {
    writeln("You shouldn't see this");
  }
} catch {
  halt("catchall");
}